#include "optimizeCW.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <map>
#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TH2.h>
#include <TCanvas.h>

void optimizeCW::OptimizeCWdata(std::string source,int sectorID,int RoI,int CW, TFile* file){

    std::cout<<"<<Optimize CW>>"<<std::endl;

    TH2I *LowMap = new TH2I("LowMap",";#DeltaR;#Delta#phi",31,-15.5,15.5,31,-15.5,15.5);
    TH2I *HighMap = new TH2I("highMap",";#DeltaR;#Delta#phi",31,-15.5,15.5,31,-15.5,15.5);
    TH2F *h_Low = new TH2F("h_Low",";#Delta#phi;#DeltaR",15,-7.5,7.5,31,-15.5,15.5);
    TH2F *h_High = new TH2F("h_High",";#Delta#phi;#DeltaR",15,-7.5,7.5,31,-15.5,15.5);

    int mapID;
    for(int sec=0;sec!=sectorID;sec++){
    for(int roi=0;roi!=RoI;roi++){
    if(source=="Ec"){mapID = roi + (sectorID%6)*148;}
    if(source=="Fw"){mapID = roi + (sectorID%6)*64;} ///?

    for(int cw=0;cw!=CW;cw++){
        std::stringstream MAP,PT;
        MAP << std::setw(3) << std::setfill('0') << mapID;
        PT << "_pT" << std::setw(2) << std::setfill('0') << cw;
        std::string HistName="HitMap_" + source + MAP.str() + PT.str();
        TH2I *HitMap = (TH2I*)file->Get(HistName.c_str());
        this->HitMapCleaner(HitMap);

        std::string BottomName="";
        if(cw<=22){BottomName="HitMap_" + source + MAP.str() + "_pT00";}
        else if(cw>22){
            std::stringstream BPT;
            BPT << "_pT" << std::setw(2) << std::setfill('0') << (2*cw)-39;
            BottomName="HitMap_" + source + MAP.str() + BPT.str();
        }
        TH2I *BottomMap = (TH2I*)file->Get(BottomName.c_str());
        this->HitMapCleaner(BottomMap);

        std::string TopName = "";
        if(cw>22){TopName="HitMap_" + source + MAP.str() + "_pT39";}
        else if(cw<=22 && cw>5){
            std::stringstream TPT;
            TPT << "_pT" << std::setw(2) << std::setfill('0') << (2*cw)-5;
            TopName="HitMap_" + source + MAP.str() + TPT.str();
        }
        else if(cw<=5){TopName="HitMap_" + source + MAP.str() + "_pT08";}
        TH2I *TopMap = (TH2I*)file->Get(TopName.c_str());
        this->HitMapCleaner(TopMap);

        LowMap->Add(BottomMap,1);
        LowMap->Add(HitMap,-1);

        HighMap->Add(HitMap,1);
        HighMap->Add(TopMap,-1);

        this->FillHitMap(LowMap, HighMap, h_Low, h_High, sec, roi, cw);

        LowMap->Reset();
        HighMap->Reset();
        h_Low->Reset();
        h_High->Reset();
    }
    }
    }
}

void optimizeCW::HitMapCleaner(TH2I *hs){

  int YbinN = hs->GetYaxis()->GetNbins();
  int XbinN = hs->GetXaxis()->GetNbins();

  for(int Xbin=1;Xbin!=XbinN+1;Xbin++){
      for(int Ybin=1;Ybin!=YbinN+1;Ybin++){
          if(hs->GetBinContent(Xbin,Ybin)==1){  
              hs->SetBinContent(Xbin,Ybin,0);
          }
      }
  }
}

void optimizeCW::HitMapCleanerStage2(TH2F *hs){

  int YbinN = hs->GetYaxis()->GetNbins();
  int XbinN = hs->GetXaxis()->GetNbins();

////Oldest Algorism////
  std::vector<int> isolatedBinX;
  std::vector<int> isolatedBinY;
  std::vector<int> holeBinX;
  std::vector<int> holeBinY;

  for(int Xbin=1;Xbin!=XbinN+1;Xbin++){
      for(int Ybin=1;Ybin!=YbinN+1;Ybin++){
          int Naround=0;
          for(int i=-1;i!=2;i++){
              for(int j=-1;j!=2;j++){
                  if(i==0 && j==0){continue;}
                  if(hs->GetBinContent(Xbin+i,Ybin+j)!=0){Naround += 1;}
              }
          }
          if(Naround>=5 ){
              holeBinX.push_back(Xbin);
              holeBinY.push_back(Ybin);
          }
      }
  }
  for(int i=0;i!=holeBinX.size();i++){
    hs->SetBinContent(holeBinX[i],holeBinY[i],1);
  }

  for(int Xbin=1;Xbin!=XbinN+1;Xbin++){
      for(int Ybin=1;Ybin!=YbinN+1;Ybin++){
          int Naround=0;
          for(int i=-1;i!=2;i++){
              for(int j=-1;j!=2;j++){
                  if(i==0 && j==0){continue;}
                  if(hs->GetBinContent(Xbin+i,Ybin+j)!=0){Naround += 1;}
              }
          }
          if(Naround<=1 ){
              isolatedBinX.push_back(Xbin);
              isolatedBinY.push_back(Ybin);
          }
      }
  }
  for(int i=0;i!=isolatedBinX.size();i++){
      hs->SetBinContent(isolatedBinX[i],isolatedBinY[i],0);
  }
}

void optimizeCW::FillHitMap(TH2I *LowMap, TH2I *HighMap, TH2F *h_Low, TH2F *h_High,int sec,int roi, int cw)
{

    //int YbinN = LowMap->GetYaxis()->GetNbins();
    //int XbinN = LowMap->GetXaxis()->GetNbins();
    int YbinN = 15;
    int XbinN = 31;
    int YOffset = 8;

    for(int Xbin=1;Xbin!=XbinN+1;Xbin++){
        for(int Ybin=1;Ybin!=YbinN+1;Ybin++){
            int LowContent = LowMap->GetBinContent(Xbin,Ybin+YOffset);
            h_Low->SetBinContent(Ybin,Xbin,LowContent);
            int HighContent = HighMap->GetBinContent(Xbin,Ybin+YOffset);
            h_High->SetBinContent(Ybin,Xbin,HighContent);
        }
    }
    float TotalLow = h_Low->Integral();
    float TotalHigh = h_High->Integral();
    if(TotalLow>=TotalHigh){h_Low->Scale(TotalHigh/TotalLow);}
    else if(TotalLow<TotalHigh){h_High->Scale(TotalLow/TotalHigh);}

    for(int Xbin=1;Xbin!=XbinN+1;Xbin++){
        for(int Ybin=1;Ybin!=YbinN+1;Ybin++){
            float h = h_High->GetBinContent(Ybin,Xbin);
            if(h==0){continue;}
            float l = h_Low->GetBinContent(Ybin,Xbin);
            float x = h/sqrt(l*l + h*h);

            if(x>0.2){h_RenewMap[sec][roi][cw]->SetBinContent(Ybin,Xbin,x);}
        }
    }
    this->HitMapCleanerStage2(h_RenewMap[sec][roi][cw]);

    for(int Xbin=1;Xbin!=XbinN+1;Xbin++){
        for(int Ybin=1;Ybin!=YbinN+1;Ybin++){
            if(h_RenewMap[sec][roi][cw]->GetBinContent(Ybin,Xbin)!=0){h_RenewMap[sec][roi][cw]->SetBinContent(Ybin,Xbin,cw);}
        }
    }
}

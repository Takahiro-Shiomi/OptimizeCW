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

    int mapID;
    if(source=="Ec"){mapID = RoI + (sectorID%6)*148;}
    if(source=="Fw"){mapID = RoI + (sectorID%6)*64;} ///?

    std::stringstream MAP,PT;
    MAP << std::setw(3) << std::setfill('0') << mapID;
    PT << "_pT" << std::setw(2) << std::setfill('0') << CW;
    std::string HistName="HitMap_" + source + MAP.str() + PT.str();
    std::cout << "Optimize CW: " << HistName.c_str() << std::endl;
    HitMap = (TH2I*)file->Get(HistName.c_str());
    this->HitMapCleaner(HitMap);

    std::string BottomName="";
    if(CW<=22){BottomName="HitMap_" + source + MAP.str() + "_pT00";}
    else if(CW>22){
        std::stringstream BPT;
        BPT << "_pT" << std::setw(2) << std::setfill('0') << (2*CW)-39;
        BottomName="HitMap_" + source + MAP.str() + BPT.str();
    }
    std::cout << "Bottom CW: " << BottomName.c_str() << std::endl;
    BottomMap = (TH2I*)file->Get(BottomName.c_str());
    this->HitMapCleaner(BottomMap);

    std::string TopName = "";
    if(CW>22){TopName="HitMap_" + source + MAP.str() + "_pT39";}
    else if(CW<=22 && CW>5){
        std::stringstream TPT;
        TPT << "_pT" << std::setw(2) << std::setfill('0') << (2*CW)-5;
        TopName="HitMap_" + source + MAP.str() + TPT.str();
    }
    else if(CW<=5){TopName="HitMap_" + source + MAP.str() + "_pT08";}
    std::cout << "Top CW: " << TopName.c_str() << std::endl;
    TopMap = (TH2I*)file->Get(TopName.c_str());
    this->HitMapCleaner(TopMap);

    LowMap->Add(BottomMap,1);
    LowMap->Add(HitMap,-1);

    HighMap->Add(HitMap,1);
    HighMap->Add(TopMap,-1);

    this->FillHitMap();

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

void optimizeCW::FillHitMap()
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
    std::cout<<TotalLow<<"__"<<TotalHigh<<std::endl;
    if(TotalLow>=TotalHigh){h_Low->Scale(TotalHigh/TotalLow);}
    else if(TotalLow<TotalHigh){h_High->Scale(TotalLow/TotalHigh);}

    for(int Xbin=1;Xbin!=XbinN+1;Xbin++){
        for(int Ybin=1;Ybin!=YbinN+1;Ybin++){
            float h = h_High->GetBinContent(Ybin,Xbin);
            if(h==0){continue;}
            float l = h_Low->GetBinContent(Ybin,Xbin);
            float x = h/sqrt(l*l + h*h);

            h_Ratio->SetBinContent(Ybin,Xbin,x);
            if(x>0.2){h_RenewMap->SetBinContent(Ybin,Xbin,x);}
        }
    }
    this->HitMapCleanerStage2(h_RenewMap);
}

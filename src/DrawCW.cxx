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

void optimizeCW::DrawCWdata(TString pdf,int sec,int roi,int cw)
{
    TCanvas *c1 = new TCanvas("c1","c1",450,720);
    c1->Print(pdf + "[", "pdf");
   
    for(int i=0;i!=sec;i++){ 
    for(int j=0;j!=roi;j++){
    for(int k=0;k!=cw;k++){ 
        h_RenewMap[i][j][k]->GetXaxis()->SetNdivisions(15,kFALSE);
        h_RenewMap[i][j][k]->GetYaxis()->SetNdivisions(31,kFALSE);
        h_RenewMap[i][j][k]->GetZaxis()->SetRangeUser(0,36);
        h_RenewMap[i][j][k]->Draw("L COL text");
        h_RenewMap[i][j][k]->SetStats(0);
        h_RenewMap[i][j][k]->SetMarkerColor(0);
        c1->SetGrid();
        //c1->Print(pdf,"pdf");
        c1->Clear();
    }
    }
    }

    c1->Print(pdf + "]", "pdf");
    delete c1;
}

void optimizeCW::MakeHistgram()
{
    for(int i=0;i!=6;i++){
    for(int j=0;j!=148;j++){
    for(int k=0;k!=35;k++){
        h_RenewMap[i][j][k] = new TH2F(Form("HitMap_Ec%d_pT%d",(i*148)+j,k),";#Delta#phi;#DeltaR",15,-7.5,7.5,31,-15.5,15.5);
    }
    }
    }
}

void optimizeCW::DeleteHist()
{
    for(int i=0;i!=6;i++){
    for(int j=0;j!=148;j++){
    for(int k=0;k!=35;k++){
        if(h_RenewMap[i][j][k]!=0){delete h_RenewMap[i][j][k];}
    }
    }
    }
}

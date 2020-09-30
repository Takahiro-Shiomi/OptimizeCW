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

void optimizeCW::DrawCWdata(TString pdf)
{
    TCanvas *c1 = new TCanvas("c1","c1",800,800);
    c1->Print(pdf + "[", "pdf");
    
    HitMap->Draw("L COL text");
    HitMap->SetStats(0);
    c1->Print(pdf,"pdf");

    BottomMap->Draw("L COL text");
    BottomMap->SetStats(0);
    c1->Print(pdf,"pdf");

    TopMap->Draw("L COL text");
    TopMap->SetStats(0);
    c1->Print(pdf,"pdf");

    LowMap->Draw("L COL text");
    LowMap->SetStats(0);
    c1->Print(pdf,"pdf");

    HighMap->Draw("L COL text");
    HighMap->SetStats(0);
    c1->Print(pdf,"pdf");

    h_Low->GetXaxis()->SetNdivisions(15,kFALSE);
    h_Low->GetYaxis()->SetNdivisions(31,kFALSE);
    h_Low->Draw("L COL text");
    h_Low->SetStats(0);
    c1->SetGrid();
    c1->Print(pdf,"pdf");
    c1->Clear();

    h_High->GetXaxis()->SetNdivisions(15,kFALSE);
    h_High->GetYaxis()->SetNdivisions(31,kFALSE);
    h_High->Draw("L COL text");
    h_High->SetStats(0);
    c1->SetGrid();
    c1->Print(pdf,"pdf");
    c1->Clear();

    h_Ratio->GetXaxis()->SetNdivisions(15,kFALSE);
    h_Ratio->GetYaxis()->SetNdivisions(31,kFALSE);
    h_Ratio->Draw("L COL text");
    h_Ratio->SetStats(0);
    c1->SetGrid();
    c1->Print(pdf,"pdf");
    c1->Clear();

    h_RenewMap->GetXaxis()->SetNdivisions(15,kFALSE);
    h_RenewMap->GetYaxis()->SetNdivisions(31,kFALSE);
    h_RenewMap->Draw("L COL text");
    h_RenewMap->SetStats(0);
    c1->SetGrid();
    c1->Print(pdf,"pdf");
    c1->Clear();

    c1->Print(pdf + "]", "pdf");
    delete c1;
}

void optimizeCW::MakeHistgram()
{
    LowMap = new TH2I("LowMap",";#DeltaR;#Delta#phi",31,-15.5,15.5,31,-15.5,15.5);
    HighMap = new TH2I("highMap",";#DeltaR;#Delta#phi",31,-15.5,15.5,31,-15.5,15.5);

    h_Low = new TH2F("h_Low",";#Delta#phi;#DeltaR",15,-7.5,7.5,31,-15.5,15.5);
    h_High = new TH2F("h_High",";#Delta#phi;#DeltaR",15,-7.5,7.5,31,-15.5,15.5);

    h_Ratio = new TH2F("h_Ratio",";#Delta#phi;#DeltaR",15,-7.5,7.5,31,-15.5,15.5);
    h_RenewMap = new TH2F("h_RenewMap",";#Delta#phi;#DeltaR",15,-7.5,7.5,31,-15.5,15.5);
}

void optimizeCW::DeleteHist()
{
    if(HitMap!=0){delete HitMap;}
    if(BottomMap!=0){delete BottomMap;}
    if(TopMap!=0){delete TopMap;}
    if(LowMap!=0){delete LowMap;}
    if(HighMap!=0){delete HighMap;}
    if(h_Low!=0){delete h_Low;}
    if(h_High!=0){delete h_High;}
    if(h_Ratio!=0){delete h_Ratio;}
    if(h_RenewMap!=0){delete h_RenewMap;}
}

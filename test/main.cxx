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
#include <typeinfo>
#include "TString.h"
#include <TFile.h>
#include <TH2.h>

main(int argc, char *argv[])
{
    std::cout<<"<<Run START>>"<<std::endl;

    std::string InputFile="";
    if(argc!=0){InputFile = argv[1];}
    std::cout<<"Input File = "<<InputFile.c_str()<<std::endl;

    std::string source= argv[2];
    if(source=="Endcap"){source="Ec";}
    if(source=="Forward"){source="Fw";}
    Int_t sectorID=atoi(argv[3]);
    Int_t RoI=atoi(argv[4]);
    Int_t CW=atoi(argv[5]);

    TString PdfLabel = argv[6];

    TFile *fout = new TFile(Form("./../rootfile/%s.root", PdfLabel.Data()), "recreate");

    optimizeCW *CWoptimizer = new optimizeCW();
    //Make Histgram
    CWoptimizer->MakeHistgram();

    TFile *file = new TFile(InputFile.c_str());
    //Optimize CW
    CWoptimizer->OptimizeCWdata(source,sectorID,RoI,CW,file);
    //Draw
    CWoptimizer->DrawCWdata("./../pdf/Draw_" + PdfLabel + ".pdf");

    fout->Write();
    //Delete Histgram
    CWoptimizer->DeleteHist();
}

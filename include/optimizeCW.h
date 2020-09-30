#ifndef MAKECWDATA_hh
#define MAKECWDATA_hh

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <fstream>

class TFile;
class TH2I;
class TH2F;
class TH1D;
class TString;

class optimizeCW{

 public:

  void OptimizeCWdata(std::string source,int sectorID,int RoI,int CW,TFile* file);

  void HitMapCleaner(TH2I *hs);
  void HitMapCleanerStage2(TH2F *hs);

  void MakeHistgram();

  void DeleteHist();

  void DrawCWdata(TString pdf,int sec,int roi,int cw);

  void FillHitMap(TH2I *LowMap,TH2I *HighMap,TH2F *h_Low,TH2F *h_High,int sec,int roi,int cw);

 private:

  TH2F *h_RenewMap[6][148][35];
  //TH2F *h_RenewMap[6][148][35];

};

#endif

#ifndef MCGENERATOR_HPP
#define MCGENERATOR_HPP

#include <TF1.h>
#include <TH1F.h>
#include <TMath.h>
#include <TROOT.h>
#include <TRandom3.h>
#include <TStyle.h>

#include <iostream>
#include <vector>

class MCgenerator {
 private:
  double k;
  double teta;
  double b;

  // intervallo di generazione
  double x_min;
  double x_max;

  int N;  // numero estrazioni
  int Bins;

  TF1* function;  // puntatore??
  TH1D* h;

 public:
  // costruttore
  MCgenerator(double k_val = 5.2, double teta_val = 1.8, double b_val = 0.2,
              double x_min_val = 0, double x_max_val = 0.5 * TMath::Pi(),
              int N_val = 10000, int Bins_val = 200);

  TF1* GetFunction() const;
  void CreateHistogram(const char* name = "h", const char* title = "myHisto");
  TH1D* GetHistogram() const;
  TH1D* Fillh();
  void DrawFunction() const;
};

#endif
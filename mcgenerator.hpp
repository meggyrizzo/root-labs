#ifndef MCGENERATOR_HPP
#define MCGENERATOR_HPP

#include <TF1.h>
#include <TGraph.h>        // Per TGraph (senza errori)
#include <TGraphErrors.h>  // Per TGraphErrors (con barre di errore)
#include <TH1D.h>
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

// incertezze sui parametri (Punto 4)
    double dk;
    double dteta;
    double db;

  // intervallo di generazione
  double x_min;
  double x_max;

  int N;     // numero estrazioni
  int Bins;  // numero bin

  TF1* function;  // puntatore ad una funzione
  TH1D* h;        // puntatore ad histo

 public:
  // costruttore, tutti i valori sono fissati tranne il numero di estrazioni e
  // il numero di bin
  MCgenerator(int N_val, int Bins_val, double k_val = 5.2,
              double teta_val = 1.8, double b_val = 0.2, double x_min_val = 0,
              double x_max_val = 0.5 * TMath::Pi());

  ~MCgenerator();            // distruttore

  
  TF1* GetFunction() const;  // per accedere alla funione private da fuori,
                             // forse è superfluo...
  void CreateHistogram(const char* name = "h", const char* title = "myHisto");
  TH1D* GetHistogram() const;  // per accedere all'istogramma
  TH1D* Fillh();               // riempe istogramma
  double GetRMSD()
      const;  // ritorna la radice quadrata della media degli scarti al quadrato

  // pto 3.2
  TGraphErrors* GraphMeanWithError(int N_replicas);

  // pto 3.3
  TGraphErrors* GraphBinSmeering(int N_replicas);

  // pto 4(3.2)
  TGraphErrors* GraphParamUncertainty_32(int N_replicas);

  // pto 4(3.3)
  TGraphErrors* GraphParamUncertainty_33(int N_replicas);

  // pto 1-> disegna funzione ed istogramma a confronto in un unico file
  void DrawFunction(const char* filename) const;
};

#endif
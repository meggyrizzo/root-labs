#ifndef MCGENERATOR_HPP
#define MCGENERATOR_HPP

#include <TF1.h>
#include <TGraph.h>        // Per TGraph (senza errori)
#include <TGraphErrors.h>  // Per TGraphErrors (con barre di errore)
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

  TF1* function;  // puntatore ad una funzione
  TH1D* h;        // histo

 public:
  // costruttore, tutti i valori sono fissati tranne il numero di estrazioni e
  // il numero di bin
  MCgenerator(int N_val, int Bins_val, double k_val = 5.2,
              double teta_val = 1.8, double b_val = 0.2, double x_min_val = 0,
              double x_max_val = 0.5 * TMath::Pi());

  TF1* GetFunction() const;  // per accedere alla funione private da fuori,
                             // forse è superfluo...
  void CreateHistogram(const char* name = "h", const char* title = "myHisto");
  TH1D* GetHistogram() const;  // per accedere all'istogramma
  TH1D* Fillh();               // riempe istogramma
  double GetRMSD()
      const;  // ritorna la radice quadrata della media degli scarti al quadrato
  TGraphErrors* GraphMediaConErrore(
      int N_replicas);  // esegue una simulazione mc per un numero elevato di
                        // volte (N_replicas) e calcolare la media e
                        // l'incertezza statistica dei conteggi in ciascun bin
                        // dell'istogramma-> praticamente è la dev standard
                        // della distribuzione delle medie campionarie dei
                        // conteggi per bin
  void DrawFunction(const char* filename) const;
};

#endif
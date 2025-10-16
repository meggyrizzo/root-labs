#include "mcgenerator.hpp"

#include <TCanvas.h>

#include <iostream>
#include <vector>

MCgenerator::MCgenerator(double k_val, double teta_val, double b_val,
                         double x_min_val, double x_max_val)
    : k(k_val),
      teta(teta_val),
      b(b_val),
      x_min(x_min_val),
      x_max(x_max_val),
      N(10000),
      Bins(100) {
  function =
      new TF1("myFunc", "TMath::Power(TMath::Cos([0] * x + [1]), 2) + [2]",
              x_min, x_max);
  function->SetParameter(0, k);
  function->SetParameter(1, teta);
  function->SetParameter(2, b);
}

TF1* MCgenerator::GetFunction() const { return function; }

void MCgenerator::CreateHistogram(const char* name, const char* title) {
  h = new TH1D(name, title, 100, x_min, x_max);
  h->SetLineColor(kBlue);
}

TH1D* MCgenerator::GetHistogram() const { return h; }

TH1D* MCgenerator::Fillh() {
  TRandom3 rnd(0);
  int n_accept = 0;
  while (n_accept < N) {
    double x = rnd.Uniform(x_min, x_max);
    double y = rnd.Uniform(0, 1 + b);
    if (y < function->Eval(x)) {
      h->Fill(x);
      n_accept++;
    }
  }
  return h;
}

void MCgenerator::DrawFunction() const {
  h->Scale(1.0 / h->Integral("width"));  // normalizza l'istogramma

  TF1 f_norm("myFunc", "TMath::Power(TMath::Cos([0] * x + [1]), 2) + [2]",
             x_min, x_max);
  f_norm.SetParameter(0, k);
  f_norm.SetParameter(1, teta);
  f_norm.SetParameter(2, b);
  f_norm.SetLineColor(kRed);

  // Normalizza la funzione alla stessa area dell'istogramma
  double integral_f = f_norm.Integral(x_min, x_max);
  f_norm.SetParameter(0, f_norm.GetParameter(0) / integral_f);

  TCanvas* canvas = new TCanvas("canvas", "Funzione vs Istogramma", 800, 600);
  h->Draw("HISTO");
  f_norm.Draw("SAME");
  canvas->Update();
  canvas->SaveAs("funzione.png");
}
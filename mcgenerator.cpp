#include "mcgenerator.hpp"

#include <TCanvas.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TRandom.h>
#include <TRandom3.h>
#include <TStyle.h>

#include <iostream>
#include <vector>

#include "functions.hpp"

MCgenerator::MCgenerator(int N_val, int Bins_val, double k_val, double teta_val,
                         double b_val, double x_min_val, double x_max_val)
    : N(N_val),
      Bins(Bins_val),
      k(k_val),
      teta(teta_val),
      b(b_val),
      x_min(x_min_val),
      x_max(x_max_val),
      h(nullptr) {
  function =
      new TF1("myFunc", "TMath::Power(TMath::Cos([0] * x + [1]), 2) + [2]",
              x_min, x_max);
  function->SetParameters(k, teta, b);

  // imposto inceretzze sistematiche
  dk = 0.02 * k;
  dteta = 0.05 * teta;
  db = 0.01 * b;

  if (!gRandom) {
    gRandom = new TRandom3(0);
  }
}

// distruttore
MCgenerator::~MCgenerator() {
  delete function;
  delete h;  // sicuro grazie a SetDirectory(0) in CreateHistogram
}

TF1* MCgenerator::GetFunction() const { return function; }

void MCgenerator::CreateHistogram(const char* name, const char* title) {
  if (h) {
    delete h;  // cancella l'istogramma precedente se esiste
    h = nullptr;
  }

  // creo un nuovo istogramma
  h = new TH1D(name, title, Bins, x_min, x_max);
  h->SetLineColor(kBlue);
  h->SetDirectory(0);  // impedisce a gDirectory di appropiarsene
}

TH1D* MCgenerator::GetHistogram() const { return h; }

TH1D* MCgenerator::Fillh() {
  int n_accept = 0;
  while (n_accept < N) {
    double x = gRandom->Uniform(x_min, x_max);
    double y = gRandom->Uniform(0, 1 + b);
    if (y < function->Eval(x)) {
      h->Fill(x);
      n_accept++;
    }
  }
  return h;
}

// pto 3.2
TGraphErrors* MCgenerator::GraphMeanWithError(int N_replicas) {
  std::vector<std::vector<double>> bins(Bins);

  for (int r = 0; r < N_replicas; ++r) {
    MCgenerator replica(N, Bins, k, teta, b, x_min, x_max);
    replica.CreateHistogram();
    replica.Fillh();
    TH1D* h_replica = replica.GetHistogram();

    // normalizza l'istogramma di replica
    double integral = h_replica->Integral("width");
    if (integral == 0) integral = 1;
    h_replica->Scale(1.0 / integral);

    for (int i = 0; i < Bins; ++i)
      bins[i].push_back(h_replica->GetBinContent(i + 1));
  }

  return CreateGraph(bins, x_min, x_max, Bins,
                     "Incertezza da Rigenerazione (3.2); x; PDF media", 20,
                     kBlue, "file/output_graph_32");
}

// pto 3.3
TGraphErrors* MCgenerator::GraphBinSmeering(int N_replicas) {
  std::vector<std::vector<double>> bins_smeared(Bins);
  double bin_width = (x_max - x_min) / Bins;
  // usiamo la funzione già normalizzata per il valore teorico
  TF1* f_norm = GetNormalizedFunction(k, teta, b, x_min, x_max);
  for (int r = 0; r < N_replicas; ++r) {
    for (int i = 0; i < Bins; ++i) {
      double x = x_min + (i + 0.5) * bin_width;
      double val = f_norm->Eval(x);  // valore teorico

      // calcolo incertezza statistica attesa per la pdf
      double sigma_stat_pdf = 0.0;
      if (val > 0) {  // evita la divisione per zero o sqrt di negativi
        sigma_stat_pdf = std::sqrt(
            val / (N * bin_width));  // l'inceretzza statistica è la dev std
      }

      // fluttua il valore teorico (val) con la sua incertezza statistica
      // (sigma)
      double smeared = gRandom->Gaus(val, sigma_stat_pdf);
      bins_smeared[i].push_back(smeared);
    }
  }

  delete f_norm;  // pulizia (allocazione dinamica nel metodo
                  // GetNormalizedFunction())

  return CreateGraph(
      bins_smeared, x_min, x_max, Bins,
      "Bin-smeering: fluttuazione teorica (3.3); x; Conteggio medio", 20,
      kGreen, "file/output_graph_33");
}

// pto 4 (metodo 3.2)
TGraphErrors* MCgenerator::GraphParamUncertainty_32(int N_replicas) {
  std::vector<std::vector<double>> bins(Bins);

  for (int r = 0; r < N_replicas; ++r) {
    // fluttua i parametri
    double k_smeared = gRandom->Gaus(k, dk);
    double teta_smeared = gRandom->Gaus(teta, dteta);
    double b_smeared = gRandom->Gaus(b, db);

    // crea replica della funzione con parametri fluttuati
    MCgenerator replica2(N, Bins, k_smeared, teta_smeared, b_smeared, x_min,
                         x_max);
    replica2.CreateHistogram();
    replica2.Fillh();
    TH1D* h_replica2 = replica2.GetHistogram();

    // store results (normalized)
    double integral = h_replica2->Integral("width");
    if (integral == 0) integral = 1;
    h_replica2->Scale(1.0 / integral);

    for (int i = 0; i < Bins; ++i) {
      bins[i].push_back(h_replica2->GetBinContent(i + 1));
    }
  }
  return CreateGraph(bins, x_min, x_max, Bins,
                     "Prop. Parametri (Rigenerazione, 4a); x; PDF", 20, kRed,
                     "file/output_graph_uncertainty_32");
}

TGraphErrors* MCgenerator::GraphParamUncertainty_33(int N_replicas) {
  std::vector<std::vector<double>> bins_smeared(Bins);

  for (int r = 0; r < N_replicas; ++r) {
    // faccio fluttuare i parametri con gaussiana
    double k_smeared = gRandom->Gaus(k, dk);
    double teta_smeared = gRandom->Gaus(teta, dteta);
    double b_smeared = gRandom->Gaus(b, db);

    // crea funzione temporanea con parametri "che fluttuano"
    TF1* temp_func =
        GetNormalizedFunction(k_smeared, teta_smeared, b_smeared, x_min, x_max);

    // calcola valori fluttuati
    for (int i = 0; i < Bins; ++i) {
      double x_val = x_min + (i + 0.5) * (x_max - x_min) / Bins;
      double val = temp_func->Eval(x_val);  // valore al centro del bin
      bins_smeared[i].push_back(val);  // vettore con valori al centro del bin
    }
    delete temp_func;
  }
  return CreateGraph(bins_smeared, x_min, x_max, Bins,
                     "Prop. Parametri (Smeering, 4b); x; PDF", 20, kMagenta,
                     "file/output_graph_uncertainty_33");
}

void MCgenerator::DrawFunction(const char* filename) const {
  TH1D* h_norm = (TH1D*)h->Clone("h_norm_draw");
  h_norm->Scale(1.0 / h_norm->Integral("width"));  // normalizza l'istogramma
  h_norm->SetTitle("Funzione (norm) vs Istogramma (norm)");
  h_norm->GetYaxis()->SetTitle("PDF");

  TF1* f_scaled = GetNormalizedFunction(
      k, teta, b, x_min, x_max);  // Ottieni funzione normalizzata
  f_scaled->SetLineColor(kRed);

  TCanvas* canvas = new TCanvas("canvas", "Funzione vs Istogramma", 800, 600);
  h_norm->Draw("HISTO");
  f_scaled->Draw("SAME");

  canvas->Update();
  canvas->SaveAs(filename);

  delete canvas;
  delete f_scaled;  // Pulizia
  delete h_norm;    // Pulizia
}

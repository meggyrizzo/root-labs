#include "mcgenerator.hpp"

#include <TCanvas.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TRandom3.h>
#include <TStyle.h>  // per gRandom

#include <iostream>
#include <string>
#include <vector>

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

TF1* MCgenerator::GetNormalizedFunction() const {
  // crea una NUOVA funzione, il chiamante deve gestirne la delete
  TF1* f_norm =
      new TF1("f_norm_temp", "TMath::Power(TMath::Cos([0] * x + [1]), 2) + [2]",
              x_min, x_max);
  f_norm->SetParameters(k, teta, b);

  double integral = f_norm->Integral(x_min, x_max);
  if (integral > 0) {
    // per normalizzare la PDF, dobbiamo scalare per 1/integrale
    // TF1 non lo fa facilmente->creiamo una funzione wrapper.
    delete f_norm;  // cancella la precedente

    f_norm = new TF1("f_norm_pdf",
                     "[0]*(TMath::Power(TMath::Cos([1]*x + [2]), 2) + [3])",
                     x_min, x_max);
    f_norm->SetParameter(0, 1.0 / integral);  // Fattore di scala
    f_norm->SetParameter(1, k);
    f_norm->SetParameter(2, teta);
    f_norm->SetParameter(3, b);
  }
  f_norm->SetLineColor(kRed);
  return f_norm;
}

double MCgenerator::GetRMSD() const {
  double sum = 0.0;
  int n_bins = h->GetNbinsX();

  // normalizziamo la funzione per il confronto
  TF1* f_norm = GetNormalizedFunction();

  // normalizziamo l'istogramma
  TH1D* h_norm = (TH1D*)h->Clone("h_norm_temp");
  h_norm->Scale(1.0 / h_norm->Integral("width"));

  for (int i = 1; i <= n_bins; ++i) {
    double x = h_norm->GetBinCenter(i);
    double h_val = h_norm->GetBinContent(i);
    double f_val = f_norm->Eval(x);
    double diff = h_val - f_val;
    sum += diff * diff;
  }

  delete f_norm;
  delete h_norm;

  return std::sqrt(sum / n_bins);
}

TGraphErrors* MCgenerator::GraphMeanWithError(int N_replicas) {
  std::vector<std::vector<double>> bins(Bins);

  for (int r = 0; r < N_replicas; ++r) {
    MCgenerator replica(N, Bins, k, teta, b, x_min, x_max);
    replica.CreateHistogram();
    replica.Fillh();
    TH1D* h_replica = replica.GetHistogram();

    // normalizza l'istogramma di replica a PDF
    double integral = h_replica->Integral("width");
    if (integral == 0) integral = 1;
    h_replica->Scale(1.0 / integral);

    for (int i = 0; i < Bins; ++i)
      bins[i].push_back(h_replica->GetBinContent(i + 1));
  }

  std::vector<double> x(Bins), y(Bins), ex(Bins, 0), ey(Bins);
  double bin_width = (x_max - x_min) / Bins;

  for (int i = 0; i < Bins; ++i) {
    double sum = 0, sum2 = 0;
    for (double v : bins[i]) {
      sum += v;
      sum2 += v * v;
    }

    double mean = sum / N_replicas;
    double stddev = std::sqrt(sum2 / N_replicas - mean * mean);

    x[i] = h->GetBinCenter(i + 1);
    y[i] = mean;
    ey[i] = stddev;
  }

  // crea grafico
  auto* graph = new TGraphErrors(Bins, &x[0], &y[0], &ex[0], &ey[0]);
  graph->SetTitle("Incertezza da Rigenerazione (3.2); x; PDF media");
  graph->SetMarkerStyle(20);
  graph->SetLineColor(kBlue + 2);

  return graph;
}

TGraphErrors* MCgenerator::GraphBinSmeering(int N_replicas,
                                            double smear_fraction) {
  std::vector<std::vector<double>> bins(Bins);
  double bin_width = (x_max - x_min) / Bins;

  // usiamo la funzione già normalizzata per il valore teorico
  TF1* f_norm = GetNormalizedFunction();

  for (int r = 0; r < N_replicas; ++r) {
    for (int i = 0; i < Bins; ++i) {
      double x = x_min + (i + 0.5) * bin_width;
      double val = f_norm->Eval(x);
      double smeared = gRandom->Gaus(val, smear_fraction * val);
      bins[i].push_back(smeared);
    }
  }

  delete f_norm;  // pulizia (allocazione dinamica nel metodo
                  // GetNormalizedFunction())

  std::vector<double> x(Bins), y(Bins), ex(Bins, 0), ey(Bins);

  for (int i = 0; i < Bins; ++i) {
    double sum = 0, sum2 = 0;
    for (double v : bins[i]) {
      sum += v;
      sum2 += v * v;
    }
    double mean = sum / N_replicas;
    double stddev = std::sqrt(sum2 / N_replicas - mean * mean);

    x[i] = x_min + (i + 0.5) * bin_width;
    y[i] = mean;
    ey[i] = stddev;
  }

  auto* graph = new TGraphErrors(Bins, &x[0], &y[0], &ex[0], &ey[0]);
  graph->SetTitle(
      "Bin-smeering: fluttuazione teorica (3.3); x; Conteggio medio");
  graph->SetMarkerStyle(21);
  graph->SetLineColor(kGreen + 2);

  return graph;
}

// pto 4 (metodo 3.2)
TGraphErrors* MCgenerator::GraphParamUncertainty_32(int N_replicas) {
  std::vector<std::vector<double>> bins(Bins);
  TRandom3 rnd(0);

  double dk = 0.02 * k;
  double dteta = 0.05 * teta;
  double db = 0.01 * b;

  for (int r = 0; r < N_replicas; ++r) {
    // fluttua i parametri
    double k_smeared = rnd.Gaus(k, dk);
    double teta_smeared = rnd.Gaus(teta, dteta);
    double b_smeared = rnd.Gaus(b, db);

    // crea replica con parametri fluttuati
    MCgenerator replica(N, Bins, k_smeared, teta_smeared, b_smeared, x_min,
                        x_max);
    replica.CreateHistogram();
    replica.Fillh();
    TH1D* h_replica = replica.GetHistogram();

    // store results (normalized)
    double integral = h_replica->Integral("width");
    if (integral == 0) integral = 1;
    h_replica->Scale(1.0 / integral);

    for (int i = 0; i < Bins; ++i) {
      bins[i].push_back(h_replica->GetBinContent(i + 1));
    }
  }

  // calcola media e stddev
  std::vector<double> x(Bins), y(Bins), ex(Bins, 0), ey(Bins);
  double bin_width = (x_max - x_min) / Bins;

  for (int i = 0; i < Bins; ++i) {
    double sum = 0, sum2 = 0;
    for (double v : bins[i]) {
      sum += v;
      sum2 += v * v;
    }
    double mean = sum / N_replicas;
    double stddev = std::sqrt(sum2 / N_replicas - mean * mean);

    x[i] = x_min + (i + 0.5) * bin_width;
    y[i] = mean;
    ey[i] = stddev;  // incertezza propagata
  }

  auto* graph = new TGraphErrors(Bins, &x[0], &y[0], &ex[0], &ey[0]);
  graph->SetTitle("Prop. Parametri (Rigenerazione, 4a); x; PDF");
  graph->SetMarkerStyle(20);
  graph->SetLineColor(kRed + 2);

  return graph;
}

TGraphErrors* MCgenerator::GraphParamUncertainty_33(int N_replicas) {
  std::vector<std::vector<double>> bins(Bins);
  TRandom3 rnd(0);

  double dk = 0.02 * k;
  double dteta = 0.05 * teta;
  double db = 0.01 * b;

  for (int r = 0; r < N_replicas; ++r) {
    // fluttua i parametri
    double k_smeared = rnd.Gaus(k, dk);
    double teta_smeared = rnd.Gaus(teta, dteta);
    double b_smeared = rnd.Gaus(b, db);

    // crea funzione temporanea
    TF1 temp_func("temp_func",
                  "TMath::Power(TMath::Cos([0] * x + [1]), 2) + [2]", x_min,
                  x_max);
    temp_func.SetParameters(k_smeared, teta_smeared, b_smeared);

    // normalizza la funzione
    double integral = temp_func.Integral(x_min, x_max);
    if (integral == 0) integral = 1;
    double scale = 1.0 / integral;

    // calcola valori fluttuati
    for (int i = 0; i < Bins; ++i) {
      double x_val = x_min + (i + 0.5) * (x_max - x_min) / Bins;
      double val = temp_func.Eval(x_val);
      bins[i].push_back(val * scale);  // store normalized value
    }
  }

  // calcola media e stddev
  std::vector<double> x(Bins), y(Bins), ex(Bins, 0), ey(Bins);
  double bin_width = (x_max - x_min) / Bins;

  for (int i = 0; i < Bins; ++i) {
    double sum = 0, sum2 = 0;
    for (double v : bins[i]) {
      sum += v;
      sum2 += v * v;
    }
    double mean = sum / N_replicas;
    double stddev = std::sqrt(sum2 / N_replicas - mean * mean);

    x[i] = x_min + (i + 0.5) * bin_width;
    y[i] = mean;
    ey[i] = stddev;  // incertezza propagata
  }

  auto* graph = new TGraphErrors(Bins, &x[0], &y[0], &ex[0], &ey[0]);
  graph->SetTitle("Prop. Parametri (Smeering, 4b); x; PDF");
  graph->SetMarkerStyle(21);
  graph->SetLineColor(kMagenta + 2);

  return graph;
}

void MCgenerator::DrawFunction(const char* filename) const {
  TH1D* h_norm = (TH1D*)h->Clone("h_norm_draw");
  h_norm->Scale(1.0 / h_norm->Integral("width"));  // normalizza l'istogramma
  h_norm->SetTitle("Funzione (norm) vs Istogramma (norm)");
  h_norm->GetYaxis()->SetTitle("PDF");

  TF1* f_scaled = GetNormalizedFunction();  // Ottieni funzione normalizzata
  f_scaled->SetLineColor(kRed);

  TCanvas* canvas = new TCanvas("canvas", "Funzione vs Istogramma", 800, 600);
  h_norm->Draw("HISTO");
  f_scaled->Draw("SAME");
  TLegend* leg = new TLegend(0.6, 0.8, 0.9, 0.9);
  leg->AddEntry(h_norm, "Istogramma MC (norm)", "l");
  leg->AddEntry(f_scaled, "Funzione Teorica (norm)", "l");
  leg->Draw();

  canvas->Update();
  canvas->SaveAs(filename);

  delete canvas;
  delete f_scaled;  // Pulizia
  delete h_norm;    // Pulizia
  delete leg;
}
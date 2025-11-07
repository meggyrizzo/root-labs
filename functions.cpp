#include "functions.hpp"

#include <fstream>
#include <iomanip>

#include "mcgenerator.hpp"

TGraphErrors* CreateGraph(const std::vector<std::vector<double>>& bin_values,
                          double x_min, double x_max, int Bins,
                          const char* title, int marker_style, int line_color,
                          const char* output_filename) {
  std::vector<double> x(Bins), y(Bins), ex(Bins, 0), ey(Bins);
  double bin_width = (x_max - x_min) / Bins;

  for (int i = 0; i < Bins; ++i) {
    double sum = 0, sum2 = 0;
    for (double v : bin_values[i]) {
      sum += v;
      sum2 += v * v;
    }
    double mean = sum / bin_values[i].size();
    double stddev = std::sqrt(sum2 / bin_values[i].size() - mean * mean);

    x[i] = x_min + (i + 0.5) * bin_width;
    y[i] = mean;
    ey[i] = stddev;

    if (output_filename) {
      std::ofstream out(output_filename);
      out << "# Bin\tMedia\tDev_Std\n";
      for (int i = 0; i < Bins; ++i) {
        out << i << "\t" << std::fixed << std::setprecision(6) << y[i] << "\t"
            << ey[i] << "\n";
      }
      out.close();
    }
  }

  auto* graph = new TGraphErrors(Bins, &x[0], &y[0], &ex[0], &ey[0]);
  graph->SetTitle(title);
  graph->SetMarkerStyle(marker_style);
  graph->SetLineColor(line_color);
  return graph;
}

// per convertire un oggetto TGraphErrors in un istogramam
TH1D* ConvertGraphToHistogram(TGraphErrors* graph, int Bins, double x_min,
                              double x_max, const char* title) {
  TH1D* histo_from_graph = new TH1D("h_from_graph", title, Bins, x_min, x_max);
  double bin_width = (x_max - x_min) / Bins;  // calcolo larghezza bin

  for (int i = 0; i < graph->GetN();  // ciclo su tutti i pti del TGraphErrors
       ++i) {
    double x = graph->GetX()[i];  // estrae coordinata x del pto i-esimo
    double y = graph->GetY()[i];  // estrae coordinata y del pto i-esimo
    int bin = histo_from_graph->FindBin(
        x);  // trova bin istogramma corrispondente alla posizione x
    histo_from_graph->SetBinContent(
        bin, y);  // imposta altezza al valore y corrispondente
  }
  return histo_from_graph;
}

// funzione x normalizzare
TF1* GetNormalizedFunction(double k, double teta, double b, double x_min,
                           double x_max) {
  // crea una NUOVA funzione non normalizzata
  TF1* f_norm =
      new TF1("f_norm_temp", "TMath::Power(TMath::Cos([0] * x + [1]), 2) + [2]",
              x_min, x_max);
  f_norm->SetParameters(k, teta, b);
  double integral = f_norm->Integral(x_min, x_max);
  if (integral > 0) {
    // se l'integrale è valido, crea la funzione wrapper normalizzata
    delete f_norm;
    // nuova funzione con parametro [0] come fattore di scala
    f_norm = new TF1("f_norm_pdf",
                     "[0]*(TMath::Power(TMath::Cos([1]*x + [2]), 2) + [3])",
                     x_min, x_max);
    f_norm->SetParameter(0,
                         1.0 / integral);  // Fattore di scala (normalizzazione)
    f_norm->SetParameter(1, k);
    f_norm->SetParameter(2, teta);
    f_norm->SetParameter(3, b);
  }

  f_norm->SetLineColor(kRed);
  return f_norm;
}

/*double GetRMSD() const {
  double sum = 0.0;
  int n_bins = GetHistogram()->GetNbinsX();

  // normalizziamo la funzione per il confronto
  TF1* f_norm = GetNormalizedFunction(k, teta, b, x_min, x_max);
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
}*/

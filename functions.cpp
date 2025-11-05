#include "functions.hpp"

#include "mcgenerator.hpp"

/*Stats CalculateStats(int Bins, const std::vector<double>& bin_data, size_t n_replicas) {
  // calcola media e stddev
  std::vector<double> x(Bins), y(Bins), ex(Bins, 0), ey(Bins);
  double bin_width = (0.5 * TMath::Pi()) / Bins;

  for (int i = 0; i < Bins; ++i) {
    double sum = 0, sum2 = 0;
    for (double v : bin_data) {
      sum += v;  // somma dei valori nelle varie generazioni nel bin i
      sum2 += v * v;
    }
    double mean = sum / n_replicas;
    double stddev = std::sqrt(sum2 / n_replicas - mean * mean);

    x[i] = (i + 0.5) * bin_width;
    y[i] = mean;
    ey[i] = stddev;  // incertezza propagata
  }

return Stats;
}*/
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

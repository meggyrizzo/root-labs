#include "fit_analyzer.hpp"

#include <TCanvas.h>

#include <cmath>
#include <iostream>

#include "functions.hpp"
#include "mcgenerator.hpp"

FitAnalyzer::FitAnalyzer(MCgenerator& generation)
    : mcgen(generation), fit_function(nullptr) {}

FitAnalyzer::~FitAnalyzer() {
  delete fit_function;
}  // libera la memoria se è stato creato un fit

void FitAnalyzer::FitFixedParametersFromGraph(TGraphErrors* graph,
                                              const char* title) {
  // converto il TGraphErrors in un istogramma
  TH1D* h = ConvertGraphToHistogram(graph, mcgen.Bins, mcgen.x_min, mcgen.x_max,
                                    title);
  // creo la funzione di fit con i parametri originali gia normalizzata
  fit_function = GetNormalizedFunction(mcgen.k, mcgen.teta, mcgen.b,
                                       mcgen.x_min, mcgen.x_max);

  fit_function->SetLineColor(kBlack);

  // eseguo il fit sull'istogramma ottenuto dal TGraphErrors
  h->Fit(fit_function, "R");

  // calcolo residuo
  TH1D* h_residuals = new TH1D("h_residuals", "Residui: h_i - f(x_i)",
                               mcgen.Bins, mcgen.x_min, mcgen.x_max);
  for (int i = 1; i <= mcgen.Bins; ++i) {
    double x = h->GetBinCenter(i);
    double h_i = h->GetBinContent(i);
    double f_xi = fit_function->Eval(x);
    h_residuals->SetBinContent(i, h_i - f_xi);
  }
  h_residuals->SetLineColor(kRed);
  h_residuals->GetYaxis()->SetTitle("Residue");

  TCanvas* c = new TCanvas("c_fit_graph", "Fit e Residues", 800, 800);
  c->Divide(1, 2);

  c->cd(1);  // pad 1: fit
  h->Draw("HIST");
  fit_function->Draw("SAME");
  c->cd(2);  // pad 2: residui
  h_residuals->Draw("HIST");
  c->SaveAs("graphs/fit_from_graph.png");

  delete c;
  delete h;
  delete h_residuals;
}

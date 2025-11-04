#include <TCanvas.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TLegend.h>

#include "mcgenerator.hpp"

int main() {
  // prova 1
  MCgenerator main_generation(10000, 50);
  main_generation.GetFunction();
  main_generation.CreateHistogram("h", "histo MonteCarlo");
  main_generation.Fillh();
  main_generation.DrawFunction("graphs/function1.png");

  TGraphErrors* graph = main_generation.GraphMeanWithError(50);
  graph->SetMarkerColor(kBlue);

  TCanvas* c = new TCanvas("c_media", "Media con errore", 800, 600);
  graph->Draw("AP");
  c->SaveAs("graphs/uncertainty_rigeneration_histo.png");

  MCgenerator smeared(0, 100);  // N_entries=0 perché non serve generare eventi
  TGraphErrors* smear_graph = smeared.GraphBinSmeering(50);  // 100 repliche
  smear_graph->SetMarkerColor(kGreen + 2);

  TCanvas* c_smear = new TCanvas("c_smear", "Bin-smeering", 800, 600);
  smear_graph->Draw("AP");
  c_smear->SaveAs("graphs/bin_smeering.png");

  // pto 4 (3.2)
  TGraphErrors* g_param_regen = main_generation.GraphParamUncertainty_32(50);
  g_param_regen->SetMarkerColor(kRed + 2);
  g_param_regen->SetLineColor(kRed + 2);

  TCanvas* c_param_regen = new TCanvas("c_regen32", "Bin-smeering", 800, 600);
  g_param_regen->Draw("AP");
  c_param_regen->SaveAs("graphs/uncertainty_regen_param.png");

  // pto 4 (3.3)
  TGraphErrors* g_param_smear = main_generation.GraphParamUncertainty_33(50);
  g_param_smear->SetMarkerColor(kMagenta + 2);
  g_param_smear->SetLineColor(kMagenta + 2);

  TCanvas* c_param_smear = new TCanvas("c_smear33", "Bin-smeering", 800, 600);
  g_param_smear->Draw("AP");
  c_param_smear->SaveAs("graphs/uncertainty_smear_param.png");

  // prova 2
  MCgenerator generate2(5000, 300);
  generate2.GetFunction();
  generate2.CreateHistogram("h", "histo MonteCarlo");
  generate2.Fillh();
  generate2.DrawFunction("graphs/function2.png");

  // prova 3, il migliore, piu vicino alla funzione, è l'histo con + entrate e
  // meno bin
  MCgenerator generate3(100000, 100);
  generate3.GetFunction();
  generate3.CreateHistogram("h", "histo MonteCarlo");
  generate3.Fillh();
  generate3.DrawFunction("graphs/function3.png");

  delete c;
  delete graph;
  delete c_smear;
  delete smear_graph;
  delete c_param_regen;
  delete g_param_regen;
  delete c_param_smear;
  delete g_param_smear;

  return 0;
}

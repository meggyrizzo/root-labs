#include <TCanvas.h>

#include "mcgenerator.hpp"

int main() {
  // prova 1
  MCgenerator generate1(10000, 200);
  generate1.GetFunction();
  generate1.CreateHistogram("h", "histo MonteCarlo");
  generate1.Fillh();
  generate1.DrawFunction("function1.png");

  TGraphErrors* graph = generate1.GraphMeanWithError(100);  // 100 repliche
  TCanvas* c = new TCanvas("c_media", "Media con errore", 800, 600);
  graph->Draw("AP");
  c->SaveAs("incertezza_rigenerazion_histo.png");

  // prova 2
  MCgenerator generate2(5000, 300);
  generate2.GetFunction();
  generate2.CreateHistogram("h", "histo MonteCarlo");
  generate2.Fillh();
  generate2.DrawFunction("function2.png");

  // prova 3, il migliore, piu vicino alla funzione, è l'histo con + entrate e
  // meno bin
  MCgenerator generate3(100000, 100);
  generate3.GetFunction();
  generate3.CreateHistogram("h", "histo MonteCarlo");
  generate3.Fillh();
  generate3.DrawFunction("function3.png");

  MCgenerator smeared(0, 100);  // N=0 perché non serve generare eventi
  TGraphErrors* smear_graph = smeared.GraphBinSmeering(100);  // 100 repliche
  TCanvas* c_smear = new TCanvas("c_smear", "Bin-smeering", 800, 600);
  smear_graph->Draw("AP");
  c_smear->SaveAs("bin_smeering.png");

  return 0;
}

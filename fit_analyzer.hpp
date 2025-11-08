#ifndef FIT_ANALYZER_HPP
#define FIT_ANALYZER_HPP

#include <TF1.h>
#include <TGraphErrors.h>
#include <TH1D.h>

#include <vector>

#include "mcgenerator.hpp"

/* classe per analizzare i risultati di MCgenerator,
   esegue il fit a parametri
   fissi e liberi sul grafico 3.2
   e calcola i residui per valutare la bontà
   dell'accordo */

class FitAnalyzer {
 private:
  MCgenerator& mcgen;  // riferimento all'oggetto specifico da analizzare (sarà
                       // questo credo: TGraphErrors* graph =
                       // main_generation.GraphMeanWithError(50); che è generato
                       // nella main generation del main)
  TF1* fit_function;   // puntatore alla funzione di fit da riutilizzare

 public:
  FitAnalyzer(MCgenerator& generation);  // costruttore
  ~FitAnalyzer();                        // distruttore
  void FitFixedParametersFromGraph(TGraphErrors* graph, const char* title);
  void FitWithOneParameterVaried(const char* param_name, TGraphErrors* graph,
                                 const char* title);
};

#endif
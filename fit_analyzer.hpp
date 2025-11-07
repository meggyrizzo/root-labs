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
   dell'accordo*/

class FitAnalyzer {
 public:
  FitAnalyzer();  // costruttore
};

#endif
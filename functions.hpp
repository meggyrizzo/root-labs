#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <TF1.h>
#include <TH1D.h>
#include <TMath.h>

#include <string>
#include <vector>

#include "TGraphErrors.h"

TGraphErrors* CreateGraph(const std::vector<std::vector<double>>& bin_values,
                          double x_min, double x_max, int Bins,
                          const char* title, int marker_style, int line_color,
                          const char* output_filename = nullptr);

TH1D* ConvertGraphToHistogram(TGraphErrors* graph, int Bins, double x_min,
                              double x_max, const char* title);

TF1* GetNormalizedFunction(double k, double teta, double b, double x_min,
                           double x_max);

// double GetRMSD() const;

#endif
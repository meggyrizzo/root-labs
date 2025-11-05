#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <TF1.h>
#include <TH1D.h>
#include <TMath.h>

#include <string>

struct Stats {
  double mean;
  double stddev;
};

//Stats CalculateStats(const std::vector<double>& bin_data, size_t n_replicas);
TF1* GetNormalizedFunction(double k, double teta, double b, double x_min,
                           double x_max);

// double GetRMSD() const;

#endif
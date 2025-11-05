#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <TF1.h>
#include <TMath.h>

#include <string>
TF1* GetNormalizedFunction(double k, double teta, double b, double x_min,
                           double x_max);
double function_mean(int N, int sum);  // non la sto usando x ora

#endif
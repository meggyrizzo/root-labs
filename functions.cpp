#include "functions.hpp"

// funzione x normalizzare
TF1* GetNormalizedFunction(double k, double teta, double b, double x_min, double x_max) {
    // Crea una NUOVA funzione non normalizzata
    TF1* f_norm = new TF1("f_norm_temp", "TMath::Power(TMath::Cos([0] * x + [1]), 2) + [2]", 
                          x_min, x_max);
    f_norm->SetParameters(k, teta, b);

    double integral = f_norm->Integral(x_min, x_max);
    
    if (integral > 0) {
        // Se l'integrale è valido, crea la funzione wrapper normalizzata
        delete f_norm; 

        // Nuova funzione con parametro [0] come fattore di scala
        f_norm = new TF1("f_norm_pdf", 
                         "[0]*(TMath::Power(TMath::Cos([1]*x + [2]), 2) + [3])", 
                         x_min, x_max);
        f_norm->SetParameter(0, 1.0 / integral); // Fattore di scala (normalizzazione)
        f_norm->SetParameter(1, k);
        f_norm->SetParameter(2, teta);
        f_norm->SetParameter(3, b);
    }
    
    f_norm->SetLineColor(kRed);
    return f_norm;
}
// funczione x media
double function_mean (int N, int sum) {
    return sum/N;
}

// funzione x dev std
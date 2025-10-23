#include "mcgenerator.hpp"

int main() {
  MCgenerator generate;
  generate.GetFunction();
  generate.CreateHistogram("h", "histo MonteCarlo");
  generate.Fillh();
  generate.DrawFunction();
  return 0;
}
// ciao

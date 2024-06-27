#include "Common.h"
#include "GammaLawGas.h"
#include "Grid.h"
int main(int argc, char const *argv[])
{
  Material::GammaLawGas<DIM, 1> Gas;
  Grid grid;
  std::string meshname = "D.mesh";
  grid.set_meshfile(meshname);
  return 0;
}

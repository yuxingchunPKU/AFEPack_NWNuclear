#include "Common.h"
#include "Solution.h"
#include "Grid.h"
#include "Problem.h"
int main(int argc, char const *argv[])
{
  Grid grid;
  std::string meshname = "D.mesh";
  grid.set_meshfile(meshname);
  Problem app;
  return 0;
}

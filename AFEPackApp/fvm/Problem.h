#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include "Solution.h"
#include "Grid.h"
class Problem : public Grid
{
private:
  double cfl;
  u_int step;

public:
  void initialize();
  void initial_value();
  void boundary_value();
  void calculate_edge_flux();
};

#endif // _PROBLEM_H_
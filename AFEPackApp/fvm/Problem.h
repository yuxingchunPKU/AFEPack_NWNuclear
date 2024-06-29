#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include "Solution.h"
#include "Grid.h"
class Problem : public Grid
{
private:
  double cfl;          // CFL数
  double dt;           // 时间步
  u_int step;          // 演化步
  fe_space_t DG_sapce; // 间断有限元空间
  Solution u_h;        // 数值解

public:
public:
  void initialize();
  void initialValue();
  void boundaryValue();
  void boundaryFlux();
  void calculateEdgeFlux();
  void run();
  void getTimeStepLength();
};

#endif // _PROBLEM_H_
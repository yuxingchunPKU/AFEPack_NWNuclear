#ifndef _SOLUTION_H_
#define _SOLUTION_H_

#include "Common.h"
#include "GammaLawGas.h"
struct SolutionGas
{
  enum
  {
    dos = DIM + 2
  };
  typedef Material::GammaLawGas<DIM, 1> sol_t;
};

struct Solution : public SolutionGas, virtual public Types
{
  enum
  {
    dos = SolutionGas::dos
  };
  typedef typename SolutionGas::sol_t sol_t;
  // 守恒量数值解向量
  std::vector<fe_fun_t> u_h;
  property_id_t<sol_t> pid_edge_u;

  void new_property()
  {
    new_property_id(pid_edge_u);
  }

  void free_property()
  {
    free_property_id(pid_edge_u);
  }
};

#endif // _SOLUTION_H_
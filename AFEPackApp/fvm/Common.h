#ifndef _COMMON_H_
#define _COMMON_H_

#include "CommonHeader.h"
#include "CommonTypes.h"

struct Types
{
  typedef HGeometryTree<DIM> h_tree_t;
  typedef IrregularMesh<DIM> ir_mesh_t;
  typedef RegularMesh<DIM> mesh_t;
  typedef AFEPack::Point<DIM> point_t;
  typedef DGFEMSpace<double, DIM> fe_space_t;
  typedef Element<double, DIM> element_t;
  typedef DGElement<double, DIM> dg_element_t;
  typedef FEMFunction<double, DIM> fe_fun_t;
  typedef SVector<DIM> velocity_t;
};

#endif // _COMMON_H_
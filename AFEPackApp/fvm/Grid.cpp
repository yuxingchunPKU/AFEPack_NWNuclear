#include "Grid.h"

// 空函数即可 负载平衡会用
void Grid::initialize()
{
}

void Grid::init_mesh()
{
  h_tree.readMesh(meshfile);
  ir_mesh->reinit(h_tree);
  ir_mesh->semiregularize();
  ir_mesh->regularize(false); // 参数对单元序号重排 有限体积不需要
}

void Grid::init_template_element()
{
  triangle_template_geometry.readData("triangle.tmp_geo");
  triangle_coord_transform.readData("triangle.crd_trs");
  triangle_unit_out_normal.readData("triangle.out_nrm");
  triangle_0_template_dof.reinit(triangle_template_geometry);
  triangle_0_template_dof.readData("triangle.0.tmp_dof");
  triangle_0_basis_function.reinit(triangle_0_template_dof);
  triangle_0_basis_function.readData("triangle.0.bas_fun");

  triangle_1_D_template_dof.reinit(triangle_template_geometry);
  triangle_1_D_template_dof.readData("triangle.v1.D.tmp_dof");
  triangle_1_D_basis_function.reinit(triangle_1_D_template_dof);
  triangle_1_D_basis_function.readData("triangle.v1.D.bas_fun");

  triangle_1_template_dof.reinit(triangle_template_geometry);
  triangle_1_template_dof.readData("triangle.1.tmp_dof");
  triangle_1_basis_function.reinit(triangle_1_template_dof);
  triangle_1_basis_function.readData("triangle.1.bas_fun");

  interval_template_geometry.readData("interval.tmp_geo");
  interval_to2d_coord_transform.readData("interval.to2d.crd_trs");

  edge_template_element.resize(1);
  edge_template_element[0].reinit(interval_template_geometry,
                                  interval_to2d_coord_transform);
}
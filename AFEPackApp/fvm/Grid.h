#ifndef _GRID_H_
#define _GRID_H_

#include "Common.h"
class Grid : public Types
{
public:
  std::string meshfile; // 网格文件名
private:
  h_tree_t h_tree;    // 层次几何体
  ir_mesh_t *ir_mesh; // 非正则网格

public:
  Grid(){}; // 不设置默认参数
  ~Grid(){};

public:
  void initialize();
  void init_mesh();
  void init_template_element();
  void set_meshfile(const std::string);

protected:
  std::vector<TemplateElement<double, DIM>> template_element;         // 单元模板向量
  std::vector<TemplateDGElement<DIM - 1, DIM>> edge_template_element; // 边的模板向量
  // 二维信息
  TemplateGeometry<DIM> triangle_template_geometry; /* 模板单元信息 */
  CoordTransform<DIM> triangle_coord_transform;     /* 坐标变换 */
  UnitOutNormal<DIM> triangle_unit_out_normal;      /* 单位外法向量 */

  TemplateDOF<DIM> triangle_0_template_dof;                  /* 自由度 */
  BasisFunctionAdmin<double, DIM> triangle_0_basis_function; /* 基函数 */
  // 线性重构
  TemplateDOF<DIM> triangle_1_template_dof;                    /* 自由度 */
  BasisFunctionAdmin<double, DIM> triangle_1_basis_function;   /* 基函数 */
  TemplateDOF<DIM> triangle_1_D_template_dof;                  /* 自由度 */
  BasisFunctionAdmin<double, DIM> triangle_1_D_basis_function; /* 基函数 */

  /// 表面线段
  TemplateGeometry<DIM - 1> interval_template_geometry;
  CoordTransform<DIM - 1, DIM> interval_to2d_coord_transform;
};

#endif // _GRID_H_
#ifndef _GAMMALAWGAS_H_
#define _GAMMALAWGAS_H_
#include "Common.h"

namespace Material
{
  template <int D>
  struct GammaLawGasBase
  {
    const static char *variable_name[];
  };
  template <int D, u_int DOF>
  struct GammaLawGas : public SVector<D + 2>, public GammaLawGasBase<D>
  {
  public:
    enum
    {
      dim = D,     // 维数
      dos = D + 2, // 解的维数
      dof = DOF    // 原来是分子的自由度，现在废弃不用了
    };
    typedef GammaLawGas<D, DOF> this_t;
    typedef SVector<dos> flux_t;
    static double gamma;              // = (DOF + 2.0)/DOF; /// Gamma常数
    static double default_value[dos]; /**
                                       * 缺省值，参考 Solution.cpp 中做的解释。
                                       */
    double rho;                       /// 密度
    Types::velocity_t u;              /// 速度
    double E;                         /// 能量
    double p;                         /// 压力
    double c;                         /// 声速
    double V2;                        /// 速度模
    double e;                         /// 内能

    GammaLawGas() : SVector<dos>() {} // 构造函数
    const Types::velocity_t &velocity() const
    {
      return u;
    }
    // 最大特征速度
    double signal_speed() const
    {
      return c + sqrt(V2);
    }
    /// 检查密度和压强是否非负
    bool is_feasible() const
    {
      return ((rho > 0.0) && (p >= 0.0));
    }
    // 通量 F(U)
    void flux(std::vector<std::vector<double>> &f) const
    {
      for (u_int i = 0; i < dim; ++i)
      {
        f[0][i] = rho * u(i); // 质量通量
        // 动量通量
        f[i + 1][i] = p;
        for (u_int j = 0; j < dim; ++j)
        {
          f[i + 1][j] += rho * u(i) * u(j);
        }
        f[dim + 1][i] = (E + p) * u(i); // 能量通量
      }
    }

  private:
    void set_as_default_value()
    {
      for (u_int i = 0; i < dos; ++i)
      {
        (*this)(i) = default_value[i];
      }
      update_auxvar();
    }

  public:
    void update_auxvar()
    {
      rho = (*this)(0);
      V2 = 0.0;
      for (int i = 0; i < dim; ++i)
      {
        u(i) = (*this)(i + 1) / rho;
        V2 += u(i) * u(i);
      }
      E = (*this)(dim + 1);
      e = E / rho - 0.5 * V2;
      p = (gamma - 1) * (E - 0.5 * rho * V2);
      if (is_feasible())
      {
        double c2 = gamma * p / rho;
        c = (c2 >= 0.0) ? sqrt(c2) : 0.0;
      }
      else
      {
        set_as_default_value();
      }
    }

    this_t &equ(double d)
    {
      Vector<double>::operator=(d);
      return *this;
    }
    this_t &equ(double s, const this_t &f)
    {
      Vector<double>::equ(s, f);
      return *this;
    }

    this_t &add(double s, const flux_t &f)
    {
      Vector<double>::add(s, f);
      return *this;
    }
    this_t &add(double s, const this_t &f)
    {
      Vector<double>::add(s, f);
      return *this;
    }

    friend Migration::ostream<> &
    operator<<(Migration::ostream<> &os, const this_t &gls)
    {
      const SVector<dos> &v(gls);
      os << v;
      return os;
    }
    friend Migration::istream<> &
    operator>>(Migration::istream<> &is, this_t &gls)
    {
      SVector<dos> &v(gls);
      is >> v;
      return is;
    }
  };
  // 静态成员变量
  template <int D, u_int DOF>
  double GammaLawGas<D, DOF>::gamma;
  template <int D, u_int DOF>
  double GammaLawGas<D, DOF>::default_value[GammaLawGas<D, DOF>::dos];
}
// 数值通量
namespace RiemannSolver
{
  template <typename SOL>
  struct InGammaLawGas
  {
    // LF 通量
    static void
    lax_friedrich(const SOL &U_l,
                  const SOL &U_r,
                  const Types::point_t &pnt,
                  const std::vector<double> &n,
                  typename SOL::flux_t &F)
    {
      const Types::velocity_t &u_l = U_l.velocity();
      const Types::velocity_t &u_r = U_r.velocity();
      double un_l = 0.0, un_r = 0.0;
      for (u_int i = 0; i < n.size(); ++i)
      {
        un_l += u_l(i) * n[i];
        un_r += u_r(i) * n[i];
      }
      double lambda_l = U_l.c + fabs(un_l);
      double lambda_r = U_r.c + fabs(un_r);
      double lambda = std::max(lambda_l, lambda_r);
      std::vector<std::vector<double>> f_l(SOL::dos, std::vector<double>(SOL::dim));
      std::vector<std::vector<double>> f_r(SOL::dos, std::vector<double>(SOL::dim));
      U_l.flux(f_l);
      U_r.flux(f_r);
      for (u_int k = 0; k < SOL::dos; ++k)
      {
        F(k) = 0.5 * (innerProduct(f_l[k], n) + innerProduct(f_r[k], n) -
                      lambda * (U_r(k) - U_l(k)));
      }
    }
  };
}
#endif // _GAMMALAWGAS_H_

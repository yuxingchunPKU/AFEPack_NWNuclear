#include "GammaLawGas.h"

namespace Material
{
  template <>
  const char *GammaLawGasBase<1>::variable_name[] = {
      "rho", "rho_u", "E"};

  template <>
  const char *GammaLawGasBase<2>::variable_name[] = {
      "rho", "rho_u", "rho_v", "E"};

  template <>
  const char *GammaLawGasBase<3>::variable_name[] = {
      "rho", "rho_u", "rho_v", "rho_w", "E"};
}
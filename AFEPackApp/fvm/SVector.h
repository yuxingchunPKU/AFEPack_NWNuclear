#ifndef _SVECTOR_H_
#define _SVECTOR_H_
#include <iostream>
#include <algorithm>
#include <functional>
#include <lac/vector.h>
#include <AFEPack/BinaryBuffer.h>

template <int N>
class SVector : public Vector<double>
{
public:
  SVector() : Vector<double>(N) {}

  SVector<N> &operator=(const double s)
  {
    Vector<double>::operator=(s);
    return *this;
  }

  template <class CNT>
  SVector<N> &equ(double s, const CNT &v)
  {
    std::transform(v.begin(), v.end(), this->begin(),
                   std::bind1st(std::multiplies<double>(), s));
    return *this;
  }

  friend Migration::ostream<> &
  operator<<(Migration::ostream<> &os, const SVector<N> &v)
  {
    for (u_int i = 0; i < N; ++i)
    {
      os << v(i);
    }
    return os;
  }
  friend Migration::istream<> &
  operator>>(Migration::istream<> &is, SVector<N> &v)
  {
    for (u_int i = 0; i < N; ++i)
    {
      is >> v(i);
    }
    return is;
  }
};

#endif // _SVECTOR_H_
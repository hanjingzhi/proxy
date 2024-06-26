#if __STDC_HOSTED__
#error "This file shall be compiled targeting a freestanding environment."
#endif  // __STDC_HOSTED__

#include "proxy.h"

unsigned GetHash(int v) { return static_cast<unsigned>(v + 3) * 31; }
unsigned GetHash(double v) { return static_cast<unsigned>(v * v + 5) * 87; }
unsigned GetHash(const char* v) {
  unsigned result = 91u;
  for (int i = 0; v[i]; ++i) {
    result = result * 47u + v[i];
  }
  return result;
}
unsigned GetDefaultHash() { return -1; }

namespace spec {

PRO_DEF_FREE_DISPATCH(FreeGetHash, ::GetHash, GetHash, ::GetDefaultHash);
struct Hashable : pro::facade_builder
    ::add_convention<FreeGetHash, unsigned()>
    ::build {};

}  // namespace spec

extern "C" int main() {
  int i = 123;
  double d = 3.14159;
  const char* s = "lalala";
  std::tuple<int, double> t{11, 22};
  pro::proxy<spec::Hashable> p;
  p = &i;
  if (GetHash(*p) != GetHash(i)) {
    return 1;
  }
  p = &d;
  if (GetHash(*p) != GetHash(d)) {
    return 1;
  }
  p = pro::make_proxy_inplace<spec::Hashable>(s);
  if (GetHash(*p) != GetHash(s)) {
    return 1;
  }
  p = &t;
  if (GetHash(*p) != GetDefaultHash()) {
    return 1;
  }
  return 0;
}

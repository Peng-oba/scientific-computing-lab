#include <cassert>
#include <iostream>

#ifndef CONST_CASE
#define CONST_CASE 1
#endif

int main() {
    double a = 1.0, b = 2.0;
    (void)b;
#if CONST_CASE == 1
    double const* p = &a;
    p = &b;
    assert(p == &b);
    std::cout << "C1: value=" << *p << '\n';
#elif CONST_CASE == 2
    double const* p = &a;
    *p = 3.0;  // Expected compile failure.
#elif CONST_CASE == 3
    double* const q = &a;
    *q = 3.0;
    assert(a == 3.0);
    std::cout << "C3: a=" << a << '\n';
#elif CONST_CASE == 4
    double* const q = &a;
    q = &b;  // Expected compile failure.
#elif CONST_CASE == 5
    double const* const z = &a;
    z = &b;  // Expected compile failure.
#elif CONST_CASE == 6
    double const* const z = &a;
    *z = 3.0;  // Expected compile failure.
#elif CONST_CASE == 7
    double const& r = a;
    r = 3.0;  // Expected compile failure.
#elif CONST_CASE == 8
    double const& r = a;
    a = 4.0;
    assert(r == 4.0);
    std::cout << "C8: r=" << r << '\n';
#else
#error CONST_CASE must be an integer from 1 to 8
#endif
}

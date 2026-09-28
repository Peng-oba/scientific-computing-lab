#include <cassert>
#include <iostream>

double byValue(double x) { x = 2.0; return x; }
void byRef(double& x) { x = 2.0; }
double byConstRef(double const& x) { return x + 1.0; }
bool byPointer(double* x) {
    if (x == nullptr) { return false; }
    *x = 2.0;
    return true;
}
void rebind(double* p, double* other) { p = other; *p = 9.0; }

int main() {
    double x = 1.0;
    double result = byValue(x);
    assert(x == 1.0 && result == 2.0);
    std::cout << "value: x=" << x << " result=" << result << '\n';

    x = 1.0;
    byRef(x);
    assert(x == 2.0);
    std::cout << "reference: x=" << x << '\n';

    x = 1.0;
    result = byConstRef(x);
    assert(x == 1.0 && result == 2.0);
    std::cout << "const-reference: x=" << x << " result=" << result << '\n';

    x = 1.0;
    bool const changed = byPointer(&x);
    assert(changed && x == 2.0);
    std::cout << "pointer: x=" << x << " changed=" << std::boolalpha << changed << '\n';
    bool const null_changed = byPointer(nullptr);
    assert(!null_changed);
    std::cout << "nullptr: changed=" << null_changed << '\n';

    double a = 1.0, b = 2.0;
    double* p = &a;
    rebind(p, &b);
    assert(a == 1.0 && b == 9.0 && p == &a);
    std::cout << "rebind: a=" << a << " b=" << b << " p_is_a=" << (p == &a) << '\n';

    a = 1.0; b = 2.0;
    double& r = a;
    r = b;
    assert(&r == &a && a == 2.0 && b == 2.0);
    std::cout << "reference-assignment: a=" << a << " r_is_a=" << (&r == &a) << '\n';
}

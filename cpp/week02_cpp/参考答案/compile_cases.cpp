#include <cassert>
#include <memory>
#include <utility>

#ifndef CASE
#define CASE 4
#endif

void take(std::unique_ptr<int> p) { assert(p && *p == 7); }

#if CASE == 3
struct Base {
    virtual ~Base() = default;
    virtual double stress(double strain) const = 0;
};
struct Bad : Base {
    double stress(double strain) override { return strain; } // Missing const.
};
#endif

int main() {
    auto p = std::make_unique<int>(7);
#if CASE == 1
    auto q = p; // Expected: copy of unique_ptr is deleted.
    (void)q;
#elif CASE == 2
    take(p); // Expected: no copy into by-value unique_ptr parameter.
#elif CASE == 3
    (void)p;
#elif CASE == 4
    take(std::move(p));
    assert(!p);
#elif CASE == 5
    auto const q = std::make_unique<int>(7);
    *q = 8; // const applies to q, not to its int pointee.
    assert(*q == 8);
#else
#error CASE must be 1 through 5
#endif
}

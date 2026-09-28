#include <cassert>
#include <iostream>

// This class only makes lifetime events visible. Full RAII study is in week 2.
struct Tracer {
    char const* name;
    explicit Tracer(char const* n) : name(n) { std::cout << "create " << name << '\n'; }
    ~Tracer() { std::cout << "destroy " << name << '\n'; }
};

double safeValue() { double local = 3.0; return local; }

int main() {
    Tracer outer("outer");
    {
        Tracer inner("inner");
        std::cout << "inside block\n";
    }
    std::cout << "outer still alive\n";

    double const result = safeValue();
    assert(result == 3.0);
    double local = 3.0;
    double* alias = &local;
    assert(*alias == 3.0);  // local is still alive here.
    std::cout << "safe value and live borrow: PASS\n";
}

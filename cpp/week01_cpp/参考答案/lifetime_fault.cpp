#include <iostream>

// Intentional isolated fault for optional AddressSanitizer observation.
// This is not a normal reference program and must not be copied into OGS.
int main() {
    double* p = new double(3.0);
    double* alias = p;
    delete p;
    p = nullptr;
    std::cout << *alias << '\n';  // Intentional use-after-free.
}

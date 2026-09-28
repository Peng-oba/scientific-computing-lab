#include <iostream>
#include <limits>

int main()
{
    volatile int maximum = std::numeric_limits<int>::max();
    const int result = maximum + 1;  // Intentional: signed integer overflow (UB).
    std::cout << result << '\n';
}

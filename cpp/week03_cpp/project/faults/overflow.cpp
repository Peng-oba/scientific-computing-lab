#include <iostream>
#include <limits>

int main()
{
    volatile int maximum = std::numeric_limits<int>::max();

    // 整数溢出
    const int result = maximum + 1;
    
    std::cout << result << '\n';
}

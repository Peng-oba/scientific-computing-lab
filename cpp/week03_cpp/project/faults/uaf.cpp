#include <iostream>

int main()
{
    double *values = new double[2]{1.0, 2.0};

    const double *borrowed = values;

    delete[] values;

    // 悬垂指针
    std::cout << borrowed[0] << '\n';
}

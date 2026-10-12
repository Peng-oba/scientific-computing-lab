#include <iostream>

int main(int argc, char**)
{
    int *values = new int[3]{10, 20, 30};

    // 若无额外参数: index == 3
    const int index = argc + 2;

    // 索引超出范围
    std::cout << values[index] << '\n';

    delete[] values;
}

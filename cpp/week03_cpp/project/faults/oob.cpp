#include <iostream>

int main(int argc, char**)
{
    int* values = new int[3]{10, 20, 30};
    const int index = argc + 2;  // With no extra arguments: index == 3.
    std::cout << values[index] << '\n';  // Intentional: heap-buffer-overflow.
    delete[] values;
}

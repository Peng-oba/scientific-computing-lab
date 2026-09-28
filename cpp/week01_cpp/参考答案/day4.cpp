#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

int main() {
    std::vector<int> a{1, 2, 3};
    auto b = a;
    b[0] = 9;
    assert(a[0] == 1 && b[0] == 9);
    std::cout << "copy: a0=" << a[0] << " b0=" << b[0] << '\n';

    auto c = std::move(a);
    assert((c == std::vector<int>{1, 2, 3}));
    // No assumption about a's size or contents after the move.
    a = {4, 5};
    assert((a == std::vector<int>{4, 5}));
    std::cout << "move destination: 1 2 3\n";
    std::cout << "source reassigned: 4 5\n";
}

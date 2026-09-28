#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

int main()
{
    std::vector<int> a{1, 2, 3};

    // 调用的是 vector 的拷贝构造函数, 会在堆上新分配一块内存, 把 a 的元素逐个拷贝过去
    // b 和 a 拥有各自独立的堆内存, 互不影响
    auto b = a;
    b[0] = 9;

    // b 改动不会影响 a
    assert(a[0] == 1 && b[0] == 9);
    std::cout << "copy: a[0] = " << a[0] << ", b[0] = " << b[0] << '\n';

    // 堆内存没有移动, 而是由 c 接管
    // a 仍然是一个合法的 vector 对象
    // 但它的内容已被 "掏空", 通常变成空 vector
    auto c = std::move(a);
    assert((c == std::vector<int>{1, 2, 3}));

    std::cout << "c[0] = " << c[0] << ", c[1] = " << c[1] << ", c[2] = " << c[2] << '\n';
    std::cout << "size of c: " << c.size() << '\n';

    // a 没有销毁, 处于 "有效但未指定" 状态
    std::cout << "size of a: " << a.size() << '\n';

    a = {4, 5};
    assert((a == std::vector<int>{4, 5}));

    std::cout << "a[0] = " << a[0] << ", a[1] = " << a[1] << '\n';

}

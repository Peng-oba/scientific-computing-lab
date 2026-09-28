#include <iostream>
#include <vector>

/// \note const 修饰它左边的东西; 如果左边没有东西, 就修饰右边的东西


int main()
{
    double a = 1.0;
    double b = 2.0;

    std::cout << "&a = " << &a << ", &b = " << &b << '\n';

    // const 修饰 double
    // p 可以赋予新地址, 但不能通过 *p 修改值
    double const *p = &a;
    std::cout << "&p1 = " << p << ", *p1 = " << *p << '\n';
    p = &b;
    std::cout << "&p2 = " << p << ", *p2 = " << *p << '\n';

    // *p = 3.0; error! 不能修改 *p

    // const 修饰 *
    // 可以通过 *p 修改值, 但不能赋予 p 新地址 
    double *const q = &a;
    *q = 3.0;
    std::cout << "&q = " << q << ", *q = " << *q << ", a = " << a << '\n';

    // q = &b;  error! 不能修改 p

    // const 同时修饰 double 和 *
    // z 就不能赋予新地址, 也不能通过 *z 修改值
    double const* const z = &a;

    a = 1.0;

    // 只读引用
    // 不能经 r 进行赋值
    double const& r = a;

    // r = 3.0; error!

    // 可以修改 a
    a = 4.0;
    std::cout << "r = " << r << '\n';
}
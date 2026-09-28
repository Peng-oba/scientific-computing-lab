#include <iostream>

// 按值传递 (pass by value) 会复制实参, 在栈上创建一份独立的副本
// 对于大对象(如 Eigen::VectorXd), 复制开销可能很大，造成性能浪费
double byValue(double x)
{
    // 把局部 x 设为 2, 返回 x
    x = 2.0;

    return x;
}

// 引用: 此时 x 是传入对象的别名 (alias), 不是独立对象
void byRef(double &x)
{
    // 把原对象设为 2
    x = 2.0;
}

double byConstRef(double const &x)
{
    // 返回 x+1, 不修改 x
    return x + 1.0;
}

// 函数接收指针, 通过解引用修改所指对象
// 传入 &a, 函数内 *x = 2.0 修改的就是 a
bool byPointer(double *x)
{
    if(!x)
    {
        return false;
    }

    *x = 2.0;

    return true;
}

// 会创建指针副本 p_copy, 因此在函数中是对 p_copy 进行重新赋值, 而不是原 p 指针
void rebind(double *p, double *other)
{
    // 普通指针可以被重新赋值
    p = other;
    *p = 9.0;
}

int main()
{
    double x = 1.0;
    double result = byValue(x);

    std::cout << "value: x = " << x << " result = " << result << '\n';

    x = 1.0;
    byRef(x);

    std::cout << "reference: x = " << x << '\n';

    x = 1.0;
    result = byConstRef(x);

    std::cout << "const-reference: x = " << x << " result = " << result << '\n';

    x = 1.0;
    bool changed = byPointer(&x);

    std::cout << "pointer: x = " << x << " changed = " << std::boolalpha << changed << '\n';

    bool null_changed = byPointer(nullptr);

    std::cout << "nullptr: changed = " << null_changed << '\n';

    double a = 1.0;
    double b = 2.0;

    double *p = &a;

    rebind(p, &b);

    std::cout << "rebind: a = " << a << " b = " << b << " p_is_a = " << (p == &a) << '\n';

    a = 1.0; b = 2.0;

    double &r = a;
    // r = b 是给 r 原先引用的对象赋值
    // 而非改绑到另一个对象
    r = b;

    std::cout << "reference-assignment: a = " << a << " r_is_a = " << (&r == &a) << '\n';
}
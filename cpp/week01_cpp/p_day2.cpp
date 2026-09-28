#include <cassert>
#include <iostream>
#include <vector>

/*
    Automatic Storage Duration (自动存储期)
    变量的生命周期由作用域 (大括号 {}) 自动控制, 进入作用域时创建, 离开作用域时自动销毁.
    scope 与 lifetime 高度相关.

    Dynamic Storage Duration (动态存储期)
    变量的生命周期由手动控制, 通过 new 创建, 通过 delete 销毁, 不受作用域限制, 你让它活多久就活多久.
*/

// Automatic Storage Duration
void f1()
{
    // a 从栈上申请内存, 函数结束时自动销毁
     double a = 1.0;
}

// Dynamic Storage Duration
void f2()
{
    // 从堆上申请内存，并初始化为 1.0
    double *b = new double{1.0};

    // b 是指针变量，本身在栈上
    // 函数结束时, b (栈上的指针) 自动销毁
    // 但堆上的 double 对象不会自动销毁, 必须手动 delete
    delete b;

    // 如果没有 delete：
    // 堆上的 8 字节依然存在
    // 但没有任何指针指向它 → 无法再访问, 也无法释放
    // 这就是内存泄漏 (memory leak)
    // 泄漏的内存会一直占用, 直到程序结束
}


void f3()
{
    // v 本身在栈内
    // 1.0、2.1、1.5 是在堆内
    std::vector<double> v(1000);
    v.push_back(1.0);
    v.push_back(2.1);
    v.push_back(1.5);

    // 获得的是 1.0 的内存地址
    // 即此时 p 指向地是堆内存
    double *p = v.data();

    std::cout << "stack: " << &v << '\n';
    std::cout << "heap: " << p << '\n';
    std::cout << "1.0: " << &v[0] << '\n';
}

struct Tracer
{
    char const* name;

    explicit Tracer(char const* n) : name(n) { std::cout << "create " << name << '\n'; }

    ~Tracer() { std::cout << "destroy " << name << '\n'; }
};

double safeValue()
{ 
    double local = 3.0;
    
    // 返回 local 的 副本
    return local;
}


int main()
{
    // 局部变量, 离开作用域时销毁
    Tracer outer("outer");  // 创建 outer
    {
        Tracer inner("inner");  // 创建 inner
        std::cout << "inside block\n";
    }   // 销毁 inner

    std::cout << "outer still alive\n";

    // 临时对象, 当前语句结束结束时销毁
    Tracer("Temporary");

    // 通过 const& 绑定临时对象可延长临时对象声明周期和 t 一致
    Tracer const& t = Tracer("Const_Temporary");

    double const result = safeValue();
    assert(result == 3.0);

    double local = 3.0;
    double *alias = &local;

    assert(*alias == 3.0);

    std::cout << "safe value and live borrow: PASS\n";


}   // 销毁 outer

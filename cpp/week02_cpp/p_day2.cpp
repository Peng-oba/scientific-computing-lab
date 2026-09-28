#include <cassert>
#include <iostream>
#include <memory>
#include <utility>

struct Resource
{
    inline static int live = 0;
    int value;

    explicit Resource(int v) : value(v)
    { 
        ++live;

        std::cout << "create " << value << '\n'; 
    }

    ~Resource() { std::cout << "destroy " << value << '\n'; --live; }
};

// consume 接管对象声明周期
// 函数结束时, 销毁对象
void consume(std::unique_ptr<Resource> owner)
{
    assert(owner && owner->value == 11);

    std::cout << "consume value = " << owner->value << '\n';
}

int main()
{
    std::cout << "unique begin\n";

    auto p = std::make_unique<Resource>(7);

    // borrowed 获取 p 内部指向 7 的指针
    // borrowed 为临时访问对象, 不负责销毁
    Resource *borrowed = p.get();

    // p 不放弃所有权
    std::cout << "The p value is: " << p->value << '\n';
    std::cout << "The borrowed value is: " << borrowed->value << '\n';

    std::cout << "Live = " << p->live << '\n';

    // 仅仅一个类型转换 (std::move) 不会转移所有权
    (void)std::move(p);
    assert(p && p.get() == borrowed);
    std::cout << "U1: cast alone keeps p\n";

    // 触发移动构造, 转移所有权
    // q 接管对象, 因此并不会出发析构函数
    auto q = std::move(p);
    std::cout << "Live = " << q->live << '\n';

    // 此时 p 为空
    if(!p) {
        std::cout << "p is nullpter" << '\n';
    }

    assert(!p && q.get() == borrowed && borrowed->value == 7);
    assert(Resource::live == 1);
    std::cout << "U2: p empty, q owns the same object\n";

    // 销毁 q 当前拥有的对象 (调用对象的析构函数)
    // 把 q 置空 (q 变成 nullptr)
    q.reset();

    if(!q) {
        std::cout << "q is nullpter" << '\n';
    }

    // borrowed 如果不处理会变成悬垂指针
    borrowed = nullptr;

        if(!borrowed) {
        std::cout << "borrowed is nullpter" << '\n';
    }

    std::cout << "Live = " << q->live << '\n';

    assert(Resource::live == 0);
    std::cout << "U3: reset destroyed the object\n";

    auto r = std::make_unique<Resource>(9);

    // r 放弃所有权, 返回裸指针
    // 此时 r 为空
    Resource *raw = r.release();

    if(!r) {
        std::cout << "r is nullpter" << '\n';
    }

    assert(!r && raw->value == 9 && Resource::live == 1);

    // 需手动释放内存, 否则会造成内存泄漏
    delete raw;
    
    std::cout << "U4: release did not destroy the object\n";

    raw = nullptr;
    assert(Resource::live == 0);

    auto input = std::make_unique<Resource>(11);
    consume(std::move(input));
    assert(!input && Resource::live == 0);
    std::cout << "U5: consume accepted and released ownership\n";

    std::cout << "shared begin\n";

    auto a = std::make_shared<Resource>(13);
    assert(a.use_count() == 1);

    // 有多少个对象指向同一个对象
    std::cout << "S1: count=" << a.use_count() << '\n';

    auto b = a;
    assert(a.get() == b.get() && a.use_count() == 2 && Resource::live == 1);

    std::cout << "S2: count=" << a.use_count() << '\n';

    // view 仍是裸指针
    Resource *view = a.get();
    assert(view == b.get() && a.use_count() == 2);

    std::cout << "S3: get keeps count=" << a.use_count() << '\n';

    // a 销毁了, 但是 b 仍指向对象
    // 此时并不会调用析构函数
    a.reset();
    assert(b.use_count() == 1 && Resource::live == 1);

    std::cout << "S4: one owner remains, count=" << b.use_count() << '\n';
    
    // 最后一个对象销毁, 调用析构函数
    b.reset();
    view = nullptr;
    assert(Resource::live == 0);
    
    std::cout << "S5: last owner released, live=0\n";
}

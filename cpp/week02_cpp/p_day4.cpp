#include <cassert>
#include <iostream>
#include <memory>

struct Node
{
    inline static int live = 0;

    char const* name;

    std::shared_ptr<Node> child;
    std::weak_ptr<Node> parent;

    explicit Node(char const* n) : name(n)
    { 
        ++live;
        std::cout << "create " << name << '\n';
    }

    ~Node() { --live; std::cout << "destroy " << name << '\n'; }
};

int main()
{
    std::weak_ptr<Node> observer;

    {
        auto owner = std::make_shared<Node>("single");
        observer = owner;

        std::cout << owner->name << '\n';

        // weak_ptr 无法直接访问对象
        // std::cout << observer->name << '\n'; error

        // weak_ptr 不拥有对象, 不增加引用计数
        assert(owner.use_count() == 1);
        std::cout << "owner count: " << owner.use_count() << '\n';

        // 将 weak_ptr 升级为共享所有权
        auto keep = observer.lock();

        // 应用计数变为 2
        assert(keep && keep.use_count() == 2);

        std::cout << "owner count: " << owner.use_count() << '\n';
        std::cout << keep->name << '\n';

        // observer.expired() 判断对象是否销毁
        owner.reset();
        assert(!observer.expired() && Node::live == 1);

        keep.reset();
        assert(observer.expired() && !observer.lock() && Node::live == 0);
    }

    std::cout << "W1: weak does not own; successful lock temporarily owns\n";

    {
        auto a = std::make_shared<Node>("A");
        auto b = std::make_shared<Node>("B");

        std::cout << "Live: " << b->live << '\n';

        a->child = b;
        b->parent = a;  // Non-owning back edge.

        // A 只被 a 拥有, b->parent 是 weak_ptr, 不增加计数
        // B 被 b 和 a->child 两个 shared_ptr 拥有
        assert(a.use_count() == 1 && b.use_count() == 2);

        // a 销毁 -> Node A 销毁 -> a->child 销毁 -> B 计数减 1
        a.reset();

        std::cout << "Live: " << b->live << '\n';

        assert(b->parent.expired() && b.use_count() == 1 && Node::live == 1);
        b.reset();
        
        assert(Node::live == 0);
    }

    std::cout << "W2: weak back edge permits both nodes to be destroyed\n";
}

#include <cassert>
#include <iostream>
#include <memory>

struct Node {
    inline static int live = 0;
    char const* name;
    std::shared_ptr<Node> child;
    std::weak_ptr<Node> parent;
    explicit Node(char const* n) : name(n) { ++live; std::cout << "create " << name << '\n'; }
    ~Node() { --live; std::cout << "destroy " << name << '\n'; }
};

int main() {
    std::weak_ptr<Node> observer;
    {
        auto owner = std::make_shared<Node>("single");
        observer = owner;
        assert(owner.use_count() == 1);
        auto keep = observer.lock();
        assert(keep && keep.use_count() == 2);
        owner.reset();
        assert(!observer.expired() && Node::live == 1);
        keep.reset();
        assert(observer.expired() && !observer.lock() && Node::live == 0);
    }
    std::cout << "W1: weak does not own; successful lock temporarily owns\n";
    {
        auto a = std::make_shared<Node>("A");
        auto b = std::make_shared<Node>("B");
        a->child = b;
        b->parent = a;  // Non-owning back edge.
        assert(a.use_count() == 1 && b.use_count() == 2);
        a.reset();
        assert(b->parent.expired() && b.use_count() == 1 && Node::live == 1);
        b.reset();
        assert(Node::live == 0);
    }
    std::cout << "W2: weak back edge permits both nodes to be destroyed\n";
}

#include <cassert>
#include <iostream>
#include <memory>
#include <utility>

struct Resource {
    inline static int live = 0;
    int value;
    explicit Resource(int v) : value(v) { ++live; std::cout << "create " << value << '\n'; }
    ~Resource() { std::cout << "destroy " << value << '\n'; --live; }
};

void consume(std::unique_ptr<Resource> owner) {
    assert(owner && owner->value == 11);
    std::cout << "consume value=" << owner->value << '\n';
}

int main(){
    std::cout << "unique begin\n";
    auto p = std::make_unique<Resource>(7);
    Resource* borrowed = p.get();
    (void)std::move(p);  // A cast alone does not transfer ownership.
    assert(p && p.get() == borrowed);
    std::cout << "U1: cast alone keeps p\n";
    auto q = std::move(p);
    assert(!p && q.get() == borrowed && borrowed->value == 7);
    assert(Resource::live == 1);
    std::cout << "U2: p empty, q owns the same object\n";
    q.reset();
    borrowed = nullptr;  // No dereference after the pointee is destroyed.
    assert(Resource::live == 0);
    std::cout << "U3: reset destroyed the object\n";

    auto r = std::make_unique<Resource>(9);
    Resource* raw = r.release();
    assert(!r && raw->value == 9 && Resource::live == 1);
    std::cout << "U4: release did not destroy the object\n";
    delete raw;  // Only this isolated legacy-interface demonstration uses manual delete.
    raw = nullptr;
    assert(Resource::live == 0);

    auto input = std::make_unique<Resource>(11);
    consume(std::move(input));
    assert(!input && Resource::live == 0);
    std::cout << "U5: consume accepted and released ownership\n";

    std::cout << "shared begin\n";
    auto a = std::make_shared<Resource>(13);
    assert(a.use_count() == 1);
    std::cout << "S1: count=" << a.use_count() << '\n';
    auto b = a;
    assert(a.get() == b.get() && a.use_count() == 2 && Resource::live == 1);
    std::cout << "S2: count=" << a.use_count() << '\n';
    Resource* view = a.get();
    assert(view == b.get() && a.use_count() == 2);
    std::cout << "S3: get keeps count=" << a.use_count() << '\n';
    a.reset();
    assert(b.use_count() == 1 && Resource::live == 1);
    std::cout << "S4: one owner remains, count=" << b.use_count() << '\n';
    b.reset();
    view = nullptr;
    assert(Resource::live == 0);
    std::cout << "S5: last owner released, live=0\n";
}

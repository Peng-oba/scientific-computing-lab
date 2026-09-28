#include <iostream>
#include <memory>
#include <vector>

using namespace std;

/**
 * @brief 唯一指针 unique_ptr
 *        auto ptr = std::make_unique<Type>(args...);
 * 访问: 和裸指针一样, 用 -> 或 *
 * 销毁: 离开作用域 {} 自动销毁
 */

class Rock
{
    private:
        string _name;

    public:
        Rock(string name) : _name(name)
        {
            cout << "Rock [" << name << "] Created.\n";
        }
        ~Rock()
        {
            cout << "Rock [" << _name << "] Destroyed.\n";
        }
        void printInfo()
        {
            cout << "Rock Type is: " << _name << endl;
        }
};

class Mesh
{
    private:
        string _name;

    public:
        Mesh(string n) : _name(n)
        {
            cout << "Mesh [" << _name << "] Created.\n";
        }

        ~Mesh() {cout<<"Mesh [" << _name << "] Destroyed.\n";}

        void printInfo()
        {
            cout << "Mesh Type is: " << _name << "\n";
        }
};

void Test1()
{
    // 创建 unique_ptr
    unique_ptr<Mesh> mesh = make_unique<Mesh>("GloblaMesh");
    mesh->printInfo();

    // unique_ptr 禁止拷贝
    // unique_ptr<Mesh> copy_mesh = my_mesh;    Error! 

    cout << "Out of scope!" << '\n';
}   // 离开作用域, unique_ptr 自动销毁释放内存

void Test2()
{
    unique_ptr<Rock> rock1 = make_unique<Rock>("Granite");

    // 将 rock1 的地址移动到 rock2, rock1 变空
    unique_ptr<Rock> rock2 = move(rock1);
    rock2->printInfo();

    if (rock1 == nullptr)
        cout << "my_rock is empty!" << endl;

    cout << "Out of scope!" << '\n';
}

struct Element
{
    int id;

    Element(int i) : id(i) { cout << "Element " << id << " created.\n"; }

    ~Element() {cout << "Element " << id << " destroyed.\n"; }
};

unique_ptr<Element> createElement (int id)
{
    auto el = make_unique<Element>(id);

    return el;
}

int main()
{
    Test1();

    Test2();

    vector<unique_ptr<Element>> element_container;

    auto ptr = createElement(100);

    /// element_container.emplace_back(ptr); 试图拷贝 ptr
    element_container.emplace_back(move(ptr));

    if (ptr == nullptr)
    {
        cout << "Original ptr is empty (nullptr).\n";
    }

    // // 清空 vector, 依旧会调用析构函数
    // element_container.clear();
}
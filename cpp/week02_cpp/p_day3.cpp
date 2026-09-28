#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>

class Material
{
public:
    // 虚析构函数
    // 如果没有 virtual, 只会调用基类析构, 派生类资源泄漏
    virtual ~Material() { std::cout << "Material base destructor\n"; }

    // 纯虚函数
    // 派生类必须重写
    virtual double stress(double strain) const = 0;

};

class ElasticMaterial : public Material
{
public:
    // 避免隐式转换
    // 多用于单参数构造函数
    explicit ElasticMaterial(double modulus) : modulus_(modulus)
    {
        if (modulus <= 0.0) { throw std::invalid_argument("modulus must be positive"); }
        std::cout << "ElasticMaterial constructed\n";
    }

    ~ElasticMaterial() override { std::cout << "ElasticMaterial destructor\n"; }

    // 派生类重写虚函数时, 要注意函数构造和基类保持一致
    // const 说明 modulus_ 不能被修改
    double stress(double strain) const override { return modulus_ * strain; }

private:
    double modulus_;
};

// 工厂函数
// 负责选择和创建具体类型, 返回的 unique_ptr 将所有权交给调用方
std::unique_ptr<Material> createMaterial(const double modulus)
{
    // 这里相当于隐式调用了 std::move()
    return std::make_unique<ElasticMaterial>(modulus);
}

double evaluate(Material const& material, double strain)
{
    return material.stress(strain);
}   // 函数结束时, 引用别名销毁, 引用本身并不会销毁

int main()
{
    {
        std::unique_ptr<Material> material = createMaterial(1000.0);

        double const sigma = evaluate(*material, 0.01);

        assert(std::abs(sigma - 10.0) < 1e-12);
        std::cout << "stress = " << sigma << '\n';
    }

    std::cout << "D3: derived and base destruction finished\n";
}

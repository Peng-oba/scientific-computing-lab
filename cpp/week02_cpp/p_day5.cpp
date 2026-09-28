#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

class Material
{
public:
    inline static int live = 0;

    Material() { ++live; }
    virtual ~Material() { --live; std::cout << "Material base destructor\n"; }

    virtual double stress(double strain) const = 0;
    virtual char const* name() const noexcept = 0;
};

class ElasticMaterial : public Material
{
public:
    explicit ElasticMaterial(double modulus) : modulus_(modulus)
    {
        std::cout << "ElasticMaterial constructed\n";
    }

    ~ElasticMaterial() override
    { 
        std::cout << "ElasticMaterial destructor\n"; 
    }

    double stress(double strain) const override { return modulus_ * strain; }

    char const* name() const noexcept override { return "ElasticMaterial"; }

private:
    double modulus_;
};

std::unique_ptr<Material> createMaterial(double modulus)
{
    if (!(modulus > 0.0) || !std::isfinite(modulus)) 
    {
        throw std::invalid_argument("modulus must be finite and positive");
    }

    return std::make_unique<ElasticMaterial>(modulus);
}

class Solver
{
public:
    inline static int live = 0;

    explicit Solver(Material const &material) : material_(material)
    {
        ++live;
        std::cout << "Solver constructed\n";
    }

    ~Solver()
    {
        // "被借用者" 必须比 "借用者" 活得更久
        std::cout << "Solver destructor; material still alive: " << material_.name() << '\n';

        --live;
    }

    double solve(double strain) const { return material_.stress(strain); }

private:
    Material const &material_; // 借用, 不负责释放
};

class Simulation
{
public:
    explicit Simulation(std::unique_ptr<Material> material)
        : material_(requireMaterial(std::move(material))),
          solver_(std::make_unique<Solver>(*material_))
    {
        std::cout << "Simulation constructed\n";
    }

    ~Simulation() { std::cout << "Simulation destructor body\n"; }

    Simulation(Simulation const&) = delete;
    Simulation& operator=(Simulation const&) = delete;

    double run(double strain) const { return solver_->solve(strain); }
    Material const* materialAddress() const { return material_.get(); }

private:
    static std::unique_ptr<Material> requireMaterial(std::unique_ptr<Material> p)
    {
        if (!p)
        { 
            throw std::invalid_argument("null material"); 
        }

        return p;
    }

    std::unique_ptr<Material> material_;    // 先声明, 后析构
    std::unique_ptr<Solver> solver_;        // 后声明, 先析构

    // solver_ 持有 material_ 的借用 (Material const&)
    // 如果 material_ 先析构, solver_ 的引用就悬垂了
};

bool near(double a, double b)
{ 
    return std::abs(a - b) < 1e-12; 
}

void checkReleased(char const* label)
{
    assert(Material::live == 0 && Solver::live == 0);

    std::cout << label << ": PASS, live = 0\n";
}

void earlyReturn()
{
    Simulation sim(createMaterial(1000.0));
    assert(near(sim.run(0.01), 10.0));

    std::cout << "early return requested\n";
    return;
}

void throwAfterConstruction()
{
    Simulation sim(createMaterial(1000.0));

    std::cout << "exception requested\n";
    throw std::runtime_error("planned failure");
}

int main()
{
    {
        // createMaterial 创建的对象经返回值与移动参数进入 Simulation
        Simulation sim(createMaterial(1000.0));

        assert(near(sim.run(0.01), 10.0));

        std::cout << "stress = 10\n";
    }

    checkReleased("M1-normal");

    earlyReturn();

    checkReleased("M2-early-return");

    try { throwAfterConstruction(); }
    catch (std::runtime_error const&) { std::cout << "exception caught\n"; }

    checkReleased("M3-exception");

    {
        auto material = createMaterial(2000.0);
        Material const *address = material.get();

        // std::move() 只是转移所有权, 并不移动对象本身
        Simulation sim(std::move(material));
        // 原指针变空
        if(!material) {
            std::cout << "material is empty!\n";
        }
        assert(!material && sim.materialAddress() == address);
        assert(near(sim.run(0.01), 20.0));

        std::cout << "M4-transfer: same material, source empty, stress = 20\n";
    }

    checkReleased("M4-transfer");

    {
        auto owner = createMaterial(1000.0);    // owner 拥有 Material
        // 一个材料可以有多个 solver
        Solver first(*owner);   // first 借用 *Material
        Solver second(*owner);  // second 借用 *Material

        // 借用者不增加 Material 的计数
        assert(Material::live == 1 && Solver::live == 2);
        assert(near(first.solve(0.01), 10.0) && near(second.solve(0.02), 20.0));

        std::cout << "M5-two-borrowers: one material, two solvers\n";
    }

    checkReleased("M5-two-borrowers");

    bool rejected = false;

    // nullptr 抛异常
    // simulation 构造失败, 不调用析构函数
    try { Simulation invalid(nullptr); }
    catch (std::invalid_argument const&) { rejected = true; }
    assert(rejected);

    checkReleased("M6-null-rejected-before-use");
}

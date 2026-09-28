#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>

class Material {
public:
    virtual ~Material() { std::cout << "Material base destructor\n"; }
    virtual double stress(double strain) const = 0;
};

class ElasticMaterial : public Material {
public:
    explicit ElasticMaterial(double modulus) : modulus_(modulus) {
        if (modulus <= 0.0) { throw std::invalid_argument("modulus must be positive"); }
        std::cout << "ElasticMaterial constructed\n";
    }
    ~ElasticMaterial() override { std::cout << "ElasticMaterial destructor\n"; }
    double stress(double strain) const override { return modulus_ * strain; }
private:
    double modulus_;
};

std::unique_ptr<Material> createMaterial(double modulus) {
    return std::make_unique<ElasticMaterial>(modulus);
}

double evaluate(Material const& material, double strain) {
    return material.stress(strain);  // Borrow without taking ownership.
}

int main() {
    {
        std::unique_ptr<Material> material = createMaterial(1000.0);
        double const sigma = evaluate(*material, 0.01);
        assert(std::abs(sigma - 10.0) < 1e-12);
        std::cout << "stress=" << sigma << '\n';
    }
    std::cout << "D3: derived and base destruction finished\n";
}

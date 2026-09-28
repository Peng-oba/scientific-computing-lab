#include "lab/material.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {
bool near(double actual, double expected)
{
    return std::abs(actual - expected) <= 1e-12;
}

bool normal()
{
    // TODO: implement this case yourself; false means NOT completed.
    return false;
}

bool boundary()
{
    // TODO: implement this case yourself; false means NOT completed.
    return false;
}

bool invalid()
{
    // TODO: implement this case yourself; false means NOT completed.
    return false;
}

bool repeated()
{
    lab::MaterialState state;
    lab::updateStress(state, 1000.0, 0.01);
    lab::updateStress(state, 1000.0, 0.01);
    return near(state.stress, 10.0);
}
}  // namespace

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "usage: material_tests normal|boundary|invalid|repeated\n";
        return 2;
    }
    const std::string name = argv[1];
    bool passed = false;
    try {
        if (name == "normal") passed = normal();
        else if (name == "boundary") passed = boundary();
        else if (name == "invalid") passed = invalid();
        else if (name == "repeated") passed = repeated();
        else {
            std::cerr << "unknown case: " << name << '\n';
            return 2;
        }
    } catch (const std::exception& error) {
        std::cerr << "unexpected exception: " << error.what() << '\n';
        return 1;
    }
    std::cout << name << ": " << (passed ? "PASS" : "FAIL") << '\n';
    return passed ? 0 : 1;
}

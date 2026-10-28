#include "lab/material.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
// 判断两个浮点数是否近似相等 (容差 1e-12)
bool near(double actual, double expected)
{
    return std::abs(actual - expected) <= 1e-12;
}

// 正常路径: 验证 stress = modulus * strain
// 输入 1000.0 * 0.01, 期望 10.
bool normal()
{
    lab::MaterialState state;
    lab::updateStress(state, 1000.0, 0.01);
    return near(state.stress, 10.0);
}

// 边界情况: strain = 0 时, stress 应为 0
// 验证 1000.0 * 0.0 = 0.0
bool boundary()
{
    lab::MaterialState state{7.0};
    lab::updateStress(state, 1000.0, 0.0);
    return near(state.stress, 0.0);
}

// 非法输入: modulus = -1000.0 应抛 std::invalid_argument
// 且 state.stress 保持原值 7.0（强异常安全）
bool invalid()
{
    lab::MaterialState state{7.0};
    try {
        lab::updateStress(state, -1000.0, 0.01);
    } catch (const std::invalid_argument&) {
        return near(state.stress, 7.0);
    }
    return false;
}

// 重复调用: 验证 updateStress 是 "覆盖" 而非 "累加"
// 两次 updateStress(1000.0, 0.01) 后, stress 应为 10.0 (不是 20.0)
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

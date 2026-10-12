#pragma once

namespace lab {
struct MaterialState
{
    double stress = 0.0;
};

// strain 表示总应变, 而非应变增量.
// 弹性模量 E (即 modulus) 必须大于 0; 输入参数必须为有限值.
// 输入参数无效时, 抛出 std::invalid_argument 异常, 且不改变 state.
void updateStress(MaterialState& state, double modulus, double strain);
}  // 命名空间 lab

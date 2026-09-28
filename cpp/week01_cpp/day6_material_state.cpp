#include <iostream>
#include <Eigen/Dense>

struct MaterialState
{
    double damage;
    double kappa;
    Eigen::VectorXd stress;
};

// 复制 MaterialState, 此时会创建新的内存地址
void f1(MaterialState state)
{
    std::cout << "&state = "
              << &state << '\n';

    std::cout << "stress.data() = "
              << state.stress.data() << '\n';

    state.damage = 0.9;
}

// 避免 MaterialState 复制, 但会产生修改成员变量的风险
void f2(MaterialState &state)
{
    std::cout << "&state = "
              << &state << '\n';

    std::cout << "stress.data() = "
              << state.stress.data() << '\n';

    state.damage = 0.9;
}

// 1. 避免 MaterialState 复制
// 2. 防止当前函数修改 state
void f3(MaterialState const &state)
{
    // const 引用, 此时不能修改成员变量, 只能读取成员变量
    // state.damage = 0.9;

    std::cout << state.damage << '\n';
}

// 仅复制 pointer 内存
void f4(MaterialState *state)
{
    // 防止传入空指针
    if(!state)
    {
        std::cout << "Empty Pointer!" << '\n';
        
        return;
    }

    std::cout << "&state = "
              << state << '\n';

    std::cout << "stress.data() = "
              << state->stress.data() << '\n';


    state->damage = 10.0;
}

int main()
{
    MaterialState s1{
        0.1,
        0.02,
        Eigen::VectorXd::Ones(6)};

    std::cout << "&s1 main = "
          << &s1 << '\n';

    std::cout << "stress.data() main = "
            << s1.stress.data() << '\n';

    std::cout << "========= Function ==========" << '\n';

    f1(s1);

    std::cout << s1.damage << '\n';

    std::cout << "========= Reference ==========" << '\n';

    f2(s1);

    std::cout << s1.damage << '\n';

    std::cout << "========= Pointer ==========" << '\n';

    f4(&s1);

    std::cout << s1.damage << '\n';

    f4(nullptr);
}
#include <Eigen/Dense>
#include <cmath>
#include <iostream>
#include <stdexcept>

struct MaterialState
{
    double damage = 0.1;
    double kappa = 0.2;
    Eigen::VectorXd stress = Eigen::VectorXd::Ones(3);
};

void printInfo(MaterialState state)
{
    std::cout << "origin-value: " << state.damage << ' ' << state.kappa
              << " [" << state.stress.transpose() << "]\n";
}

void updateByValue(MaterialState state, double delta)
{
    state.damage += delta;
    state.kappa += 2.0 * delta;
    state.stress *= 2.0;

    std::cout << "local-value: " << state.damage << ' ' << state.kappa
              << " [" << state.stress.transpose() << "]\n";
}

void updateByReference(MaterialState& state, double delta)
{
    state.damage += delta;
    state.kappa += 2.0 * delta;
    state.stress *= 2.0;
}

MaterialState updateByConstReference(MaterialState const& state, double delta)
{
    // 显式复制
    MaterialState res = state;

    res.damage += delta;
    res.kappa += 2.0 * delta;
    res.stress *= 2.0;

    return res;
}

bool updateByPointer(MaterialState* state, double delta)
{
    if(!state) {
        return false;
    }

    state->damage += delta;
    state->kappa += 2.0 * delta;
    state->stress *= 2.0;

    return true;
}

int main()
{
    {
        double delta = 0.1;
        MaterialState s1;

        // 按值传递, 不会改变原对象
        updateByValue(s1, delta);
        printInfo(s1);
    }

    std::cout << "=====================" << '\n';

    {
        double delta = 0.1;
        MaterialState s1;

        // 引用, 会修改原对象
        updateByReference(s1, delta);
        printInfo(s1);
    }

    std::cout << "=====================" << '\n';

    {
        double delta = 0.1;
        MaterialState s1;
        printInfo(s1);

        MaterialState res = updateByConstReference(s1, delta);
        printInfo(s1);
        printInfo(res);
    }

    std::cout << "=====================" << '\n';

    {
        double delta = 0.1;
        MaterialState s1;
        printInfo(s1);

        bool changed = updateByPointer(&s1, delta);
        printInfo(s1);
        std::cout << changed << '\n';
    }

    std::cout << "=====================" << '\n';

    {
        double delta = 0.1;
        MaterialState s1;

        bool unchanged = updateByPointer(nullptr, delta);
        printInfo(s1);
        std::cout << unchanged << '\n';
    }

    std::cout << "=====================" << '\n';

    {
        MaterialState original;
        MaterialState copy = original;

        copy.stress[0] = 0.99;

        printInfo(original);
        printInfo(copy);
    }
}
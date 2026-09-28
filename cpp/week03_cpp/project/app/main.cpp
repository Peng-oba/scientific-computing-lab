#include "lab/material.hpp"

#include <iostream>

void runCase()
{
    lab::MaterialState state;

    const double modulus = 1000.0;
    const double total_strain = 0.01;

    lab::updateStress(state, modulus, total_strain);
    std::cout << "first=" << state.stress << '\n';
    
    lab::updateStress(state, modulus, total_strain);
    std::cout << "second=" << state.stress << '\n';
}

int main()
{
    runCase();
    return 0;
}

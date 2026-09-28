#include "lab/material.hpp"

#include <cmath>
#include <stdexcept>

namespace lab
{
void updateStress(MaterialState& state, double modulus, double strain)
{
    if (!std::isfinite(modulus) || modulus <= 0.0 || !std::isfinite(strain))
    {
        throw std::invalid_argument("modulus must be finite and positive; strain must be finite");
    }

    state.stress = modulus * strain;
}
}  // namespace lab

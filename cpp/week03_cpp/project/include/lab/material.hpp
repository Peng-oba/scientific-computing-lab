#pragma once

namespace lab {
struct MaterialState {
    double stress = 0.0;
};

// Strain is TOTAL strain, not an increment. E > 0; inputs must be finite.
// Invalid input throws std::invalid_argument without changing state.
// The teaching examples use values for which E * strain is representable.
void updateStress(MaterialState& state, double modulus, double strain);
}  // namespace lab

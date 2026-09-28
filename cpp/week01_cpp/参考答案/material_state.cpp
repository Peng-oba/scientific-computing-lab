#include <Eigen/Dense>
#include <cmath>
#include <iostream>
#include <stdexcept>

// Pedagogical values only. This is not a constitutive model.
struct MaterialState {
    double damage = 0.1;
    double kappa = 0.2;
    Eigen::VectorXd stress = Eigen::VectorXd::Ones(3);
};

bool near(double a, double b) { return std::abs(a - b) < 1e-12; }
bool matches(MaterialState const& s, double damage, double kappa, double stress) {
    return near(s.damage, damage) && near(s.kappa, kappa)
        && s.stress.size() == 3
        && near(s.stress[0], stress) && near(s.stress[1], stress)
        && near(s.stress[2], stress);
}

void updateByValue(MaterialState state, double delta) {
    state.damage += delta;
    state.kappa += 2.0 * delta;
    state.stress *= 2.0;
    std::cout << "local-value: " << state.damage << ' ' << state.kappa
              << " [" << state.stress.transpose() << "]\n";
}

void updateByReference(MaterialState& state, double delta) {
    state.damage += delta;
    state.kappa += 2.0 * delta;
    state.stress *= 2.0;
}

MaterialState updateByConstReference(MaterialState const& state, double delta) {
    MaterialState result = state;
    result.damage += delta;
    result.kappa += 2.0 * delta;
    result.stress *= 2.0;
    return result;
}

bool updateByPointer(MaterialState* state, double delta) {
    if (state == nullptr) { return false; }
    state->damage += delta;
    state->kappa += 2.0 * delta;
    state->stress *= 2.0;
    return true;
}

int check(bool condition, char const* name) {
    std::cout << name << ": " << (condition ? "PASS" : "FAIL") << '\n';
    return condition ? 0 : 1;
}

int main() {
    int failures = 0;
    {
        MaterialState s;
        updateByValue(s, 0.1);
        failures += check(matches(s, 0.1, 0.2, 1.0), "M1-original-unchanged");
    }
    {
        MaterialState s;
        updateByReference(s, 0.1);
        failures += check(matches(s, 0.2, 0.4, 2.0), "M2-reference");
    }
    {
        MaterialState s;
        MaterialState next = updateByConstReference(s, 0.1);
        failures += check(matches(s, 0.1, 0.2, 1.0) && matches(next, 0.2, 0.4, 2.0),
                          "M3-const-input-and-new-result");
    }
    {
        MaterialState s;
        bool const changed = updateByPointer(&s, 0.1);
        failures += check(changed && matches(s, 0.2, 0.4, 2.0), "M4-valid-pointer");
    }
    {
        failures += check(!updateByPointer(nullptr, 0.1), "M5-null-pointer");
    }
    {
        MaterialState original;
        MaterialState copy = original;
        copy.stress[0] = 9.0;
        failures += check(matches(original, 0.1, 0.2, 1.0) && near(copy.stress[0], 9.0),
                          "M6-independent-vector-copy");
    }
    {
        MaterialState accepted;
        MaterialState trial = accepted;
        updateByReference(trial, 0.1);
        failures += check(matches(accepted, 0.1, 0.2, 1.0) && matches(trial, 0.2, 0.4, 2.0),
                          "application-independent-trial");
    }
    {
        MaterialState accepted;
        MaterialState& trial = accepted;
        updateByReference(trial, 0.1);
        failures += check(matches(accepted, 0.2, 0.4, 2.0), "application-alias-changes-accepted");
    }
    return failures == 0 ? 0 : 1;
}

#include "lab/material.hpp"

#include <gtest/gtest.h>
#include <stdexcept>

TEST(MaterialStress, NormalInput)
{
    lab::MaterialState state;
    lab::updateStress(state, 1000.0, 0.01);
    EXPECT_NEAR(state.stress, 10.0, 1e-12);
}

TEST(MaterialStress, ZeroStrainClearsOldValue)
{
    lab::MaterialState state{7.0};
    lab::updateStress(state, 1000.0, 0.0);
    EXPECT_NEAR(state.stress, 0.0, 1e-12);
}

TEST(MaterialStress, InvalidInputPreservesState)
{
    lab::MaterialState state{7.0};
    EXPECT_THROW(lab::updateStress(state, -1000.0, 0.01), std::invalid_argument);
    EXPECT_NEAR(state.stress, 7.0, 1e-12);
}

TEST(MaterialStress, RepeatedTotalStrainDoesNotAccumulate)
{
    lab::MaterialState state;
    lab::updateStress(state, 1000.0, 0.01);
    lab::updateStress(state, 1000.0, 0.01);
    EXPECT_NEAR(state.stress, 10.0, 1e-12);
}

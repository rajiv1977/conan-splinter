#include <array>
#include <iostream>
#include <random>
#include <vector>

#include "bsplinebuilder.h"

int main(int argc, char* argv[])
{

    SPLINTER::DataTable samples;
    int                 degFreedom = 5;

    std::random_device                    rseed;
    std::mt19937                          rng(rseed());
    std::uniform_real_distribution<float> dist(-2000, 2000);
    std::uniform_real_distribution<float> elevation(0, rand());

    std::vector<std::array<float, 2>> originalInput         = {};
    std::vector<std::array<float, 2>> bsplineFormattedInput = {};

    for (size_t index = 0; index < 1000; index++)
    {
        auto randomInput = dist(rng);
        samples.addSample(static_cast<float>(index + 1), randomInput);
        std::array<float, 2> internalTemp = {static_cast<float>(index + 1), randomInput};
        originalInput.push_back(internalTemp);
    }

    if (samples.getNumSamples() > degFreedom)
    {
        auto pspline = SPLINTER::BSpline::Builder(samples)
                           .alpha(0.3)
                           .degree(degFreedom)
                           .knotSpacing(SPLINTER::BSpline::KnotSpacing::AS_SAMPLED)
                           .smoothing(SPLINTER::BSpline::Smoothing::PSPLINE)
                           .build();

        auto knotVectors = pspline.getKnotVectors();

        for (int it = 0; it < knotVectors[0].size(); it++)
        {
            SPLINTER::DenseVector x(1);
            x(0)                              = knotVectors[0].at(it);
            auto                 y            = pspline.eval(x);
            std::array<float, 2> internalTemp = {static_cast<float>(x(0)), static_cast<float>(y)};
            bsplineFormattedInput.push_back(internalTemp);
        }
        knotVectors.clear();

        std::cout << "test done" << std::endl;
    }

    return 0;
}

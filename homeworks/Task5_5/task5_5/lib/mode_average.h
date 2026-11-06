#pragma once

#include <random>
#include <type_traits>

namespace modified {

    template <typename T>
    T mode_average(const T& a, const T& b) {
        static std::mt19937 gen(std::random_device{}());

        T result = (a + b) / static_cast<T>(2);

        if (std::bernoulli_distribution(0.5)(gen)) {
            if constexpr (std::is_integral_v<T>) {
                result += static_cast<T>(
                    std::uniform_int_distribution<int>(-10, 10)(gen)
                );
            } else {
                result += static_cast<T>(
                    std::uniform_real_distribution<double>(-10.0, 10.0)(gen)
                );
            }
        }
        return result;
    }

}
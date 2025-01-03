#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>

#define M_PI 3.14159265358979323846

namespace mymath
{
    float	radians(float degrees);

    float	dregrees(float radians);

    template <typename T>
    T clamp(T value, T min, T max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    template <typename T>
    T mod(T value, T divisor) {
        return value - divisor * static_cast<int>(value / divisor);
    }

    template <typename T>
    T two_pi() {
        return static_cast<T>(2.0 * M_PI);
    }
}

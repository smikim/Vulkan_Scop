#include "math.h"

namespace mymath
{
    float radians(float degrees)
    {
        std::cout << "M_PI : " << M_PI << std::endl;

        return degrees * M_PI / 180;
    }

    float dregrees(float radians)
    {
        return radians * 180 / M_PI;
    }
}




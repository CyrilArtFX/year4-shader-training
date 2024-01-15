#pragma once

namespace Random
{
    static inline float randomFloat()
    {
        static unsigned int seed = 0x13371337;

        float res;
        unsigned int tmp;

        seed *= 16807;
        tmp = seed ^ (seed >> 4) ^ (seed << 15);
        *((unsigned int*)&res) = (tmp >> 9) | 0x3F800000;

        return (res - 1.0f);
    }
}
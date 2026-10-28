#pragma once
#include "Vec3.h"


enum class MaterialType {
    Lambertian,
    Mirror,
    Transparent,
    Emissive
};

struct Material {
    MaterialType type = MaterialType::Lambertian;

    //reflection per color channel (RGB)
    Vec3 albedo = { 0.8, 0.8, 0.8 };
    //emission per color channel (RGB)
    Vec3 emission = { 0.0, 0.0, 0.0 };

    //index of refraction for transparent materials
    double ior = 1.0;
};

namespace Materials {

    inline constexpr Material white{
        MaterialType::Lambertian,
        {0.8, 0.8, 0.8},
        {0, 0, 0},
        1.0
    };

    inline constexpr Material red{
        MaterialType::Lambertian,
        {0.8, 0.1, 0.1},
        {0, 0, 0},
        1.0
    };

    inline constexpr Material green{
        MaterialType::Lambertian,
        {0.1, 0.8, 0.1},
        {0, 0, 0},
        1.0
    };

    inline constexpr Material blue{
        MaterialType::Lambertian,
        {0.1, 0.1, 0.8},
        {0, 0, 0},
        1.0
    };

    inline constexpr Material mirror{
        MaterialType::Mirror,
        {1, 1, 1},
        {0, 0, 0},
        1.0
    };

    inline constexpr Material glass{
        MaterialType::Transparent,
        {1, 1, 1},
        {0, 0, 0},
        1.5
    };

    inline constexpr Material light{
        MaterialType::Emissive,
        {0, 0, 0},
        {10, 10, 10},
        1.0
    };
}


#include <array>
#include "Vec3.h"
#include "Material.h"

#pragma once

//create a struct for all triangles with their properties
struct Triangle {
	std::array<Vec3, 3> vertices;
	Vec3 normal;
	Material material;
};
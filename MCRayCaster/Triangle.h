#include <string>
#include <array>
#include "Vec3.h"
#pragma once

//create a struct for all triangles with their properties
struct Triangle {
	std::array<Vec3, 3> vertices;
	Vec3 normal;
	std::array<double, 3> color;
	std::string material = "Opaque";

};
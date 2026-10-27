#pragma once

#include "Vec3.h"
#include "Material.h"

struct Sphere {
	Vec3 center;
	double radius;
	Material material;
};
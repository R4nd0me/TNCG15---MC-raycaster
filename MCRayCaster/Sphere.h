#pragma once

#include "Vec3.h"
#include "Mat.h"

struct Sphere {
	Vec3 center;
	double radius;
	Material material;
};
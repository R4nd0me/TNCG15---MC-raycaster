#pragma once

#include "Vec3.h"
#include "Material.h"

struct HitRecord {
	double t;
	Vec3 pos;
	Vec3 normal;
	bool front_face;
	Material material;
};
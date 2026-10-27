#pragma once
#include "Vec3.h"

struct Camera {
	Vec3 position;
	Vec3 forward;
	Vec3 right;
	Vec3 up;

	double fov = 90.0;
};
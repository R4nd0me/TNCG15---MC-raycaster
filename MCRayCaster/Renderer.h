#pragma once

#include "Triangle.h"
#include "Sphere.h"
#include "Camera.h"

#include <vector>

void render(const std::vector<Triangle>& triangles, 
	const std::vector<Sphere>& spheres, 
	const Camera& camera, 
	int width, int height);

inline Vec3 subtraction(const Vec3& a, const Vec3& b);
Vec3 normalize(const Vec3& v);
inline Vec3 cross(const Vec3& a, const Vec3& b);
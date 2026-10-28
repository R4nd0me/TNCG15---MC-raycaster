#pragma once

#include "Triangle.h"
#include "Sphere.h"
#include "Camera.h"
#include "HitRecord.h"
#include "Ray.h"

#include <vector>
#include <optional>

std::optional<HitRecord> hit_scene(const Ray& ray, 
	const std::vector<Triangle>& triangles, 
	const std::vector<Sphere>& spheres);

void render(const std::vector<Triangle>& triangles, 
	const std::vector<Sphere>& spheres, 
	const Camera& camera, 
	int width, int height,
	int samples_per_pixel);
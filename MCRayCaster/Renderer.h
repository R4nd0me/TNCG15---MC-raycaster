#pragma once

#include "Triangle.h"
#include "Camera.h"
#include <vector>

void render(const std::vector<Triangle>& triangles, const Camera& camera, int width, int height);
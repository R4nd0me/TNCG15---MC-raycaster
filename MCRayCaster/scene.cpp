
#include "Vec3.h"
#include "Triangle.h"
#include "MCRayTracer.h"
#include "Renderer.h"
#include "Camera.h"

#include <vector>
#include <array>
#include <iostream>



//create the room by returning a vector with all triangles and their propeties
std::vector<Triangle> create_room() {
	std::vector<Triangle> triangles;

	//divide the square into two triangles and add them to the vector
	auto divide_square = [&](Vec3 a, Vec3 b, Vec3 c, Vec3 d,
		Vec3 normal, std::array<double, 3> color) {
			triangles.push_back({ {a, b, c}, normal, color, "Opaque" });
			triangles.push_back({ {a, c, d}, normal, color, "Opaque" });
		};

	//floor
	divide_square({ 0, 0, 0 }, { 4, 0, 0 }, { 4, 4, 0 }, { 0, 4, 0 },
		{ 0, 0, 1 }, { 0.8, 0.8, 0.8 });

	//roof
	divide_square({ 0, 0, 4 }, { 0, 4, 4 }, { 4, 4, 4 }, { 4, 0, 4 },
		{ 0, 0, -1 }, { 0.9, 0.9, 0.9 });

	//left wall
	divide_square({ 0, 0, 0 }, { 0, 4, 0 }, { 0, 4, 4 }, { 0, 0, 4 },
		{ 1, 0, 0 }, { 1, 0, 0 });

	//right wall
	divide_square({ 4, 0, 0 }, { 4, 0, 4 }, { 4, 4, 4 }, { 4, 4, 0 },
		{ -1, 0, 0 }, { 0, 1, 0 });

	//front wall
	divide_square({ 0, 0, 0 }, { 0, 0, 4 }, { 4, 0, 4 }, { 4, 0, 0 },
		{ 0, 1, 0 }, { 0, 0, 1 });

	//back wall
	divide_square({ 0, 4, 0 }, { 4, 4, 0 }, { 4, 4, 4 }, { 0, 4, 4 },
		{ 0, -1, 0 }, { 1, 1, 0 });

	return triangles;
}


int main() {
	std::vector<Triangle> triangles = create_room();

	Camera camera{
		{0.5, 2.0, 2.0}, //position
		{1.0, 0.0, 0.0}, //forward
		{0.0, 1.0, 0.0}, //right
		{0.0, 0.0, 1.0}, //up
		90.0             //field of view
	};

	render(triangles, camera, 200, 200);

	return 0;
}


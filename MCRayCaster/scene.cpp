
#include "Vec3.h"
#include "Triangle.h"
#include "MCRayTracer.h"
#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "Sphere.h"

#include <vector>
#include <array>
#include <iostream>



//create the room by returning a vector with all triangles and their propeties
std::vector<Triangle> create_room() {
	std::vector<Triangle> triangles;

	//divide the square into two triangles and add them to the vector
	auto divide_square = [&](Vec3 a, Vec3 b, Vec3 c, Vec3 d,
		Vec3 normal, const Material& material) {
			triangles.push_back({ {a, b, c}, normal, material });
			triangles.push_back({ {a, c, d}, normal, material });
		};

	//floor
	divide_square({ 0, 0, 0 }, { 4, 0, 0 }, { 4, 4, 0 }, { 0, 4, 0 },
		{ 0, 0, 1 }, Materials::white);

	//roof
	divide_square({ 0, 0, 4 }, { 0, 4, 4 }, { 4, 4, 4 }, { 4, 0, 4 },
		{ 0, 0, -1 }, Materials::white);

	//left wall
	divide_square({ 0, 0, 0 }, { 0, 4, 0 }, { 0, 4, 4 }, { 0, 0, 4 },
		{ 1, 0, 0 }, Materials::red);

	//right wall
	divide_square({ 4, 0, 0 }, { 4, 0, 4 }, { 4, 4, 4 }, { 4, 4, 0 },
		{ -1, 0, 0 }, Materials::green);

	//front wall
	divide_square({ 0, 0, 0 }, { 0, 0, 4 }, { 4, 0, 4 }, { 4, 0, 0 },
		{ 0, 1, 0 }, Materials::white);

	//back wall
	divide_square({ 0, 4, 0 }, { 4, 4, 0 }, { 4, 4, 4 }, { 0, 4, 4 },
		{ 0, -1, 0 }, Materials::blue);

	//light
	divide_square({ 1.5, 1.5, 3.99 }, { 1.5, 2.5, 3.99 }, { 2.5, 2.5, 3.99 }, { 2.5, 1.5, 3.99 }, 
		{ 0, 0, -1 }, Materials::light);

	return triangles;
}

std::vector<Triangle> create_tetrahedon() {
	std::vector<Triangle> triangles;

	Vec3 a = { 2.75, 1.55, 0.05 };
	Vec3 b = { 3.40, 1.55, 0.05 };
	Vec3 c = { 3.075, 2.20, 0.05 };
	Vec3 d = { 3.075, 1.875, 1.10 };

	auto add_triangle = [&](Vec3 p1, Vec3 p2, Vec3 p3) {
		Vec3 edge1 = subtraction(p1, p2);
		Vec3 edge2 = subtraction(p3, p1);

		Vec3 normal = normalize(cross(edge1, edge2));

		triangles.push_back({ {p1, p2, p3}, normal, Materials::red });
	};

	add_triangle(a, b, c); //bottom
	add_triangle(a, b, d); //side1
	add_triangle(b, c, d); //side2
	add_triangle(c, a, d); //side3

	return triangles;
}

std::vector<Sphere> create_spheres() {
	std::vector<Sphere> spheres;

	spheres.push_back({ {2.0, 1.3, 0.6}, 0.6, Materials::glass });

	spheres.push_back({ {2.4, 2.7, 0.6}, 0.6, Materials::mirror });

	return spheres;
}


int main() {
	std::vector<Triangle> triangles = create_room();
	std::vector<Triangle> tetrahedron = create_tetrahedon();
	triangles.insert(triangles.end(), tetrahedron.begin(), tetrahedron.end());
	std::vector<Sphere> spheres = create_spheres();

	Camera camera{
		{0.5, 2.0, 2.0}, //position
		{1.0, 0.0, 0.0}, //forward
		{0.0, 1.0, 0.0}, //right
		{0.0, 0.0, 1.0}, //up
		90.0             //field of view
	};

	render(triangles, spheres, camera, 400, 400);

	return 0;
}


#pragma once

#include "Vec3.h"
#include "Mat.h"

struct HitRecord {
	double t;			// istance along the ray to the hit point
	Vec3 position;			//position where the ray hits
	Vec3 normal;		//the normal of the surfaceat the hit point
	bool front_face;	//if the ray hits the front face of the surface
	Material material;	//the material of the object
};
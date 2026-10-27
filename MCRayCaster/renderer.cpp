#include "Renderer.h"
#include "Vec3.h"

#include <vector>
#include <array>
#include <limits>
#include <cmath>
#include <iostream>
#include <optional>
#include <numbers>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <string>



//cross product
inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

//dot product
inline double dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 subtraction(const Vec3& a, const Vec3& b) {
    return {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
}

Vec3 normalize(const Vec3& v) {
    double length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    if (length == 0.0) {
        return { 0, 0, 0 };
    }

    return { v.x / length, v.y / length, v.z / length };
}


std::optional<double> ray_triangle_intersect(const Vec3& orig, const Vec3& dir, const Vec3& v0, const Vec3& v1, const Vec3& v2) {

    double eps = 1e-8;

    Vec3 edge1 = subtraction(v1, v0);
    Vec3 edge2 = subtraction(v2, v0);

    Vec3 h = cross(dir, edge2);
    double a = dot(edge1, h);

    if (a > -eps && a < eps) {
        return std::nullopt;
    }

    double f = 1.0 / a;
    Vec3 s = subtraction(orig, v0);
    double u = f * dot(s, h);

    if (u < 0.0 || u > 1.0) {
        return std::nullopt;
    }

    Vec3 q = cross(s, edge1);
    double v = f * dot(dir, q);

    if (v < 0.0 || (u + v) > 1.0) {
        return std::nullopt;
    }

    double t = f * dot(edge2, q);

    if (t > eps) {
        return t;
    }
    return std::nullopt;
}

void write_color(std::ostream& out, const std::array<double, 3>& color) {
    for (double c : color) {
        int value = static_cast<int>(255.999 * std::clamp(c, 0.0, 1.0));
        out << value << ' ';
    }
    out << '\n';
}




void render(const std::vector<Triangle>& triangles, const Camera& camera, int width, int height) {

    //image proportions, ratio between height and width as a double
    double aspect = static_cast<double>(width) / height;

    std::ofstream image("rendered_room.ppm");
    if (!image) {
        throw std::runtime_error("Could not create image");
    }

    image << "P3\n" << width << ' ' << height << "\n255\n";

    //go through every pixel (x,y)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            //calculate the pixel's location in aspect to the center of the picture
            double u = (2.0 * (x + 0.5) / width - 1.0) * aspect;
            double v = 1.0 - 2.0 * (y + 0.5) / height;

            //convert field of view to a scale
            double scale = std::tan(camera.fov * std::numbers::pi / 360.0);

            //calculate ray direction
            Vec3 direction = normalize({
                camera.forward.x + u * scale * camera.right.x + v * scale * camera.up.x,
                camera.forward.y + u * scale * camera.right.y + v * scale * camera.up.y,
                camera.forward.z + u * scale * camera.right.z + v * scale * camera.up.z
            });

            //find nearest triangle
            double nearest_t = std::numeric_limits<double>::infinity();
            std::array<double, 3> color = { 0, 0, 0 };

            for (const Triangle& tri : triangles) {
                //backface culling
                if (dot(direction, tri.normal) >= 0) {
                    continue;
                }

                auto t = ray_triangle_intersect (camera.position, direction, tri.vertices[0], tri.vertices[1], tri.vertices[2]);

                if (t.has_value() && t.value() < nearest_t) {
                    nearest_t = t.value();
                    color = tri.color;
                }
            }

            //save pixel color
            write_color(image, color);

        }
    }


}
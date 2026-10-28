#include "Renderer.h"
#include "Ray.h"
#include "HitRecord.h"

#include <random>
#include <numbers>
#include <optional>
#include <cmath>
#include <algorithm>
#include <limits>
#include <fstream>
#include <iostream>
#include <stdexcept>

std::optional<double> ray_triangle_intersect(const Vec3& orig, const Vec3& dir, const Vec3& v0, const Vec3& v1, const Vec3& v2) {

    double eps = 1e-8;

    Vec3 edge1 = subtraction(v1, v0);
    Vec3 edge2 = subtraction(v2, v0);

    Vec3 h = cross(dir, edge2);
    double a = dot(edge1, h);

    if (std::abs(a) < eps) {
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
    return t > 1e-6 ? std::optional<double>{t} : std::nullopt;
}



std::optional<double> ray_sphere_intersect(const Vec3& origin, const Vec3& direction, const Sphere& sphere) {

    Vec3 oc = subtraction(origin, sphere.center);

    double a = dot(direction, direction);
    double half_b = dot(oc, direction);
    double c = dot(oc, oc) - sphere.radius * sphere.radius;

    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0.0) {
        return std::nullopt;
    }

    double sqrt_d = std::sqrt(discriminant);

    double t = (-half_b - sqrt_d) / a;

    if (t <= 1e-6) {
        t = (-half_b + sqrt_d) / a;
    }

    if (t <= 1e-6) {
        return std::nullopt;
    }

    return t;
}


Vec3 point_at(const Ray& ray, double t) {
    return add_vec(ray.origin, scale_vec(ray.direction, t));
}


std::optional<HitRecord> hit_scene(const Ray& ray, const std::vector<Triangle>& triangles, const std::vector<Sphere>& spheres) {

    std::optional<HitRecord> closest_hit;
    double nearest_t = std::numeric_limits<double>::infinity();

    for (const Triangle& tri : triangles) {
        auto t = ray_triangle_intersect(ray.origin, ray.direction, tri.vertices[0], tri.vertices[1], tri.vertices[2]);

        if (!t || *t >= nearest_t) {
            continue;
        }

        Vec3 outward_normal = normalize(tri.normal);
        bool front_face = dot(ray.direction, outward_normal) < 0.0;
        Vec3 normal = front_face ? outward_normal : scale_vec(outward_normal, -1.0);

        closest_hit = HitRecord{ *t, point_at(ray, *t), normal, front_face, tri.material };
        nearest_t = *t;
    }
    for (const Sphere& sphere : spheres) {
        auto t = ray_sphere_intersect(ray.origin, ray.direction, sphere);

        if (!t || *t >= nearest_t) {
            continue;
        }

        Vec3 pos = point_at(ray, *t);
        Vec3 outward_normal = normalize(subtraction(pos, sphere.center));
        bool front_face = dot(ray.direction, outward_normal) < 0.0;
        Vec3 normal = front_face ? outward_normal : scale_vec(outward_normal, -1.0);

        closest_hit = HitRecord{ *t, pos, normal, front_face, sphere.material };
        nearest_t = *t;
    }
    return closest_hit;    
}


Vec3 reflect(const Vec3& direction, const Vec3& normal) {
    return subtraction(direction, scale_vec(normal, 2.0 * dot(direction, normal)));
}

double reflectance(double cosine, double eta) {
    double r0 = (1.0 - eta) / (1.0 + eta);
    r0 *= r0;

    return r0 + (1.0 - r0) * std::pow(1.0 - cosine, 5.0);
}



Vec3 trace_path(const Ray& ray, const std::vector<Triangle>& triangles, const std::vector<Sphere>& spheres, int depth, std::mt19937& rng) {
    constexpr int max_depth = 8;
    constexpr double eps = 1e-4;

    if (depth >= max_depth) {
        return { 0, 0, 0 };
    }

    auto hit = hit_scene(ray, triangles, spheres);

    if (!hit) {
        return { 0, 0, 0 };
    }

    const Material& material = hit->material;

    if (material.type == MaterialType::Emissive) {
        return material.emission;
    }

    std::uniform_real_distribution<double> random(0.0, 1.0);

    Vec3 new_direction{};

    if (material.type == MaterialType::Lambertian) {
        double phi = 2.0 * std::numbers::pi * random(rng);
        double r2 = std::sqrt(random(rng));
        double radius = std::sqrt(r2);
        double z = std::sqrt(1.0 - r2);

        Vec3 w = hit->normal;

        Vec3 helper = std::abs(w.x) > 0.9 ? Vec3{ 0, 1, 0 } : Vec3{ 1, 0, 0 };

        Vec3 u = normalize(cross(helper, w));
        Vec3 v = cross(w, u);

        new_direction = normalize(add_vec(add_vec(scale_vec(u, radius * std::cos(phi)), scale_vec(v, radius * std::sin(phi))), scale_vec(w, z)));
    }

    else if (material.type == MaterialType::Mirror) {

        new_direction = normalize(reflect(ray.direction, hit->normal));
    }

    else if (material.type == MaterialType::Transparent) {

        const double eta = hit->front_face ? 1.0 / material.ior : material.ior;
        const double cos_theta = std::clamp(-dot(ray.direction, hit->normal), 0.0, 1.0);

        const double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
        const bool cannot_refract = eta * sin_theta > 1.0;

        if (cannot_refract || random(rng) < reflectance(cos_theta, eta)) {

            new_direction = normalize(reflect(ray.direction, hit->normal));
        }
        else {
            Vec3 perpendicular = scale_vec(add_vec(ray.direction, scale_vec(hit->normal, cos_theta)), eta);

            Vec3 parallel = scale_vec(hit->normal, -std::sqrt(std::max(0.0, 1.0 - dot(perpendicular, perpendicular))));

            new_direction = normalize(add_vec(perpendicular, parallel));
        }
    }

    else {
        return { 0, 0, 0 };
    }

    double offset = dot(new_direction, hit->normal) > 0.0 ? eps : -eps;

    Ray next_ray{ add_vec(hit->position, scale_vec(hit->normal, offset)), new_direction};

    Vec3 incoming_light = trace_path(next_ray, triangles, spheres, depth + 1, rng);

    return multiply_vec(material.albedo, incoming_light);
}




void write_color(std::ostream& out, const std::array<double, 3>& color) {
    for (double c : color) {

        c = std::max(0.0, c);
        c = c / (1.0 + c);
        c = std::pow(c, 1.0 / 2.2);


        out << static_cast<int>(255.0 * std::clamp(c, 0.0, 1.0) + 0.5) << ' ';
    }
    out << '\n';
}




void render(const std::vector<Triangle>& triangles, const std::vector<Sphere>& spheres, const Camera& camera, int width, int height, int samples_per_pixel) {

    if (width <= 0 || height <= 0 || samples_per_pixel <= 0) {
        throw std::runtime_error("invalid image size");
	}

    std::ofstream image("rendered_room.ppm");
    if (!image) {
        throw std::runtime_error("Could not create image");
    }

    image << "P3\n" << width << ' ' << height << "\n255\n";

    //image proportions, ratio between height and width as a double
    double aspect = static_cast<double>(width) / height;

    //convert field of view to a scale
    double scale = std::tan(camera.fov * std::numbers::pi / 360.0);

    std::mt19937 rng(12345); 
    std::uniform_real_distribution<double> random(0.0, 1.0);


    std::cout << "MC render: " << width << 'x' << height
        << ", " << samples_per_pixel << " spp\n";
    for (int y = 0; y < height; ++y) {

        for (int x = 0; x < width; ++x) {

            Vec3 pixel_color{ 0,0,0 };

            for (int s = 0; s < samples_per_pixel; ++s) {

                double u = (2.0 * (x + random(rng)) / width - 1.0) * aspect;
                double v = 1.0 - 2.0 * (y + random(rng)) / height;

                Vec3 direction = normalize({
                    camera.forward.x + u * scale * camera.right.x + v * scale * camera.up.x,
                    camera.forward.y + u * scale * camera.right.y + v * scale * camera.up.y,
                    camera.forward.z + u * scale * camera.right.z + v * scale * camera.up.z
                    });

                Ray ray{ camera.position, direction };

                pixel_color = add_vec(pixel_color,trace_path(ray, triangles, spheres, 0, rng));
            }

            pixel_color = scale_vec(pixel_color, 1.0 / samples_per_pixel);
            write_color(image, { pixel_color.x,pixel_color.y,pixel_color.z });
        }
    }

    if (!image) {
        throw std::runtime_error("Could not write image");
    }

    std::cout << "Done: rendered_room.ppm\n";   
}
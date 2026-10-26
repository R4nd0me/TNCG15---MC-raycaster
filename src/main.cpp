#include <iostream>
#include <fstream>
#include "vec3.h"
#include "color.h"

int main()
{

    // Image
    int image_width = 256;
    int image_height = 256;

    // Open output file
    std::ofstream image("image.ppm");

    if (!image) {
        std::cerr << "Error: Could not create image.ppm\n";
        return 1;
    }

    // Render
    image << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: "
            << (image_height - j) << ' ' << std::flush;

        for (int i = 0; i < image_width; i++) {
            auto pixel_color = color(
                double(i) / (image_width - 1),
                double(j) / (image_height - 1),
                0.0
            );
            //write_color(std::cout, pixel_color);
        }
    }

    image.close();

    std::clog << "\rDone. Image saved to image.ppm\n";

    return 0;
}
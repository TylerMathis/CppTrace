#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "stdlib.h"
#include "stdint.h"

#include <string>

struct Vec3 {
    double x, y, z;
    Vec3(): x(0), y(0), z(0) {}
    Vec3(double e0, double e1, double e2)
        : x(e0), y(e1), z(e2) {}

    Vec3 operator-() const { return Vec3(-x, -y, -z); }
    Vec3 operator+(const Vec3 &o) const { return Vec3(x + o.x, y + o.y, z + o.z); }
    Vec3 operator-(const Vec3 &o) const { return Vec3(x - o.x, y - o.y, z - o.z); }
};

struct Image {
    const char *name;
    int width, height, comps;
    uint8_t *data;
    Image(std::string _name, int _width, int _height, int _comps) {
        name = _name.c_str();
        width = _width;
        height = _height;
        comps = _comps;
        data = (uint8_t *) calloc(comps * width * height, sizeof(uint8_t));
    }
    void setPixel(int row, int col, Vec3 c) {
        int base = (row * width + col) * comps;
        data[base + 0] = c.x;
        data[base + 1] = c.y;
        data[base + 2] = c.z;
    }
    void write() {
        stbi_write_png(name, width, height, comps, data, width * comps);
    }
};

int main() {
    Image image("test.png", 256, 256, 3);

    for (int j = 0; j < image.height; j++) {
        for (int i = 0; i < image.width; i++) {
            auto r = double(i) / (image.width-1);
            auto g = double(j) / (image.height-1);
            auto b = 0.25;

            uint8_t ir = std::round(255 * r);
            uint8_t ig = std::round(255 * g);
            uint8_t ib = std::round(255 * b);

            image.setPixel(j, i, Vec3(ir, ig, ib));
        }
    }

    image.write();
}

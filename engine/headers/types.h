#pragma once

#include <vector>
#include <array>
#include <set>
#include <unordered_map>
#include <random>
#include <cmath>

#include "constants.h"

using std::vector;
using std::array;
using std::set;
using std::unordered_map;
using std::pair;


struct forces_t {
    double f_x = 0.0;
    double f_y = 0.0;
    double t_z = 0.0;

    forces_t& operator+=(const forces_t& f2) {
        f_x += f2.f_x;
        f_y += f2.f_y;
        t_z += f2.t_z;
        return *this;
    }
};

struct xyz_t {
    double x;
    double y;
    double z;

    xyz_t operator*(double scale) {
        return {x * scale, y * scale, z * scale};
    }

    double length() {
        return sqrt(x * x + y * y + z * z);
    }
};

struct xy_t {
    double x;
    double y;

    xy_t(double x, double y) : x(x), y(y) {}

    explicit xy_t(const xyz_t& xyz) : x(xyz.x), y(xyz.y) {}

    xy_t operator*(double scale) {
        return {x * scale, y * scale};
    }

    double length() {
        return sqrt(x * x + y * y);
    }
};

typedef std::mt19937 generator_t;
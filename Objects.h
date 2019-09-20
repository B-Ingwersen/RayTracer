struct Objects_Vector {
    float x;
    float y;
    float z;

    float operator[](const int& rhs) {
        return ( (float*)&x )[rhs];
    }
    void operator=(const Objects_Vector& rhs) {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
    }
    Objects_Vector operator+(const Objects_Vector& rhs) {
        Objects_Vector returnVec = {
            x + rhs.x,
            y + rhs.y,
            z + rhs.z,
        };
        return returnVec;
    }
    Objects_Vector operator-(const Objects_Vector& rhs) {
        Objects_Vector returnVec = {
            x - rhs.x,
            y - rhs.y,
            z - rhs.z,
        };
        return returnVec;
    }
    Objects_Vector operator*(const float& rhs) {
        Objects_Vector returnVec = {
            x * rhs,
            y * rhs,
            z * rhs
        };
        return returnVec;
    }

    float abs() {
        return sqrt(x*x+y*y+z*z);
    }
    float dist(const Objects_Vector v) {
        Objects_Vector diff = {
            x - v.x,
            y - v.y,
            z - v.z
        };
        return diff.abs();
    }
    float dot(const Objects_Vector v) {
        return v.x * x + v.y * y + v.z * z;
    }
    float cosineOfAngleBetween(const Objects_Vector v) {
        return (x*v.x + y*v.y + z*v.z) / sqrt((x*x + y*y + z*z)*(v.x*v.x + v.y*v.y + v.z*v.z));
    }
};

struct Color {
    uint8_t r;
    uint8_t b;
    uint8_t g;
    uint8_t a;

    uint32_t u32() {
        return ((uint32_t)r << 16) + ((uint32_t)g << 8) + (uint32_t)b;
    }
    void operator+=(const Color& rhs) {
        r += rhs.r;
        b += rhs.b;
        g += rhs.g;
        a += rhs.a;
    }
    void operator/=(const int& rhs) {
        r /= rhs;
        b /= rhs;
        g /= rhs;
        a /= rhs;
    }
};

struct Color_Float {
    float r;
    float b;
    float g;
    float a;
};

struct Objects_Ray {
    Objects_Vector point;
    Objects_Vector direction;

    void createFromPoints(Objects_Vector * start, Objects_Vector * through) {
        point = *start;
        direction = *through - *start;
    }
};

struct Objects_Sphere {
    Objects_Vector center;
    float r;
};

struct Objects_Plane {
    Objects_Vector normalVector;
    float d;
};

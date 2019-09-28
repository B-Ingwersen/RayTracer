struct RTTypesVector {
    float x;
    float y;
    float z;

    void operator=(const RTTypesVector& rhs) {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
    }
    RTTypesVector operator+(const RTTypesVector& rhs) {
        RTTypesVector returnVec = {
            x + rhs.x,
            y + rhs.y,
            z + rhs.z,
        };
        return returnVec;
    }
    RTTypesVector operator-(const RTTypesVector& rhs) {
        RTTypesVector returnVec = {
            x - rhs.x,
            y - rhs.y,
            z - rhs.z,
        };
        return returnVec;
    }
    RTTypesVector operator*(const float& rhs) {
        RTTypesVector returnVec = {
            x * rhs,
            y * rhs,
            z * rhs
        };
        return returnVec;
    }
    void operator+=(const RTTypesVector& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
    }

    float abs() {
        return sqrt(x*x+y*y+z*z);
    }
    float dist(const RTTypesVector v) {
        RTTypesVector diff = {
            x - v.x,
            y - v.y,
            z - v.z
        };
        return diff.abs();
    }
    float dot(const RTTypesVector v) {
        return v.x * x + v.y * y + v.z * z;
    }
    float cosineOfAngleBetween(const RTTypesVector v) {
        return (x*v.x + y*v.y + z*v.z) / sqrt((x*x + y*y + z*z)*(v.x*v.x + v.y*v.y + v.z*v.z));
    }
    float angleBetween(const RTTypesVector v) {
        return acos( cosineOfAngleBetween(v) );
    }
    void invert() {
        x = -x;
        y = -y;
        z = -z;
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

struct Color_Int {
    int r;
    int g;
    int b;
    int a;

    Color_Int operator+(const Color_Int& rhs) {
        Color_Int returnValue = {
            r + rhs.r,
            g + rhs.g,
            b + rhs.b,
            a + rhs.a
        };
        return returnValue;
    }
    Color_Int operator*(const int& rhs) {
        Color_Int returnValue = {
            r * rhs,
            g * rhs,
            b * rhs,
            a * rhs
        };
        return returnValue;
    }
    Color_Int operator/(const int& rhs) {
        Color_Int returnValue = {
            r / rhs,
            g / rhs,
            b / rhs,
            a / rhs
        };
        return returnValue;
    }

    uint32_t toUint32() {
        uint32_t r_ret = r; if (r_ret > 255) {r_ret = 255;}
        uint32_t g_ret = g; if (g_ret > 255) {g_ret = 255;}
        uint32_t b_ret = b; if (b_ret > 255) {b_ret = 255;}

        return (r_ret) + (g_ret << 8) + (b_ret << 16);
    }

    void fromUint32(uint32_t color) {
        r = color & 0xFF;
        g = (color >> 8) & 0xFF;
        b = (color >> 16) & 0xFF;
    }
};

struct Color_Float {
    float r;
    float b;
    float g;
    float a;

    void operator*=(const float& rhs) {
        r *= rhs;
        b *= rhs;
        g *= rhs;
        a *= rhs;
    }
};

struct RTTypesRay {
    RTTypesVector point;
    RTTypesVector direction;

    void createFromPoints(RTTypesVector * start, RTTypesVector * through) {
        point = *start;
        direction = *through - *start;
    }
};

struct RTTypesSphere {
    RTTypesVector center;
    float r;
};

struct RTTypesPlane {
    RTTypesVector normalVector;
    float d;
};


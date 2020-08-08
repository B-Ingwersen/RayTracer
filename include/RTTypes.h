
#ifndef _RT_TYPES___H_
#define _RT_TYPES___H_

#include <cstdint>
#include <math.h>

struct RTTypesVector {
    float x;
    float y;
    float z;

    void operator=(const RTTypesVector& rhs);
    RTTypesVector operator+(const RTTypesVector& rhs);
    RTTypesVector operator-(const RTTypesVector& rhs);
    RTTypesVector operator*(const float& rhs);
    void operator+=(const RTTypesVector& rhs);

    float abs();
    float dist(const RTTypesVector v);
    float dot(const RTTypesVector v);
    float cosineOfAngleBetween(const RTTypesVector v);
    float angleBetween(const RTTypesVector v);
    void invert();
};

struct Color {
    uint8_t r;
    uint8_t b;
    uint8_t g;
    uint8_t a;

    uint32_t u32();
    void operator+=(const Color& rhs);
    void operator/=(const int& rhs);
};

struct Color_Int {
    int r;
    int g;
    int b;
    int a;

    Color_Int operator+(const Color_Int& rhs);
    Color_Int operator*(const int& rhs);
    Color_Int operator/(const int& rhs);
    void operator+=(const Color_Int& rhs);
    void operator/=(const int& rhs);

    uint32_t toUint32();
    void fromUint32(uint32_t color);
};

struct Color_Float {
    float r;
    float b;
    float g;
    float a;

    void operator*=(const float& rhs);
};

struct RTTypesRay {
    RTTypesVector point;
    RTTypesVector direction;

    void createFromPoints(RTTypesVector * start, RTTypesVector * through);
};

struct RTTypesSphere {
    RTTypesVector center;
    float r;
};

struct RTTypesPlane {
    RTTypesVector normalVector;
    float d;
};

#endif // _RT_TYPES___H_


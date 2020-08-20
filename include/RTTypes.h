
#ifndef _RT_TYPES___H_
#define _RT_TYPES___H_

#include <cstdint>
#include <math.h>

// A simple 3d vector type. Basic arithmetic and linear algebra operations are
// implemented for it
struct RTTypesVector {
    // 3d coordinates
    float x;
    float y;
    float z;

    // overloaded arithmetic operators
    void operator=(const RTTypesVector& rhs);
    RTTypesVector operator+(const RTTypesVector& rhs);
    RTTypesVector operator-(const RTTypesVector& rhs);
    RTTypesVector operator*(const float& rhs);
    void operator+=(const RTTypesVector& rhs);

    // basic linear algebra operations
    float abs();
    float dist(const RTTypesVector v);
    float dot(const RTTypesVector v);
    float cosineOfAngleBetween(const RTTypesVector v);
    float angleBetween(const RTTypesVector v);
    void invert();
};

// a packed 8-bit per channel rgba color descriptor
struct Color {
    uint8_t r;
    uint8_t b;
    uint8_t g;
    uint8_t a;

    // convert to a 32 bit integeber of the form rgb, with r in the most
    // significant position; the alpha channel is ignored in this operation
    uint32_t u32();

    // overloaded operators for basic color manipulation
    void operator+=(const Color& rhs);
    void operator/=(const int& rhs);
};

// an rgba color descriptor where each sub-color has signed 32-bit precision
struct Color_Int {
    int r;
    int g;
    int b;
    int a;

    // overloaded operators for basic color manipulation
    Color_Int operator+(const Color_Int& rhs);
    Color_Int operator*(const int& rhs);
    Color_Int operator/(const int& rhs);
    void operator+=(const Color_Int& rhs);
    void operator/=(const int& rhs);

    // convert to a 32 bit integeber of the form rgb, with r in the most
    // significant position; the alpha channel is ignored in this operation.
    // values greater than 255 are clipped to 255
    uint32_t toUint32();

    // convert from a packed color integer of the form rgb with r as the most
    // significant bits (8 bits per color) and covert it to a Color_Int
    void fromUint32(uint32_t color);
};

// an rgba color descriptor where each component is expressed as a float
struct Color_Float {
    float r;
    float b;
    float g;
    float a;

    // overloaded operators for basic color manipulation
    void operator*=(const float& rhs);
};

// a basic structure for a ray, defined by a point of origin and a direction
struct RTTypesRay {

    // structure
    RTTypesVector point;
    RTTypesVector direction;

    // create a ray from a point of origin and a point that the ray passes
    // through
    void createFromPoints(RTTypesVector * start, RTTypesVector * through);
};

// a basic descriptor for a sphere, based on a center and radius
struct RTTypesSphere {
    RTTypesVector center;
    float r;
};

// a basic descriptor for a plane; it is defined based on a normal vector N
// and a value d such that the plane is described by the equation
// N.x * x + N.y * y + N.z * z = d
struct RTTypesPlane {
    RTTypesVector normalVector;
    float d;
};

#endif // _RT_TYPES___H_


#include "RTTypes.h"

// Basic arithmetic operators for RTTypesVector
void RTTypesVector::operator=(const RTTypesVector& rhs) {
    this -> x = rhs.x;
    this -> y = rhs.y;
    this -> z = rhs.z;
}
RTTypesVector RTTypesVector::operator+(const RTTypesVector& rhs) {
    RTTypesVector returnVec = {
        this -> x + rhs.x,
        this -> y + rhs.y,
        this -> z + rhs.z,
    };
    return returnVec;
}
RTTypesVector RTTypesVector::operator-(const RTTypesVector& rhs) {
    RTTypesVector returnVec = {
        this -> x - rhs.x,
        this -> y - rhs.y,
        this -> z - rhs.z,
    };
    return returnVec;
}
RTTypesVector RTTypesVector::operator*(const float& rhs) {
    RTTypesVector returnVec = {
        this -> x * rhs,
        this -> y * rhs,
        this -> z * rhs
    };
    return returnVec;
}
void RTTypesVector::operator+=(const RTTypesVector& rhs) {
    this -> x += rhs.x;
    this -> y += rhs.y;
    this -> z += rhs.z;
}

// get the absolute value of a vector
float RTTypesVector::abs() {
    return sqrt(this -> x * this -> x + this -> y * this -> y + this -> z *
        this -> z);
}

// get the distance between two vectors (the vectors are interpreted as 3d
// points)
float RTTypesVector::dist(const RTTypesVector v) {
    RTTypesVector diff = {
        this -> x - v.x,
        this -> y - v.y,
        this -> z - v.z
    };
    return diff.abs();
}

// get the dot product of the vector with another vector
float RTTypesVector::dot(const RTTypesVector v) {
    return v.x * this -> x + v.y * this -> y + v.z * this -> z;
}

// return the cosine of the angle between two vectors
float RTTypesVector::cosineOfAngleBetween(const RTTypesVector v) {
    return (this -> x * v.x + this -> y * v.y + this -> z * v.z) /
        sqrt((this -> x * this -> x + this -> y * this -> y + this -> z * 
        this -> z) * (v.x * v.x + v.y * v.y + v.z * v.z));
}

// return the angle between two vectors
float RTTypesVector::angleBetween(const RTTypesVector v) {
    return acos( cosineOfAngleBetween(v) );
}

// negate a vector
void RTTypesVector::invert() {
    this -> x = -this -> x;
    this -> y = -this -> y;
    this -> z = -this -> z;
}

// convert to a 32 bit integeber of the form rgb, with r in the most
// significant position; the alpha channel is ignored in this operation
uint32_t Color::u32() {
    return ((uint32_t)r << 16) + ((uint32_t)g << 8) + (uint32_t)b;
}

// overloaded operators for manipulating a Color; WARNING overflow is NOT
// guarded against
void Color::operator+=(const Color& rhs) {
    this -> r += rhs.r;
    this -> b += rhs.b;
    this -> g += rhs.g;
    this -> a += rhs.a;
}
void Color::operator/=(const int& rhs) {
    this -> r /= rhs;
    this -> b /= rhs;
    this -> g /= rhs;
    this -> a /= rhs;
}

// overloaded operators for manipulating a Color_Int; WARNING overflow is NOT
// guarded against
Color_Int Color_Int::operator+(const Color_Int& rhs) {
    Color_Int returnValue = {
        this -> r + rhs.r,
        this -> g + rhs.g,
        this -> b + rhs.b,
        this -> a + rhs.a
    };
    return returnValue;
}
Color_Int Color_Int::operator*(const int& rhs) {
    Color_Int returnValue = {
        this -> r * rhs,
        this -> g * rhs,
        this -> b * rhs,
        this -> a * rhs
    };
    return returnValue;
}
Color_Int Color_Int::operator/(const int& rhs) {
    Color_Int returnValue = {
        this -> r / rhs,
        this -> g / rhs,
        this -> b / rhs,
        this -> a / rhs
    };
    return returnValue;
}
void Color_Int::operator+=(const Color_Int& rhs) {
    this -> r += rhs.r;
    this -> b += rhs.b;
    this -> g += rhs.g;
    this -> a += rhs.a;
}
void Color_Int::operator/=(const int& rhs) {
    this -> r /= rhs;
    this -> b /= rhs;
    this -> g /= rhs;
    this -> a /= rhs;
}

// convert to a 32 bit integeber of the form rgb, with r in the most
// significant position; the alpha channel is ignored in this operation.
// values greater than 255 are clipped to 255
uint32_t Color_Int::toUint32() {
    uint32_t r_ret = this -> r; if (r_ret > 255) {r_ret = 255;}
    uint32_t g_ret = this -> g; if (g_ret > 255) {g_ret = 255;}
    uint32_t b_ret = this -> b; if (b_ret > 255) {b_ret = 255;}

    return (r_ret << 16) + (g_ret << 8) + (b_ret);
}

// convert from a packed color integer of the form rgb with r as the most
// significant bits (8 bits per color) and covert it to a Color_Int
void Color_Int::fromUint32(uint32_t color) {
    this -> r = (color >> 16) & 0xFF;
    this -> g = (color >> 8) & 0xFF;
    this -> b = color & 0xFF;
}

// overloaded operators for manipulating a Color_Float
void Color_Float::operator*=(const float& rhs) {
    this -> r *= rhs;
    this -> b *= rhs;
    this -> g *= rhs;
    this -> a *= rhs;
}

// create a ray from a point of origin and a point that the ray passes
// through
void RTTypesRay::createFromPoints(RTTypesVector * start,
    RTTypesVector * through) {
    
    this -> point = *start;
    this -> direction = *through - *start;
}

#ifndef __Vec3__
#define __Vec3__

#include <iostream>

//a;lskd
template<typename T>
struct Vec3 {
    T x;
    T y;
    T z;
    Vec3() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
    template <typename U>
    explicit Vec3(const U &other) {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
    }
    template <typename U>
    Vec3(const Vec3<U> &other) {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
    }
    template<typename U>
    T dot(const Vec3<U> &other) {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }
    template <typename U>
    Vec3(U x, U y, U z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    friend Vec3<T> operator-(const Vec3<T> &vector, const Vec3<T> &other) {
        return Vec3<T>(vector.x - other.x, vector.y - other.y, vector.z - other.z);
    }
    friend Vec3<T> operator+(const Vec3<T> &vector, const Vec3<T> &other) {
        return Vec3<T>(vector.x + other.x, vector.y + other.y, vector.z + other.z);
    }
    friend Vec3<T> &operator+=(Vec3<T> &vector, const Vec3<T> &other) {
        vector.x += other.x;
        vector.y += other.y;
        vector.z += other.z;
        return vector;
    }
    friend Vec3<T> &operator-=(Vec3<T> &vector, const Vec3<T> &other) {
        vector.x -= other.x;
        vector.y -= other.y;
        vector.z -= other.z;
        return vector;
    }
    friend Vec3<T> operator*(const Vec3<T> &vector, const Vec3<T> &other) {
        return Vec3<T>(vector.x * other.x, vector.y * other.y, vector.z * other.z);
    }
    friend Vec3<T> operator/(const Vec3<T> &vector, const Vec3<T> &other) {
        return Vec3<T>(vector.x / other.x, vector.y / other.y, vector.z / other.z);
    }
    friend bool operator==(const Vec3<T> &vector, const Vec3<T> &other) {
        return vector.x == other.x && vector.y == other.y && vector.z == other.z;
    }
    friend bool operator!=(const Vec3<T> &vector, const Vec3<T> &other) {
        return !(vector == other);
    }
};

typedef Vec3<double> Vec3d;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

#endif


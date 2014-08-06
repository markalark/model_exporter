#ifndef __Vec2__
#define __Vec2__

#include <iostream>

//a;lskd
template<typename T>
struct Vec2 {
    T x;
    T y;
    Vec2() {
        this->x = 0;
        this->y = 0;
    }
    template <typename U>
    Vec2(const Vec2<U> &other) {
        this->x = other.x;
        this->y = other.y;
    }
    template <typename U>
    explicit Vec2(const U &other) {
        this->x = other.x;
        this->y = other.y;
    }
    Vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }
    template <typename U>
    Vec2(U x, U y) {
        this->x = x;
        this->y = y;
    }
    friend Vec2<T> operator-(const Vec2<T> &vector, const Vec2<T> &other) {
        return Vec2<T>(vector.x - other.x, vector.y - other.y);
    }
    friend Vec2<T> operator+(const Vec2<T> &vector, const Vec2<T> &other) {
        return Vec2<T>(vector.x + other.x, vector.y + other.y);
    }
    friend Vec2<T> &operator+=(Vec2<T> &vector, const Vec2<T> &other) {
        vector.x += other.x;
        vector.y += other.y;
        return vector;
    }
    friend Vec2<T> &operator-=(Vec2<T> &vector, const Vec2<T> &other) {
        vector.x -= other.x;
        vector.y -= other.y;
        return vector;
    }
    friend Vec2<T> operator*(const Vec2<T> &vector, const Vec2<T> &other) {
        return Vec2<T>(vector.x * other.x, vector.y * other.y);
    }
    friend Vec2<T> operator/(const Vec2<T> &vector, const Vec2<T> &other) {
        return Vec2<T>(vector.x / other.x, vector.y / other.y);
    }
    friend bool operator==(const Vec2<T> &vector, const Vec2<T> &other) {
        return vector.x == other.x && vector.y == other.y;
    }
    friend bool operator!=(const Vec2<T> &vector, const Vec2<T> &other) {
        return !(vector == other);
    }
};


typedef Vec2<double> Vec2d;
typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

#endif


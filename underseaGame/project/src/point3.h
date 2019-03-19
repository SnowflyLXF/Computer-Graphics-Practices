#pragma once

#include <math.h>
#include <GL/gl.h>
#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


class Point3 {
public:

    float coord[3]; 
    float X() const { return coord[0]; }

    float Y() const { return coord[1]; }

    float Z() const { return coord[2]; }

	float operator [](size_t i) const
    {
		assert(i < 3);
	    return coord[i];
    }
	float& operator [](size_t i)
    {
		assert(i < 3);
	    return coord[i];
    }

    static Point3 GetUpVector() {
        return {0.f, 1.f, 0.f};
    }

    static Point3 GetZeroVector() {
        return {0.f, 1.f, 0.f};
    }
    // Costruttore 1
    Point3(float x, float y, float z) {
        coord[0] = x;
        coord[1] = y;
        coord[2] = z;
    }

    Point3() {
        coord[0] = coord[1] = coord[2] = 0.f;
    }

    Point3 Normalize() const {
        return (*this) / modulo();
    }

    Point3 NormalizeSafe() const {
        const float mod = modulo();
		static constexpr float tolerance = 0.001f;
        if (mod < tolerance) {
            return GetZeroVector();
        } else {
            return (*this) / mod;
        }
    }

    float modulo() const {
        return
                sqrtf(coord[0] * coord[0] + coord[1] * coord[1] + coord[2] * coord[2]);
    }

    Point3 operator/(float scalar) const {
        return Point3(
                coord[0] / scalar,
                coord[1] / scalar,
                coord[2] / scalar
        );
    }

    Point3 operator*(float scalar) const {
        return Point3(
                coord[0] * scalar,
                coord[1] * scalar,
                coord[2] * scalar
        );
    }

    Point3 operator-() const {
        return Point3(
                -coord[0],
                -coord[1],
                -coord[2]
        );
    }

    Point3 operator-(const Point3 &a) const {
        return Point3(
                coord[0] - a.coord[0],
                coord[1] - a.coord[1],
                coord[2] - a.coord[2]
        );
    }

    Point3 operator+(const Point3 &a) const {
        return Point3(
                coord[0] + a.coord[0],
                coord[1] + a.coord[1],
                coord[2] + a.coord[2]
        );
    }


    Point3 operator%(const Point3 &a) const {
        return Point3(
                coord[1] * a.coord[2] - coord[2] * a.coord[1],
                -(coord[0] * a.coord[2] - coord[2] * a.coord[0]),
                coord[0] * a.coord[1] - coord[1] * a.coord[0]
        );
    }

    void SendAsVertex() const {
        glVertex3fv(coord);
    }

    void SendAsNormal() const {
        glNormal3fv(coord);
    }

};


typedef Point3 Vector3;

inline void glTranslate(const Point3& v) {
    glTranslatef(v.X(), v.Y(), v.Z());
}



#pragma once

#include "point3.h"

class Point2;

namespace utils {
    float random_float();

    float get_elapsed_seconds();

    float distance(const Point3& A, const Point3& B);

    float radiansToDegrees(const float angleRadians);

    void setCoordToPixel(const Point2& screenSize);
	GLenum checkGLError(const char* file, int line);
}

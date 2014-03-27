#ifndef CIRCLE_H
#define CIRCLE_H

#include "Vec2.h"

class Circle
{
    public:
        Circle();
        Circle(float R, const Vec2& center);

        bool hits(Circle& A, Vec2& normal);

        float r;
        Vec2  c;



};

#endif // CIRCLE_H

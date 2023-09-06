#ifndef VEC2D_H
#define VEC2D_H

#ifndef VEC2DDEF
#ifdef VEC2D_STATIC
#define VEC2DDEF static
#else
#define VEC2DDEF extern
#endif
#endif

#ifdef VEC2D_WITH_RAYLIB
#include <raylib.h>
typedef Vector2 Vec2d;

VEC2DDEF void draw_arrow(Vec2d base, Vec2d rel, Color color, char *text);
#else
typedef struct {
    float x;
    float y;
} Vec2d;
#endif // VEC2D_WITH_RAYLIB

VEC2DDEF Vec2d vec2d_add(Vec2d u, Vec2d v);
VEC2DDEF Vec2d vec2d_sum(Vec2d *vecs, unsigned int len);
VEC2DDEF Vec2d vec2d_sub(Vec2d u, Vec2d v);
VEC2DDEF Vec2d vec2d_from_to(Vec2d u, Vec2d v);
VEC2DDEF float vec2d_dot(Vec2d u, Vec2d v);
VEC2DDEF float vec2d_cross(Vec2d u, Vec2d v);
VEC2DDEF Vec2d vec2d_scale(Vec2d u, float scaler);
VEC2DDEF float vec2d_length(Vec2d u);
VEC2DDEF float vec2d_length_sqr(Vec2d u);
VEC2DDEF Vec2d vec2d_unitinterval(Vec2d u);
VEC2DDEF Vec2d vec2d_rotate(Vec2d u, float theta);
VEC2DDEF void vec2d_print(Vec2d vec, const char *name);
#define VEC2D_PRINT(vec) vec2d_print((vec), (#vec))

#endif  /* VEC2D_H */

#ifdef VEC2D_IMPLEMENTATION  /* Implementation goes here */
#include <math.h>
#include <stdio.h>


VEC2DDEF Vec2d vec2d_add(Vec2d u, Vec2d v)
{
    return (Vec2d) {
        .x = u.x + v.x,
        .y = u.y + v.y,
    };
}

VEC2DDEF Vec2d vec2d_sum(Vec2d *vecs, unsigned int len)
{
    unsigned int i;
    Vec2d rv = { 0, 0 };
    for (i = 0; i < len; ++i) {
        rv = vec2d_add(rv, vecs[i]);

    }
    return rv;
}

VEC2DDEF Vec2d vec2d_sub(Vec2d u, Vec2d v)
{
    return (Vec2d) {
        .x = u.x - v.x,
        .y = u.y - v.y,
    };
}

VEC2DDEF Vec2d vec2d_from_to(Vec2d u, Vec2d v)
{
    return (Vec2d) {
        .x = v.x - u.x,
        .y = v.y - u.y,
    };
}

VEC2DDEF float vec2d_dot(Vec2d u, Vec2d v)
{
    return u.x * v.y + u.y * v.y;
}

VEC2DDEF float vec2d_cross(Vec2d u, Vec2d v)
{
    return u.x * v.y - u.y * v.x;
}

VEC2DDEF Vec2d vec2d_scale(Vec2d u, float scaler)
{
    return (Vec2d) {
        .x = u.x * scaler,
        .y = u.y * scaler,
    };
}

VEC2DDEF float vec2d_length(Vec2d u)
{
    return sqrtf(u.x * u.x + u.y * u.y);
}

VEC2DDEF float vec2d_length_sqr(Vec2d u)
{
    return u.x * u.x + u.y * u.y;
}

VEC2DDEF Vec2d vec2d_unitinterval(Vec2d u)
{
    // Find highest absolute value
    // Find highest absolute value
    // float max = fabs(u.x) >= fabs(u.y) ? fabs(u.x) : fabs(u.y);
    float max = fmax(fabs(u.x), fabs(u.y));
    return (Vec2d) {
        .x = u.x / max,
        .y = u.y / max,
    };
}

// https://en.wikipedia.org/wiki/Rotation_matrix
// Rotate vector counter clockwise
VEC2DDEF Vec2d vec2d_rotate(Vec2d u, float theta)
{
    // -   -   -                 -
    // | x |   | cos(θ)  -sin(θ) |
    // |   | * |                 |
    // | y |   | sin(θ)   cos(θ) |
    // -   -   -                 -
    return (Vec2d) {
        .x = u.x * cosf(theta) - u.y * sinf(theta),
        .y = u.x * sinf(theta) + u.y * cosf(theta),
    };
}

VEC2DDEF void vec2d_print(Vec2d vec, const char *name)
{
    printf("%s = [%.2f, %.2f]\n", name, vec.x, vec.y);
}

#ifdef VEC2D_WITH_RAYLIB
#include <raylib.h>

VEC2DDEF void draw_arrow(Vec2d base, Vec2d rel, Color color, char *text)
{
    if (vec2d_length_sqr(rel) == 0) {
        return;
    }
    Vec2d tipp = vec2d_add(base, rel);
    Vec2d arrow_rel_tipp = vec2d_scale(vec2d_unitinterval(rel), 16);
    Vec2d arrow_base = vec2d_sub(tipp, arrow_rel_tipp);

    // Vec2d k3 = vec2d_add(arrow_base, arrow_rel_tipp);
    Vec2d k1 = vec2d_add(arrow_base, vec2d_scale(vec2d_rotate(arrow_rel_tipp, PI / 2), 0.66));
    Vec2d k2 = vec2d_add(arrow_base, vec2d_scale(vec2d_rotate(arrow_rel_tipp, -PI / 2), 0.66));

    DrawTriangle(k2, k1, tipp, color);
    DrawLineEx(base, arrow_base, 6, color);
    DrawText(text, tipp.x + 20, tipp.y + 20, 30, color);
}
#endif // VEC2D_WITH_RAYLIB

#endif  /* VEC2D_IMPLEMENTATION */

/*
The MIT License (MIT)

Copyright (c) 2023 Eliza Clausen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

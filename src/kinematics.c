#include <raylib.h>

#define VEC2D_WITH_RAYLIB
#include "vec2d.h"

#include "kinematics.h"
#include "core.h"
#include "math.h"


extern void kin_draw_square(Kin kin, u32 size, f32 thick, Color color)
{
    // Find (r)elative (l)eft and (u)p vector
    Vec2d rel_r = vec2d_rotate((Vec2d) { size, 0 }, -kin.angle);
    Vec2d rel_u = vec2d_rotate((Vec2d) { size, 0 }, -kin.angle - PI / 2);

    // Offset (d)own (l)eft corner to put center of mass in the center.
    Vec2d dl = vec2d_sub(kin.pos, vec2d_scale(vec2d_add(rel_r, rel_u), 0.5));

    Vec2d dr = vec2d_add(dl, rel_r);
    Vec2d ul = vec2d_add(dl, rel_u);
    Vec2d ur = vec2d_add(dl, vec2d_add(rel_r, rel_u));

    DrawLineEx(dl, dr, thick, color);
    DrawLineEx(dl, ul, thick, color);
    DrawLineEx(dr, ur, thick, color);
    DrawLineEx(ul, ur, thick, color);
}

extern void kin_draw_floor(Kin kin, u32 size, f32 thick, Color color)
{
    Vec2d rel_r = vec2d_rotate((Vec2d) { size, 0 }, -kin.angle);
    Vec2d rel_u = vec2d_rotate((Vec2d) { size, 0 }, -kin.angle - PI / 2);
    Vec2d dl = vec2d_sub(kin.pos, vec2d_scale(vec2d_add(rel_r, rel_u), 0.5));

    Vec2d start = vec2d_add(dl, vec2d_scale(rel_r, -10));
    Vec2d end = vec2d_add(dl, vec2d_scale(rel_r, 10));
    DrawLineEx(dl, end, thick, color);
    DrawLineEx(dl, start, thick, color);
}

extern Vec2d kin_force_normal(Kin kin)
{
    
    return vec2d_rotate(vec2d_scale(kin.acc, kin.mass * cosf(kin.angle)), -kin.angle - PI);
}

extern Vec2d kin_force_gravity(Kin kin)
{
    return (Vec2d) {
        .x = 0,
        .y = kin.mass * kin.gravity,
    };
}

// Math source: https://en.wikipedia.org/wiki/Friction#Normal_force
extern Vec2d kin_force_not_friction(Kin kin)
{
    return vec2d_add(kin_force_gravity(kin), kin_force_normal(kin));
}

// Math source: https://en.wikipedia.org/wiki/Friction#Normal_force
extern Vec2d kin_force_friction(Kin kin)
{
    Vec2d fric = vec2d_scale(kin_force_normal(kin), kin.friction_static);
    Vec2d notfric = kin_force_not_friction(kin);
    if (vec2d_length_sqr(notfric) < vec2d_length_sqr(fric)) {
        // Not Sliding
        return vec2d_scale(notfric, -1);
    }

    // Since there is no motion. Use this to find direction.
    if (fmodf(kin.angle, (2 * PI)) > 0) {
        return vec2d_rotate(fric, 90 * DEG2RAD);
    }
    return vec2d_rotate(fric, -90 * DEG2RAD);
}

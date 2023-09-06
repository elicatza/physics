#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "core.h"

#include "vec2d.h"

typedef struct {
    Vec2d pos;
    Vec2d vel;
    Vec2d acc;
    f32 mass;
    f32 friction_static;
    f32 friction_kinetic;
    f32 gravity;
    f32 angle;
} Kin;

extern void kin_draw_square(Kin kin, u32 size, f32 thick, Color color);
extern void kin_draw_floor(Kin kin, u32 size, f32 thick, Color color);
extern Vec2d kin_force_normal(Kin kin);
extern Vec2d kin_force_gravity(Kin kin);
extern Vec2d kin_force_not_friction(Kin kin);
extern Vec2d kin_force_friction(Kin kin);

#endif  // KINEMATICS_H

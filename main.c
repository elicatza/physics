#include <raylib.h>
#include <stdint.h>

#define VEC2D_RAYLIB
#define VEC2D_IMPLEMENTATION
#include "./vec2d.h"

#define WIDTH 750
#define HEIGHT 750
#define FPS 60
#define GRAVITY 9.81f

#define M_BG     GetColor(0x282828ff)
#define M_FG     GetColor(0xebdbb2ff)
#define M_BLUE   GetColor(0x458588ff)
#define M_AQUA   GetColor(0x689d6aff)
#define M_PURPLE GetColor(0xb16286ff)
#define M_RED    GetColor(0xcc241dff)
#define M_GREEN  GetColor(0x98971aff)
#define M_YELLOW GetColor(0xd79921ff)
#define M_ORANGE GetColor(0xd65d0eff)

// Kinematics
typedef struct {
    Vec2d pos;
    Vec2d vel;
    Vec2d acc;
    float mass;
    float friction_static;
    float friction_kinetic;
    float angle;
} Kin;


void kin_draw_square(Kin kin, uint32_t size, float thick, Color color)
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

void kin_draw_floor(Kin kin, uint32_t size, float thick, Color color)
{
    Vec2d rel_r = vec2d_rotate((Vec2d) { size, 0 }, -kin.angle);
    Vec2d rel_u = vec2d_rotate((Vec2d) { size, 0 }, -kin.angle - PI / 2);
    Vec2d dl = vec2d_sub(kin.pos, vec2d_scale(vec2d_add(rel_r, rel_u), 0.5));

    Vec2d start = vec2d_add(dl, vec2d_scale(rel_r, -10));
    Vec2d end = vec2d_add(dl, vec2d_scale(rel_r, 10));
    DrawLineEx(dl, end, thick, color);
    DrawLineEx(dl, start, thick, color);
}

Vec2d kin_force_normal(Kin kin)
{
    
    return vec2d_rotate(vec2d_scale(kin.acc, kin.mass * cosf(kin.angle)), -kin.angle - PI);
}

Vec2d kin_force_gravity(Kin kin)
{
    return (Vec2d) {
        .x = 0,
        .y = kin.mass * GRAVITY,
    };
}

Vec2d kin_force_not_friction(Kin kin)
{
    return vec2d_add(kin_force_gravity(kin), kin_force_normal(kin));
}

// Math source: https://en.wikipedia.org/wiki/Friction#Normal_force
Vec2d kin_force_friction(Kin kin)
{
    Vec2d fric = vec2d_scale(kin_force_normal(kin), kin.friction_static);
    Vec2d notfric = kin_force_not_friction(kin);
    if (vec2d_length(notfric) < vec2d_length(fric)) {
        // Not Sliding
        return vec2d_scale(notfric, -1);
    }

    // Since there is no motion. Use this to find direction.
    if (fmodf(kin.angle, (2 * PI)) > 0) {
        return vec2d_rotate(fric, 90 * PI / 180);
    }
    return vec2d_rotate(fric, -90 * PI / 180);
}

void draw_arrow(Vec2d base, Vec2d rel, Color color, char *text)
{
    if (vec2d_length(rel) == 0) {
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

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Physics");
    SetTargetFPS(FPS);

    Camera2D camera = {
        .zoom = 1.0f,
        .offset = (Vec2d) {  WIDTH / 2, HEIGHT / 2  },
        .rotation = 0,
        .target = (Vec2d) { WIDTH / 2, HEIGHT / 2 },
    };

    Kin kin = {
        .pos = (Vec2d) { WIDTH / 2, HEIGHT / 2 },
        .vel = (Vec2d) { 100, 100 },
        .acc = (Vec2d) { 0, GRAVITY },
        .friction_static = 0.17f,
        .friction_kinetic = 0.17f,
        .mass = 25,
        .angle = 10 * PI / 180,
    };

    bool no_hud = false;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) no_hud = !no_hud;

        if (IsKeyPressed(KEY_R)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                kin.angle -= PI / 180;
            } else {
                kin.angle += PI / 180;
            }
        }

        if (IsKeyPressed(KEY_F)) {
            if (camera.rotation != 0) {
                camera.rotation = 0;
            } else {
                camera.rotation = kin.angle * 180 / PI;
            }
        }
        if (GetMouseWheelMove()) {
            if (camera.zoom + GetMouseWheelMove() * 0.05 >= 0) {
                camera.zoom += GetMouseWheelMove() * 0.05;
            }
        }

        if (IsKeyPressed(KEY_M)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                kin.friction_static -= 0.01f;
            } else {
                kin.friction_static += 0.01f;
            }
        }
        // TODO: Display friction constant and adjust it
        if (IsKeyPressed(KEY_Z)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                camera.zoom -= 0.1f;
            } else {
                camera.zoom += 0.1f;
            }
        }

        Vec2d forces[] = {
            kin_force_gravity(kin),
            kin_force_normal(kin),
            kin_force_friction(kin),
        };

        BeginDrawing(); {
            BeginMode2D(camera); {
                ClearBackground(M_BG);

                kin_draw_square(kin, 200, 5, M_FG);
                kin_draw_floor(kin, 200, 5, M_FG);

                draw_arrow(kin.pos, kin_force_gravity(kin), M_GREEN, "G");
                draw_arrow(kin.pos, kin_force_normal(kin), M_BLUE, "N");

                draw_arrow(kin.pos, vec2d_sum(forces, 3), M_RED, "Sum F");
                draw_arrow(kin.pos, kin_force_friction(kin), M_PURPLE, "R");

            } EndMode2D();

            if (!no_hud) {
                char buf[32];
                snprintf(buf, 32, "Friction: %.2f", kin.friction_static);
                DrawText(buf, 10, 10, 50, M_BLUE);
                snprintf(buf, 32, "Angle: %.2f", kin.angle * 180 / PI);
                DrawText(buf, 10, 70, 50, M_BLUE);
            }

        } EndDrawing();
    }

    CloseWindow();

    return 0;
}

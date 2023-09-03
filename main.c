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
    float friction;
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

Vec2d kin_force_friction(Kin kin)
{
    Vec2d fric = vec2d_scale(kin_force_normal(kin), kin.friction);
    if (fmodf(kin.angle, (2 * PI)) > 0) {
        return vec2d_rotate(fric, 90 * PI / 180);
    }
    return vec2d_rotate(fric, -90 * PI / 180);
}

void draw_arrow(Vec2d base, Vec2d rel, Color color, char *text)
{
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
    Kin kin = {
        .pos = (Vec2d) { WIDTH / 2, HEIGHT / 2 },
        .vel = (Vec2d) { 100, 100 },
        .acc = (Vec2d) { 0, GRAVITY },
        .friction = 0.17f,
        .mass = 25,
        .angle = 10 * PI / 180,
    };

    while (!WindowShouldClose()) {
        BeginDrawing(); {
            ClearBackground(M_BG);
            Vec2d forces[] = {
                kin_force_gravity(kin),
                kin_force_normal(kin),
                kin_force_friction(kin),
            };
            // VEC2D_PRINT(vec2d_sum(forces, 3));

            // DrawLineEx((Vector2) { 0, HEIGHT }, (Vector2) { WIDTH, HEIGHT }, 5, M_BLUE);
            // DrawRectangle(0, HEIGHT / 1.5 - HEIGHT / 12.f, WIDTH, HEIGHT / 12.f, M_GREEN);
            draw_arrow(kin.pos, vec2d_sum(forces, 3), M_RED, "Sum F");
            DrawRectanglePro((Rectangle){
                    .x = kin.pos.x,
                    .y = kin.pos.y,
                    .width = 10,
                    .height = 20,
                    }, kin.pos, kin.angle, M_PURPLE);

            draw_arrow(kin.pos, kin_force_gravity(kin), M_GREEN, "G");
            draw_arrow(kin.pos, kin_force_normal(kin), M_BLUE, "N");
            draw_arrow(kin.pos, kin_force_friction(kin), M_PURPLE, "R");
            kin_draw_square(kin, 100, 5, M_FG);

            char angle[32];
            snprintf(angle, 32, "%.2f", kin.angle * 180 / PI);
            DrawText(angle, 10, 10, 50, M_BLUE);

        }
        EndDrawing();

        if (IsKeyPressed(KEY_C)) {
            kin.angle -= PI / 180;
        }
        if (IsKeyPressed(KEY_V)) {
            kin.angle += PI / 180;
        }
    }

    CloseWindow();

    return 0;
}

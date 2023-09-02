#include <raylib.h>
#include <stdint.h>

#define VEC2D_RAYLIB
#define VEC2D_IMPLEMENTATION
#include "./vec2d.h"

#define WIDTH 700
#define HEIGHT 700
#define FPS 60
#define GRAVITY 9.81f

#define M_BG    GetColor(0x282828ff)
#define M_BLUE  GetColor(0x55cdfcff)
#define M_PINK  GetColor(0xf7a8b8ff)
#define M_RED   GetColor(0xcc241dff)
#define M_GREEN GetColor(0x98971aff)

// Kinematics
typedef struct {
    Vec2d pos;
    Vec2d vel;
    Vec2d acc;
    float mass;
    float friction;
    // bool is_floating;
    float angle;
} Kin;


void kin_draw(Kin kin, uint32_t size, float thick)
{
    Vec2d base = vec2d_sub(kin.pos, vec2d_rotate((Vec2d) { size / 2, 0 }, -kin.angle));
    // Vec2d base = vec2d_sub(kin.pos, vec2d_scale(kin.pos, size / 2)));
    Vec2d left = vec2d_rotate((Vec2d) { size, 0 }, - kin.angle);
    Vec2d up = vec2d_rotate((Vec2d) { size, 0 }, - kin.angle - PI / 2);
    Vec2d upleft = vec2d_add(left, up);
    DrawLineEx(base, vec2d_add(base, left), thick, M_BLUE);
    DrawLineEx(base, vec2d_add(base, up), thick, M_BLUE);
    DrawLineEx(vec2d_add(base, left), vec2d_add(base, upleft), thick, M_BLUE);
    DrawLineEx(vec2d_add(base, up), vec2d_add(base, upleft), thick, M_BLUE);
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
    Vec2d end = vec2d_add(base, rel);
    Vec2d linedir = vec2d_scale(vec2d_unitinterval(rel), 16);
    Vec2d arrow_length = vec2d_scale(linedir, 1.2);
    Vec2d arrow_base = vec2d_sub(end, arrow_length);
    Vec2d k3 = vec2d_add(arrow_base, vec2d_scale(linedir, 1.2));
    Vec2d k1 = vec2d_add(arrow_base, vec2d_rotate(linedir, PI / 2));
    Vec2d k2 = vec2d_add(arrow_base, vec2d_rotate(linedir, -PI / 2));
    DrawTriangle(k2, k1, k3, color);
    DrawLineEx(base, arrow_base, 6, color);
    DrawText(text, end.x + 20, end.y + 20, 30, color);
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Physics");
    SetTargetFPS(FPS);
    Kin kin = {
        .pos = (Vec2d) { 500, 500 },
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
                    }, kin.pos, kin.angle, M_PINK);

            draw_arrow(kin.pos, kin_force_gravity(kin), M_GREEN, "G");
            draw_arrow(kin.pos, kin_force_normal(kin), M_BLUE, "N");
            draw_arrow(kin.pos, kin_force_friction(kin), M_PINK, "R");
            kin_draw(kin, 100, 5);

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

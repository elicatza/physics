#include <raylib.h>

#define VEC2D_IMPLEMENTATION
#define VEC2D_WITH_RAYLIB
#include "vec2d.h"
#undef  VEC2D_IMPLEMENTATION

#include "core.h"
#include "kinematics.h"


#define WIDTH 750
#define HEIGHT 750
#define FPS 60
#define GRAVITY 9.81f


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
        .gravity = GRAVITY,
        .angle = 10 * DEG2RAD,
    };

    bool hud = false;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) hud = !hud;

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
                camera.rotation = kin.angle * RAD2DEG;
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
                ClearBackground(COL_BG);

                kin_draw_square(kin, 200, 5, COL_FG);
                kin_draw_floor(kin, 200, 5, COL_FG);

                draw_arrow(kin.pos, kin_force_gravity(kin), COL_GREEN, "G");
                draw_arrow(kin.pos, kin_force_normal(kin), COL_BLUE, "N");

                draw_arrow(kin.pos, vec2d_sum(forces, 3), COL_RED, "Sum F");
                draw_arrow(kin.pos, kin_force_friction(kin), COL_PURPLE, "R");

            } EndMode2D();

            if (!hud) {
                char buf[32];
                snprintf(buf, 32, "Friction: %.2f", kin.friction_static);
                DrawText(buf, 10, 10, 50, COL_BLUE);
                snprintf(buf, 32, "Angle: %.2f", kin.angle * RAD2DEG);
                DrawText(buf, 10, 70, 50, COL_BLUE);
            }

        } EndDrawing();
    }

    CloseWindow();

    return 0;
}

#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <vector>

#define WIDTH 350
#define HEIGHT 600

#define FPS 60

int main(void) {
  InitWindow(WIDTH, HEIGHT, "SlingShot Game");
  SetTargetFPS(FPS);

  const Vector2 origin = {(float)WIDTH / 2, HEIGHT - 50};

  const float radius = 100.0f;
  float time = 0.0f;
  float angle = 45.0f;

  Vector2 projectile = origin;
  Vector2 projectile_direction = {0.0f, 0.0f};
  Vector2 projectile_velocity = {10.0f, 10.0f};

  bool shooting = false;

  while (!WindowShouldClose()) {
    time += GetFrameTime();
    angle = ((float)(-180 + 0) / 2) + ((float)(0 + 180) / 2) * sin(time * 1.2);
    // angle *= -1;

    // const Vector2 target

    if (IsKeyPressed(KEY_SPACE)) {
      if (!shooting) {
        shooting = true;

        projectile_direction = {
            .x = cos(angle * DEG2RAD),
            .y = sin(angle * DEG2RAD),
        };

        projectile_velocity = {
            .x = projectile_direction.x * 5.0f,
            .y = projectile_direction.y * 5.0f,
        };
      }
    }

    if (shooting) {
      if ((projectile.x + 5) > WIDTH || (projectile.x - 5) < 0) {
        projectile_velocity.x *= -1;
      }

      if (projectile.y > HEIGHT || projectile.y < 0) {
        shooting = false;
        projectile = origin;
      }

      projectile.x += projectile_velocity.x;
      projectile.y += projectile_velocity.y;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    const Vector2 target = {
        .x = origin.x + radius * cos(angle * DEG2RAD),
        .y = origin.y + radius * sin(angle * DEG2RAD),
    };

    if (shooting) {
      DrawCircleV(projectile, 5.0f, BLUE);
    }

    DrawCircleV(origin, 5.0f, RED);
    DrawCircleV(target, 5.0f, RED);

    DrawFPS(10, 30); // Show FPS counter

    EndDrawing();
  }

  CloseWindow();

  return 0;
}

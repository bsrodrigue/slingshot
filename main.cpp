#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <deque>
#include <utility>
#include <vector>

#define WIDTH 350
#define HEIGHT 600

#define FPS 60

enum class GameEvent { TARGET_HIT, TARGET_MISS, PLAYER_DEATH };

std::pair<Vector2, float> spawn_target(const float min_radius,
                                       const float max_radius) {
  // Spawn Target
  const float x = GetRandomValue(0, WIDTH);
  const float y = GetRandomValue(0, HEIGHT - 150);
  const float radius = GetRandomValue(min_radius, max_radius);

  Vector2 target_position = {x, y};
  float target_radius = radius;

  return {target_position, target_radius};
}

int main(void) {
  InitWindow(WIDTH, HEIGHT, "SlingShot Game");
  SetTargetFPS(FPS);

  const Vector2 origin = {(float)WIDTH / 2, HEIGHT - 50};

  const float radius = 100.0f;
  float time = 0.0f;
  float angle = 45.0f;

  std::deque<GameEvent> event_queue;

  Vector2 projectile = origin;
  Vector2 projectile_direction = {0.0f, 0.0f};
  Vector2 projectile_velocity = {10.0f, 10.0f};

  const float projectile_radius = 5.0f;

  Vector2 target_position = {};
  float target_radius = 0.0f;
  float target_oscillation_speed = 1.5f;
  float min_target_radius = 25.0f;
  float max_target_radius = 50.0f;
  float target_oscillation_amplitude = 30.0f;
  float target_anchor = 0.0f;

  bool target_oscillates = false;

  bool shooting = false;

  float rotation_speed = 1.1f;

  const float min_angle = -45.0f;
  const float max_angle = -135.0f;

  int score = 0;
  int lives = 3;

  std::pair<Vector2, float> p =
      spawn_target(min_target_radius, max_target_radius);

  target_position = p.first;
  target_radius = p.second;

  while (!WindowShouldClose()) {
    // Process events

    while (event_queue.size()) {
      GameEvent event = event_queue.front();
      event_queue.pop_front();

      switch (event) {
      case GameEvent::TARGET_HIT:
        score++;

        if (score == 5) {
          target_oscillates = true;
          min_target_radius = 20.0f;
          max_target_radius = 40.0f;
          rotation_speed = 1.5f;
        }

        else if (score == 10) {
          target_oscillates = true;
          min_target_radius = 10.0f;
          max_target_radius = 30.0f;
          rotation_speed = 1.8f;
        }

        break;
      case GameEvent::TARGET_MISS: {
        if (--lives <= 0) {
          event_queue.push_back(GameEvent::PLAYER_DEATH);
        }
      } break;
      case GameEvent::PLAYER_DEATH:
        lives = 0;
        CloseWindow();
        break;
      }
    }

    time += GetFrameTime();
    angle = ((float)(max_angle + min_angle) / 2) +
            ((float)(max_angle - min_angle) / 2) * sin(time * rotation_speed);

    if (target_oscillates) {
      target_position.x =
          target_oscillation_amplitude * sin(time * target_oscillation_speed) + target_anchor;
    }

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

    if (CheckCollisionCircles(projectile, projectile_radius, target_position,
                              target_radius)) {
      std::pair<Vector2, float> p =
          spawn_target(min_target_radius, max_target_radius);

      target_position = p.first;
      target_radius = p.second;
      target_anchor = target_position.x;

      shooting = false;
      projectile = origin;

      event_queue.push_back(GameEvent::TARGET_HIT);
    }

    if (shooting) {
      if ((projectile.x + 5) > WIDTH || (projectile.x - 5) < 0) {
        projectile_velocity.x *= -1;
      }

      if (projectile.y > HEIGHT || projectile.y < 0) {
        shooting = false;
        projectile = origin;

        event_queue.push_back(GameEvent::TARGET_MISS);
      }

      projectile.x += projectile_velocity.x;
      projectile.y += projectile_velocity.y;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    const Vector2 cursor = {
        .x = origin.x + radius * cos(angle * DEG2RAD),
        .y = origin.y + radius * sin(angle * DEG2RAD),
    };

    if (shooting) {
      DrawCircleV(projectile, 5.0f, BLUE);
    }

    DrawCircleV(origin, 5.0f, RED);
    DrawCircleV(cursor, 5.0f, RED);
    DrawCircleV(target_position, target_radius, GREEN);

    DrawFPS(10, 30); // Show FPS counter

    DrawText(TextFormat("Lives: %d", lives), WIDTH - 50, 30, 10, BLACK);
    DrawText(TextFormat("Score: %d", score), WIDTH - 50, 40, 10, BLACK);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}

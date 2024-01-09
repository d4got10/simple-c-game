#include "raylib.h"

Vector2 world_to_view(Vector2 world_position, Vector2 camera_position);
Vector2 view_to_world(Vector2 view_position, Vector2 camera_position);

bool rects_are_colliding(Vector2 first_position, Vector2 first_size, Vector2 second_position, Vector2 second_size);

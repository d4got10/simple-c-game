#pragma once

#include "utils.h"
#include <math.h>

Vector2 world_to_view(Vector2 world_position, Vector2 camera_position)
{
    int screen_height = GetScreenHeight();
    int screen_width = GetScreenWidth();

    Vector2 view_position = {
        x: screen_width / 2 + (world_position.x - camera_position.x),
        y: screen_height / 2 - (world_position.y - camera_position.y)
    };

    //printf("world_to_view: (%f %f) -> (%f %f)\n", world_position.x, world_position.y, view_position.x, view_position.y);

    return view_position;
}

Vector2 view_to_world(Vector2 view_position, Vector2 camera_position)
{
    int screen_height = GetScreenHeight();
    int screen_width = GetScreenWidth();

    Vector2 world_position = {
        x: view_position.x - screen_width / 2 + camera_position.x,
        y: screen_height / 2 - view_position.y + camera_position.y
    };

    // printf("view_to_world: (%f %f) -> (%f %f)\n", view_position.x, view_position.y, world_position.x, world_position.y);

    return world_position;
}

bool rects_are_colliding(Vector2 first_position,
                         Vector2 first_size,
                         Vector2 second_position,
                         Vector2 second_size)
{
    float diff_x = fabs(first_position.x - second_position.x);
    float diff_y = fabs(first_position.y - second_position.y);

    return diff_x < (first_size.x + second_size.x) / 2
        && diff_y < (first_size.y + second_size.y) / 2;
}
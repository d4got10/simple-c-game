#pragma once

#include "raylib.h"

typedef struct {
    Vector2 position;
    Vector2 size;
} MyTransform;

typedef struct {
    MyTransform transform;
    Vector2 velocity;
    float desired_horizontal_speed;
    float max_horizontal_speed;
    float horizontal_steering_force;
    float jump_velocity;
    bool wants_to_jump;
    bool is_grounded;
} Player;

typedef struct {
    float ground_level;
    float gravity_force;
    MyTransform* platforms;
    int platform_count;
} LevelInfo;

typedef struct{
    const char* path;
    Texture2D texture;
} Resource;

typedef struct {
    Resource player;
    Resource platform;
} ResourceContainer;

typedef struct{
    ResourceContainer resources;
    Player player;
    Vector2 camera_position;
    LevelInfo level_info;
    float simulation_step;
} GameState;
#pragma once

#include "raylib.h"

typedef enum {
    DEFAULT,
    JUMPER,
    BOUNCY
} PlatformType;

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
    bool wants_to_drop;
    bool is_grounded;
} Player;

typedef struct {
    PlatformType type;
    MyTransform transform;
} Platform;

typedef struct {
    float ground_level;
    float gravity_force;
    Platform* platforms;
    int platform_count;
} LevelInfo;

typedef struct{
    const char* path;
    Texture2D texture;
} Resource;

typedef struct {
    Resource player;
    Resource platform;
    Resource platform_jumper;
    Resource platform_bouncy;
} ResourceContainer;

typedef struct {
    bool manual_mode;
    bool should_step_forward;
} Debugging;

typedef struct{
    ResourceContainer resources;
    Player player;
    Vector2 camera_position;
    LevelInfo level_info;
    float simulation_step;
    Debugging debugging;
} GameState;
#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "utils.h"
#include "graphics.h"
#include "core.h"
#include "input.h"
#include "simulation.h"
#include "resources.h"
#include <stdlib.h>

int main(void)
{
    const int screen_width = 800;
    const int screen_height = 450;

    InitWindow(screen_width, screen_height, "My Window");
    SetTargetFPS(60);

    const char *working_directory = GetWorkingDirectory();
    if (working_directory == NULL) {
        printf("Failed to get working directory.\n");
        return 1;
    }

    const char *resources_directory = TextFormat("%s\\resources", working_directory);
    ResourceContainer resources = load_resources(resources_directory);

    Player player = {
        transform: {
            position: {0, 0},
            size: {50, 50}
        },
        velocity: {},
        jump_velocity: 1000.0f,
        desired_horizontal_speed: 0.0f,
        max_horizontal_speed: 500.0f,
        horizontal_steering_force: 3000.0f
    };

    LevelInfo level_info = {
        ground_level: 0,
        gravity_force: -3000.0f,
    };

    level_info.platform_count = 10;
    level_info.platforms = (MyTransform*) malloc(level_info.platform_count * sizeof(MyTransform));

    for(int i = 0; i < level_info.platform_count; i++){
        level_info.platforms[i].position.x = (2 * (i & 1) - 1) * 100;
        level_info.platforms[i].position.y = (i + 1) * 125;
        level_info.platforms[i].size.x = 200;
        level_info.platforms[i].size.y = 50;
    }

    GameState game_state =
    {
        resources: resources,
        player: player,
        camera_position: {},
        level_info: level_info,
        simulation_step: 0.02f
    };

    while(!WindowShouldClose())
    {
        game_state = apply_input(game_state);
        game_state = simulate(game_state);
        draw_graphics(game_state);
    }

    return 0;
}


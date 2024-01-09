#include "simulation.h"
#include "raymath.h"
#include "utils.h"

GameState simulate(GameState game_state)
{
    LevelInfo level_info = game_state.level_info;
    Player player = game_state.player;
    float simulation_step = game_state.simulation_step;

    float max_distance = player.horizontal_steering_force * simulation_step;
    float steering = Clamp(player.desired_horizontal_speed - player.velocity.x, -max_distance, max_distance);
    player.velocity.x += steering;
    player.velocity.x = Clamp(player.velocity.x, -player.max_horizontal_speed, player.max_horizontal_speed);

    if(player.wants_to_jump && player.is_grounded)
        player.velocity.y += player.jump_velocity;
    player.velocity.y += level_info.gravity_force * simulation_step;

    Vector2 movement = Vector2Scale(player.velocity, game_state.simulation_step);
    Vector2 player_next_position = Vector2Add(player.transform.position, movement);

    player.is_grounded = false;

    if (player.velocity.y < 0 && player_next_position.y <= level_info.ground_level){
        player.transform.position.y = level_info.ground_level - 1;
        player.velocity.y = 0;
        player.is_grounded = true;
    }
    game_state.player = player;

    game_state = simulate_collisions(game_state);

    player = game_state.player;
    movement = Vector2Scale(player.velocity, simulation_step);
    player.transform.position = Vector2Add(player.transform.position, movement);
    game_state.player = player;

    game_state = camera_follow_player(game_state);

    // printf("simulated position: (%f, %f)\n", player.transform.position.x, player.transform.position.y);

    return game_state;
}

GameState simulate_collisions(GameState game_state)
{
    Player player = game_state.player;
    if(player.velocity.y > 0)
        return game_state;

    LevelInfo level_info = game_state.level_info;

    Vector2 movement = Vector2Scale(player.velocity, game_state.simulation_step);
    Vector2 player_next_position = Vector2Add(player.transform.position, movement);

    MyTransform* platforms = level_info.platforms;

    for(int i = 0; i < level_info.platform_count; i++){
        if(!rects_are_colliding(platforms[i].position, platforms[i].size,
                                player_next_position, player.transform.size)) continue;

        float next_position_bottom_y = player_next_position.y - player.transform.size.y / 2;
        float curr_position_bottom_y = player.transform.position.y - player.transform.size.y / 2;
        float platform_top_position = platforms[i].position.y + platforms[i].size.y / 2;

        if(next_position_bottom_y < platform_top_position
           && curr_position_bottom_y >= platform_top_position)
        {
            player.velocity.y = 0;
            player.transform.position.y = platforms[i].position.y
                                + platforms[i].size.y / 2
                                + player.transform.size.y / 2;
            player.is_grounded = true;
        }

        //printf("hit\n");
    }

    //printf("%f\n", player.velocity.y);
    game_state.player = player;
    return game_state;
}

GameState camera_follow_player(GameState game_state)
{
    Player player = game_state.player;

    game_state.camera_position.x = player.transform.position.x;
    game_state.camera_position.y = Clamp(game_state.camera_position.y,
                                         player.transform.position.y,
                                         player.transform.position.y + 175);

    return game_state;
}
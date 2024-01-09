#include "input.h"

GameState apply_input(GameState game_state)
{
    Player player = game_state.player;
    float speed = player.max_horizontal_speed;
    player.desired_horizontal_speed = 0;
    if (IsKeyDown(KEY_RIGHT)) player.desired_horizontal_speed += speed;
    if (IsKeyDown(KEY_LEFT))  player.desired_horizontal_speed -= speed;
    if (IsKeyDown(KEY_W)) game_state.level_info.ground_level += 10.0f;
    if (IsKeyDown(KEY_S)) game_state.level_info.ground_level -= 10.0f;

    player.wants_to_jump = IsKeyDown(KEY_UP);

    game_state.player = player;

    return game_state;
}
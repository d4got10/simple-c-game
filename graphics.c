#include "graphics.h"
#include "utils.h"
#include "raymath.h"

void draw_graphics(GameState game_state)
{
        BeginDrawing();

        ClearBackground(GRAY);

        draw_ground(game_state);
        draw_platforms(game_state);
        draw_player(game_state);

        draw_debug_info(game_state);

        EndDrawing();
}

void draw_player(GameState game_state)
{
    MyTransform player_transform = game_state.player.transform;
    Vector2 player_view_position = world_to_view(player_transform.position,
                                                 game_state.camera_position);
    Vector2 pos = {
        x: player_view_position.x - player_transform.size.x / 2,
        y: player_view_position.y - player_transform.size.y / 2,
    };

    Texture2D player_texture = game_state.resources.player.texture;
    float scale = fminf(player_transform.size.x / player_texture.width,
                        player_transform.size.y / player_texture.height);
    DrawTextureEx(player_texture, pos, 0.0, scale, WHITE);
}

void draw_debug_info(GameState game_state)
{
    const char* position_text = TextFormat("Position: (%.2f, %.2f)",
                      game_state.player.transform.position.x,
                      game_state.player.transform.position.y);
    DrawText(position_text, 0, 0, 20, WHITE);

    const char* velocity_text = TextFormat("Velocity: (%.2f, %.2f)",
                        game_state.player.velocity.x,
                        game_state.player.velocity.y);
    DrawText(velocity_text, 0, 20, 20, WHITE);
}

void draw_ground(GameState game_state)
{
    int ground_level = game_state.level_info.ground_level;
    int height = GetScreenHeight();
    int width = GetScreenWidth();

    Vector2 ground_position = {
        x: game_state.player.transform.position.x - width / 2,
        y: ground_level - game_state.player.transform.size.y / 2
    };
    Vector2 ground_view_position = world_to_view(ground_position, game_state.camera_position);
    DrawRectangle(ground_view_position.x, ground_view_position.y, width, height, BROWN);
}

void draw_platforms(GameState game_state)
{
    LevelInfo level_info = game_state.level_info;
    Platform* platforms = level_info.platforms;
    int n = level_info.platform_count;
    // printf("%d\n", sizeof(MyTransform));
    for(int i = 0; i < n; i++){
        Vector2 view_position = world_to_view(platforms[i].transform.position,
                                              game_state.camera_position);
        Vector2 pos = {
            x: view_position.x - platforms[i].transform.size.x / 2,
            y: view_position.y - platforms[i].transform.size.y / 2,
        };
        Texture2D platform_texture;
        switch (platforms[i].type) {
            case DEFAULT:
                platform_texture = game_state.resources.platform.texture;
                break;
            case JUMPER:
                platform_texture = game_state.resources.platform_jumper.texture;
                break;
        }

        float scale = fminf(platforms[i].transform.size.x / platform_texture.width,
                            platforms[i].transform.size.y / platform_texture.height);

        DrawTextureEx(platform_texture, pos, 0.0, scale, WHITE);
    }
}
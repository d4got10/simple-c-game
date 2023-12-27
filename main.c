#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

typedef struct {
    Vector2 position;
    Vector2 size;    
} MyTransform;

typedef struct {
    MyTransform transform;
    Vector2 velocity;
    Vector2 desired_velocity;
    float max_speed;
    float steering_force;
} Player;

typedef struct {
    float ground_level;
    float gravity_force;
} LevelInfo;

typedef struct{
    Player player;
    LevelInfo level_info;
} GameState;

GameState apply_input(GameState);
GameState simulate(GameState, float simulation_step);
void draw_graphics(GameState);
Vector2 world_to_view(Vector2 world_position);

int main(void) 
{   
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    InitWindow(screenWidth, screenHeight, "My Window");
    SetTargetFPS(60);
    
    Player player = {
        transform: {
            position: {0, 0},
            size: {100, 100}
        },
        velocity: {},
        desired_velocity: {},
        max_speed: 500.0f,
        steering_force: 3000.0f
    };
    
    LevelInfo level_info = {
        ground_level: -100.0f,
        gravity_force: -1000.0f,
    };
    
    GameState game_state = 
    {
        player: player,
        level_info: level_info
    };
    
    
    while(!WindowShouldClose())
    {
        game_state = apply_input(game_state);
        game_state = simulate(game_state, 0.02f);
        draw_graphics(game_state);        
    }

    return 0;
}

GameState simulate(GameState game_state, float simulation_step)
{
    LevelInfo level_info = game_state.level_info;
    Player player = game_state.player;
    
    float prevVelocityY = player.velocity.y;
    
    float max_distance = player.steering_force * simulation_step;
    player.velocity = Vector2MoveTowards(player.velocity, player.desired_velocity, max_distance);
    player.velocity = Vector2ClampValue(player.velocity, 0, player.max_speed);
    
    Vector2 movement = Vector2Scale(player.velocity, simulation_step);
    player.transform.position = Vector2Add(player.transform.position, movement);
    
    player.velocity.y = prevVelocityY + level_info.gravity_force * simulation_step;
    
    if (player.transform.position.y < level_info.ground_level){
        player.transform.position.y = level_info.ground_level;
        player.velocity.y = 0; 
    }
    
    game_state.player = player;
    return game_state;
}

GameState apply_input(GameState game_state)
{
    Player player = game_state.player;
    float speed = player.max_speed;
    player.desired_velocity.x = 0;
    player.desired_velocity.y = 0;
    if (IsKeyDown(KEY_RIGHT)) player.desired_velocity.x += speed;
    if (IsKeyDown(KEY_LEFT))  player.desired_velocity.x -= speed;
    if (IsKeyDown(KEY_UP))    player.desired_velocity.y += speed;
    if (IsKeyDown(KEY_DOWN))  player.desired_velocity.y -= speed;
    game_state.player = player;
    
    return game_state;    
}

void draw_graphics(GameState game_state)
{
        BeginDrawing();

        ClearBackground(GRAY);
        MyTransform player_transform = game_state.player.transform;
        Vector2 player_view_position = world_to_view(player_transform.position);
        DrawRectangle(player_view_position.x, player_view_position.y, player_transform.size.x, player_transform.size.y, YELLOW);
        
        EndDrawing();
}

Vector2 world_to_view(Vector2 world_position)
{
    int screen_height = GetScreenHeight();
    int screen_width = GetScreenWidth();
    
    Vector2 view_position = {
        x: world_position.x + screen_width / 2,
        y: -world_position.y + screen_height / 2
    };
    
    return view_position;
}
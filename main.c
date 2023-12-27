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
    Player player;
    Vector2 camera_position;
    LevelInfo level_info;
    float simulation_step;
} GameState;

//input
GameState apply_input(GameState);

//simulation
GameState simulate(GameState);
GameState simulate_collisions(GameState game_state);
GameState camera_follow_player(GameState game_state);
bool rects_are_colliding(Vector2 first_position, Vector2 first_size, Vector2 second_position, Vector2 second_size);

//graphics
void draw_graphics(GameState);
void draw_platforms(GameState game_state);

//utils
Vector2 world_to_view(Vector2 world_position, Vector2 camera_position);
Vector2 view_to_world(Vector2 view_position, Vector2 camera_position);

int main(void) 
{   
    const int screen_width = 800;
    const int screen_height = 450;
    
    InitWindow(screen_width, screen_height, "My Window");
    SetTargetFPS(60);
    
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
        if(player.transform.position.y + player.transform.size.y / 2 < platforms[i].position.y) continue;
        if(!rects_are_colliding(platforms[i].position, platforms[i].size, 
                                player_next_position, player.transform.size)) continue;
        
        player.velocity.y = 0;
        player.transform.position.y = platforms[i].position.y 
                            + platforms[i].size.y / 2 
                            + player.transform.size.y / 2 - 1;
        player.is_grounded = true;
                            
        //printf("hit\n");
    }
    
    //printf("%f\n", player.velocity.y);
    game_state.player = player;
    return game_state;
}

bool rects_are_colliding(Vector2 first_position,
                         Vector2 first_size,
                         Vector2 second_position,
                         Vector2 second_size)
{
    float diff_x = abs(first_position.x - second_position.x);
    float diff_y = abs(first_position.y - second_position.y);
    
    return diff_x < (first_size.x + second_size.x) / 2 
        && diff_y < (first_size.y + second_size.y) / 2;
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

void draw_graphics(GameState game_state)
{
        BeginDrawing();

        ClearBackground(GRAY);
        MyTransform player_transform = game_state.player.transform;
        Vector2 player_view_position = world_to_view(player_transform.position, 
                                                     game_state.camera_position);
        DrawRectangle(player_view_position.x - player_transform.size.x / 2, 
                      player_view_position.y - player_transform.size.y / 2, 
                      player_transform.size.x, 
                      player_transform.size.y, 
                      YELLOW);
                      
        int ground_level = game_state.level_info.ground_level; 
        int height = GetScreenHeight();
        int width = GetScreenWidth();
        
        Vector2 ground_position = {
            x: game_state.player.transform.position.x - width / 2, 
            y: ground_level - game_state.player.transform.size.y / 2
        };
        Vector2 ground_view_position = world_to_view(ground_position, game_state.camera_position);
        DrawRectangle(ground_view_position.x, ground_view_position.y, width, height, BROWN);
        
        draw_platforms(game_state);
        
        char text[64];
                
        sprintf(text, "Position: (%.2f, %.2f)", 
                          player_transform.position.x, 
                          player_transform.position.y);
        DrawText(text, 0, 0, 20, WHITE);
        
        DrawText(TextFormat("Velocity: (%.2f, %.2f)", 
                            game_state.player.velocity.x,
                            game_state.player.velocity.y), 
                            0, 20, 20, WHITE);
        
        EndDrawing();
}

void draw_platforms(GameState game_state)
{
    LevelInfo level_info = game_state.level_info;
    MyTransform* platforms = level_info.platforms;
    int n = level_info.platform_count;
    // printf("%d\n", sizeof(MyTransform));
    for(int i = 0; i < n; i++){
        Vector2 view_position = world_to_view(platforms[i].position, game_state.camera_position);
        DrawRectangle(view_position.x - platforms[i].size.x / 2,
                      view_position.y - platforms[i].size.y / 2,
                      platforms[i].size.x,
                      platforms[i].size.y,
                      BROWN);
    }
}

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
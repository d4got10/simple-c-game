#pragma once

#include "core.h"

GameState simulate(GameState game_state);
GameState simulate_collisions(GameState game_state);
GameState camera_follow_player(GameState game_state);
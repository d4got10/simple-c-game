set RAYLIB_PATH=C:/Users/Danik/apps/raylib

gcc main.c -o bin/game.exe -O2 -Wall -I%RAYLIB_PATH%/src/external -I%RAYLIB_PATH%/src -I%RAYLIB_PATH%/src/extras -L./raylib/lib -DPLATFORM_DESKTOP -lraylib -lopengl32 -lgdi32 -lwinmm
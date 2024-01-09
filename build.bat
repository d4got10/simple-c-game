@echo off

set RAYLIB_PATH=C:/Users/Danik/apps/raylib

:: Создание временной директории
mkdir temp

:: Компиляция utils.c
gcc -c utils.c -o temp/utils.o -O2 -Wall -I%RAYLIB_PATH%/src/external -I%RAYLIB_PATH%/src -I%RAYLIB_PATH%/src/extras -L./raylib/lib -DPLATFORM_DESKTOP -lraylib -lopengl32 -lgdi32 -lwinmm
if %ERRORLEVEL% neq 0 goto :cleanup

:: Компиляция graphics.c
gcc -c graphics.c -o temp/graphics.o -O2 -Wall -I%RAYLIB_PATH%/src/external -I%RAYLIB_PATH%/src -I%RAYLIB_PATH%/src/extras -L./raylib/lib -DPLATFORM_DESKTOP -lraylib -lopengl32 -lgdi32 -lwinmm
if %ERRORLEVEL% neq 0 goto :cleanup

:: Компиляция input.c
gcc -c input.c -o temp/input.o -O2 -Wall -I%RAYLIB_PATH%/src/external -I%RAYLIB_PATH%/src -I%RAYLIB_PATH%/src/extras -L./raylib/lib -DPLATFORM_DESKTOP -lraylib -lopengl32 -lgdi32 -lwinmm
if %ERRORLEVEL% neq 0 goto :cleanup

:: Компиляция resources.c
gcc -c resources.c -o temp/resources.o -O2 -Wall -I%RAYLIB_PATH%/src/external -I%RAYLIB_PATH%/src -I%RAYLIB_PATH%/src/extras -L./raylib/lib -DPLATFORM_DESKTOP -lraylib -lopengl32 -lgdi32 -lwinmm
if %ERRORLEVEL% neq 0 goto :cleanup

:: Компиляция simulation.c
gcc -c simulation.c -o temp/simulation.o -O2 -Wall -I%RAYLIB_PATH%/src/external -I%RAYLIB_PATH%/src -I%RAYLIB_PATH%/src/extras -L./raylib/lib -DPLATFORM_DESKTOP -lraylib -lopengl32 -lgdi32 -lwinmm
if %ERRORLEVEL% neq 0 goto :cleanup

:: Сборка main.c
gcc main.c temp/utils.o temp/graphics.o temp/resources.o temp/input.o temp/simulation.o -o bin/game.exe -O2 -Wall -I%RAYLIB_PATH%/src/external -I%RAYLIB_PATH%/src -I%RAYLIB_PATH%/src/extras -L./raylib/lib -DPLATFORM_DESKTOP -lraylib -lopengl32 -lgdi32 -lwinmm
if %ERRORLEVEL% neq 0 goto :cleanup

:cleanup
rd /s /q temp
exit /b %ERRORLEVEL%
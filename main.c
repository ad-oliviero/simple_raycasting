#include <stdio.h>
#include "raylib/include/raylib.h"
#include "headers/config.h"
#include "headers/player.h"
#include "headers/rendering.h"
#include "headers/hud.h"

double d_time;
extern Player local_player;
extern Settings local_settings;
void log_level() {}

int main()
{
	// initializing raylib
	SetTraceLogCallback(log_level);
	// SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetConfigFlags(0x00034001); //GLFW_CURSOR_NORMAL
	InitWindow(WIDTH, HEIGHT, TITLE);
	// SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
	// HideCursor();
	DisableCursor();
	// initializing local variables for player and settings
	init_player(&local_player, &local_settings, 100, 50, -45 * PI / 180);
	init_settings(&local_settings, "Name", 70, 360, 30, 38);

	// main game loop
	while (!WindowShouldClose())
	{
		d_time = GetFrameTime();
		// drawing
		BeginDrawing();
		ClearBackground(BLACK);

		view_3d(&local_player, &local_settings);
		mini_map();
		player(&local_player, &local_settings);
		view(&local_player, &local_settings);

		DrawFPS(GetScreenWidth() - 80, GetScreenHeight() - 20);
		EndDrawing();
	}

	// closing program
	CloseWindow();
	printf("Program closed successfully!\n");
	return 0;
}
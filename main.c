#include <stdio.h>
#include <malloc.h>
#include "raylib/include/raylib.h"
#include "headers/config.h"
#include "headers/player.h"
#include "headers/rendering.h"
#include "headers/hud.h"

double d_time;
void log_level() {}

int main()
{
	const int tmp_ray_count = 360;
	Player *local_player = (Player *)malloc(sizeof(Player));		 // + sizeof(int[tmp_ray_count]);
	Settings *local_settings = (Settings *)malloc(sizeof(Settings)); // + sizeof(int[tmp_ray_count]);
	// initializing raylib
	SetTraceLogCallback(log_level);
	// SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetConfigFlags(0x00034001); //GLFW_CURSOR_NORMAL
	InitWindow(WIDTH, HEIGHT, TITLE);
	// SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()) * 2);
	// HideCursor();
	DisableCursor();
	// initializing local variables for player and settings
	init_settings(local_settings, "Name", 100, tmp_ray_count, 40, 38);
	init_player(local_player, local_settings, 100, 50, -45 * PI / 180);

	// main game loop
	while (!WindowShouldClose())
	{
		d_time = GetFrameTime();
		// drawing
		BeginDrawing();
		ClearBackground(BLACK);

		view_3d(/* local_player,  */ local_settings);
		draw_hud();
		view(local_player, local_settings);
		player(local_player, local_settings);

		DrawFPS(GetScreenWidth() - 80, GetScreenHeight() - 20);
		EndDrawing();
	}

	// closing program
	CloseWindow();
	printf("Program closed successfully!\n");
	return 0;
}
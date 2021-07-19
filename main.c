#include <stdio.h>
#include <malloc.h>
#include "lib/raylib/include/raylib.h"
#include "lib/raygui/src/raygui.h"
#include "headers/config.h"
#include "headers/player.h"
#include "headers/rendering.h"
#include "headers/hud.h"

double d_time;
Vector2 old_player_position = {0};
// some global bools
bool display_settings = false, movement_enabled = true;

void log_level() {}

int main(int argc, char **argv)
{
#ifdef DEBUG
	printf("Debugging started with %i args: ", argc - 1);
	for (int i = 0; i < argc; i++)
		printf("%s ", argv[i]);
	printf("\n");
#endif
	Player *local_player = (Player *)malloc(sizeof(Player));
	Settings *local_settings = (Settings *)malloc(sizeof(Settings));
	// initializing raylib
	SetTraceLogCallback(log_level);
	// SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetConfigFlags(0x00034001); //GLFW_CURSOR_NORMAL
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()) + 1);
	// HideCursor();
	DisableCursor();
	GuiLoadStyle("./lib/raygui/styles/cyber/cyber.rgs");
	SetExitKey(KEY_NULL); // disabling quit on "KEY_ESCAPE" press
	// initializing local variables for player and settings
	load_settings(local_settings, "Name", 100, 360, 40, 38, SETTINGS_FILE_LOCATION);
	init_player(local_player, local_settings, 100, 50, -45 * PI / 180);

	// main game loop
	while (!WindowShouldClose())
	{
		d_time = GetFrameTime();
		old_player_position = local_player->position;
		// drawing
		BeginDrawing();

		view_3d(local_player, local_settings);
		view(local_player, local_settings);
		draw_hud(local_player, local_settings);
		player(local_player, local_settings);

		DrawFPS(GetScreenWidth() - 80, GetScreenHeight() - 20);
		ClearBackground(BLACK);
		EndDrawing();
	}

	// closing program
	CloseWindow();
#ifdef DEBUG
	printf("%s closed correctly!\n", argv[0]);
#endif
	return 0;
}
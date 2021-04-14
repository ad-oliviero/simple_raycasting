#include <raylib.h>
#include <stdio.h>
#include "headers/config.h"
#include "headers/player.h"
#include "headers/rendering.h"
#include "headers/hud.h"

double d_time;
extern Player local_player;
void log_level() {}

int main() {
	SetTraceLogCallback(log_level);
	//SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(GetMonitorRefreshRate(0));
	HideCursor();
	init_player(&local_player, 100, 50, -45 * PI / 180, 90, 360, 50);

	while (!WindowShouldClose()) {
		d_time = GetFrameTime();
		BeginDrawing();
		ClearBackground(BLACK);
		view_3d(&local_player);
		mini_map();
		player();
		view(&local_player);
		DrawFPS(WIDTH - 100, HEIGHT - 20);
		EndDrawing();
	}
	CloseWindow();
	printf("Program closed successfully!\n");
	return 0;
}
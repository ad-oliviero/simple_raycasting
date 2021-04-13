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

	init_player(&local_player, 100, 50, 0, 90, 360, 150);

	while (!WindowShouldClose()) {
		d_time = GetFrameTime();
		BeginDrawing();
		ClearBackground(BLACK);
		view_3d();
		mini_map();
		player();
		view();
		DrawFPS(WIDTH - 100, HEIGHT - 20);
		EndDrawing();
	}
	CloseWindow();
	printf("Program closed successfully!\n");
	return 0;
}
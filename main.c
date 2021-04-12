#include <raylib.h>
#include <stdio.h>
#include "headers/config.h"
#include "headers/player.h"
#include "headers/rendering.h"
#include "headers/hud.h"

double d_time;
void log_level() {}

int main() {
	SetTraceLogCallback(log_level);
	//SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(WIDTH, HEIGHT, TITLE);

	while (!WindowShouldClose()) {
		d_time = GetFrameTime();
		BeginDrawing();
		ClearBackground(BLACK);
		view_3d();
		mini_map();
		view();
		player();
		DrawFPS(WIDTH - 100, HEIGHT - 20);
		EndDrawing();
	}
	CloseWindow();
	printf("Program closed successfully!\n");
	return 0;
}
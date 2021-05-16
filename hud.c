#include "lib/raylib/include/raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "headers/config.h"
#include "headers/player.h"
#include "headers/hud.h"
#include "headers/map.h"
#include "headers/rendering.h"

extern Vector2 linestart[128], linend[128],
	border_s[4], border_e[4],
	map1_s[4], map1_e[4];

Vector2 linestart_s[MAP_SIDES] = {0}, linend_s[MAP_SIDES] = {0};
extern bool display_settings, movement_enabled;

void draw_hud(Player *player, Settings *settings)
{
	DrawLineEx((Vector2){WIDTH / 2, HEIGHT / 2 - 10}, (Vector2){WIDTH / 2, HEIGHT / 2 + 10}, 2, WHITE);
	DrawLineEx((Vector2){WIDTH / 2 - 10, HEIGHT / 2}, (Vector2){WIDTH / 2 + 10, HEIGHT / 2}, 2, WHITE);
	draw_map(player, settings);
	if (IsKeyPressed(256))
	{
		display_settings = !display_settings;
		movement_enabled = !movement_enabled;
		DisableCursor();
	}
	if (display_settings)
	{
		EnableCursor();
		draw_settings();
	}
	// DrawCircle(WIDTH / 2, HEIGHT / 2, 4, BLACK);
}

void draw_map(/*Map *map*/ Player *player, Settings *settings)
{
	DrawRectangleRec((Rectangle){10, 10, 225, 150}, WHITE); // map background
	// for (int i = 0; i < 4; i++)DrawLineEx(border_s[i], border_e[i], 3, RED); // map background
	memcpy(linestart_s, linestart, MAP_SIDES);
	memcpy(linend_s, linend, MAP_SIDES);
	for (int i = 0; i < MAP_SIDES; i++)
	{
		linestart_s[i].x /= 2;
		linestart_s[i].y /= 2;
		linend_s[i].x /= 2;
		linend_s[i].y /= 2;
		linestart_s[i].x += 35;
		linestart_s[i].y += 45;
		linend_s[i].x += 35;
		linend_s[i].y += 45;
	}

	for (int i = 0; i < MAP_SIDES; i++)
		DrawLineEx(linestart_s[i], linend_s[i], 1, BLACK);
	// DrawLineEx(map1_s[0], map1_e[0], 1, WHITE);
	// DrawLineEx(map1_s[1], map1_e[1], 1, WHITE);
	// DrawLineEx(map1_s[2], map1_e[2], 1, WHITE);
	// DrawLineEx(map1_s[3], map1_e[3], 1, WHITE);
	p_draw_on_map(player, settings);
}

void draw_settings()
{
	DrawRectangle(0, 0, WIDTH, HEIGHT, ColorAlpha(BLACK, 0.9)); // obfuscate background
																// DrawRectangle()
}
#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include "headers/config.h"
#include "headers/map.h"
#include "headers/player.h"

extern Vector2 linestart[128], linend[128],
	border_s[4], border_e[4],
	map1_s[4], map1_e[4];

Vector2 linestart_s[MAP_SIDES] = {0}, linend_s[MAP_SIDES] = {0};

int draw_map();

int mini_map()
{
	DrawRectangleRec((Rectangle){10, 10, 225, 150}, WHITE); // map background
	//for (int i = 0; i < 4; i++) DrawLineEx(border_s[i], border_e[i], 3, RED); // map background
	draw_map();
	return 0;
}

int draw_map(/*Map *map*/)
{
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
	/* DrawLineEx(map1_s[0], map1_e[0], 1, WHITE);
	DrawLineEx(map1_s[1], map1_e[1], 1, WHITE);
	DrawLineEx(map1_s[2], map1_e[2], 1, WHITE);
	DrawLineEx(map1_s[3], map1_e[3], 1, WHITE); */
	return 0;
}

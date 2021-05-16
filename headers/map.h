/*
In this file you must define all the map lines (in arrays) or just create a function to generate it randomly
*/
#include "../lib/raylib/include/raylib.h"

typedef struct
{
	Vector2 start[128];
	Vector2 end[128];
} Map;

Vector2 linestart[128] = {{10, 10}, {10, 100}, {100, 100}, {100, 200}, {225, 200}, {225, 5}, {350, 5}, {350, -40}, {100, -40}, {100, 10}};
Vector2 linend[128] = {{10, 100}, {100, 100}, {100, 200}, {225, 200}, {225, 5}, {350, 5}, {350, -40}, {100, -40}, {100, 10}, {10, 10}};

Vector2 border_s[4] = {{10, 10}, {10, 160}, {10, 10}, {235, 10}};
Vector2 border_e[4] = {{10, 160}, {235, 160}, {235, 10}, {235, 160}};
//{10, 10, 225, 150}

Vector2 map1_s[4] = {{300, 300}, {600, 300}, {300, 600}, {300, 300}};
Vector2 map1_e[4] = {{300, 600}, {600, 600}, {600, 600}, {600, 300}};
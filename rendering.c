#include "raylib/include/raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "headers/config.h"
#include "headers/player.h"

extern Vector2 linestart[128], linend[128],
	linestart_s[MAP_SIDES], linend_s[MAP_SIDES],
	border_s[4], border_e[4],
	map1_s[4], map1_e[4];

void view(Player *player, Settings *settings)
{ // here we raycast the rays and get the closest hit
	for (int i = 0; i < settings->ray_count; i++)
	{
		Vector2 closest = {0}, collision_point;
		float lowest_value = INFINITY;
		for (int j = 0; j < MAP_SIDES; j++)
		{
			collision_point = (Vector2){0, 0};
			// cast_rays(player->rays[i], player->position, map1_s[j], map1_e[j], &collision_point);
			// cast_rays(player->rays[i], player->position, border_s[0], border_e[0], &collision_point);
			// cast_rays(player->rays[i], player->position, border_s[1], border_e[1], &collision_point);
			// cast_rays(player->rays[i], player->position, border_s[2], border_e[2], &collision_point);
			// cast_rays(player->rays[i], player->position, border_s[3], border_e[3], &collision_point);
			cast_rays(player->rays[i], player->position, linestart_s[j], linend_s[j], &collision_point);
			if (collision_point.x && collision_point.y)
			{
				float distance = sqrt(pow(player->position.x - collision_point.x, 2) + pow(player->position.y - collision_point.y, 2));
				if (distance < lowest_value)
				{
					lowest_value = distance;
					closest = collision_point;
				}
			}
		}
		DrawLineEx(player->position, closest, 1 * (closest.x && closest.y), RED);
		settings->scene[i] = lowest_value * (lowest_value != NAN);
	}
	return;
}

float map(float value, float from1, float to1, float from2, float to2)
{ // just a math function that i had to write myself.
	return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
}

void view_3d(Player *player, Settings *settings)
{ // here i get the intersections and transform them into a "3d" view
	for (int i = 0; i < settings->ray_count; i++)
	{
		const float scene_width = (float)WIDTH / settings->ray_count;
		const float alpha = map(settings->scene[i], 0, 200, 1, -0.1);

		// remove fisheye effect (not so much)
		const float norm_scene = settings->scene[i] * scene_width / 2 - cos(player->ray_angle_from_center[i]) / (settings->scene[i] * scene_width * 9);

		// drawing every "3d" line
		DrawLineEx((Vector2){i * scene_width + 1, norm_scene * (norm_scene < HEIGHT / 2) + HEIGHT / 2 * (norm_scene > HEIGHT / 2)},
				   (Vector2){i * scene_width + 1, HEIGHT - (norm_scene * (norm_scene < HEIGHT / 2) + HEIGHT / 2 * (norm_scene > HEIGHT / 2))},
				   scene_width,
				   ColorAlpha(GRAY, alpha));
	}
}

void cast_rays(Vector2 ray_s, Vector2 ray_e, Vector2 wall_s, Vector2 wall_e, Vector2 *collision_point)
{ /* https://web.archive.org/web/20060911055655/http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
     also i know that raylib has a CheckCollisionLines() function, but i can't get it working, fell free to uncomment it and try. */
	// CheckCollisionLines(ray_s, ray_e, wall_s, wall_e, collision_point);
	float den = (wall_e.y - wall_s.y) * (ray_e.x - ray_s.x) - (wall_e.x - wall_s.x) * (ray_e.y - ray_s.y),
		  num_a = (wall_e.x - wall_s.x) * (ray_s.y - wall_s.y) - (wall_e.y - wall_s.y) * (ray_s.x - wall_s.x),
		  num_b = (ray_e.x - ray_s.x) * (ray_s.y - wall_s.y) - (ray_e.y - ray_s.y) * (ray_s.x - wall_s.x);
	if (den == 0)
		return; // parallel lines
	float u_a = num_a / den,
		  u_b = num_b / den;
	if (u_a >= 0 && u_a <= 1 && u_b >= 0 && u_b <= 1)
	{
		collision_point->x = ray_s.x + u_a * (ray_e.x - ray_s.x);
		collision_point->y = ray_s.y + u_a * (ray_e.y - ray_s.y);
	}
	return;
}
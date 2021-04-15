#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "headers/config.h"
#include "headers/player.h"

extern Vector2 linestart[128], linend[128],
	linestart_s[MAP_SIDES], linend_s[MAP_SIDES],
	border_s[4], border_e[4],
	map1_s[4], map1_e[4];
float scene[360];

void view(Player *player)
{
	for (int i = 0; i < player->ray_count; i++)
	{
		Vector2 closest = {0}, collision_point;
		float lowest_value = INFINITY;
		for (int j = 0; j < MAP_SIDES; j++)
		{
			collision_point = (Vector2){0, 0};
			//cast_rays(player->rays[i], player->position, map1_s[j], map1_e[j]);
			/*cast_rays(player->rays[i], player->position, border_s[0], border_e[0], &collision_point);
			cast_rays(player->rays[i], player->position, border_s[1], border_e[1], &collision_point);
			cast_rays(player->rays[i], player->position, border_s[2], border_e[2], &collision_point);
			cast_rays(player->rays[i], player->position, border_s[3], border_e[3], &collision_point);*/
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
		scene[i] = lowest_value * (lowest_value != NAN);
	}
	return;
}

float map(float value, float from1, float to1, float from2, float to2)
{
	return (value - from1) / (to1 - from1) * (to2 - from2) + from2;
}

void view_3d(Player *player)
{
	for (int i = 0; i < (int)(sizeof(scene) / sizeof(scene[0])); i++)
	{
		const float scene_width = WIDTH / (player->ray_count * player->fov * (PI / 180)) * 1.58;
		const float alpha = map(scene[i], 0, 150, 1, 0);

		// remove fisheye effect (not so much)
		const float ray_angle = player->angle + (i * 0.018);
		const float norm_scene = scene[i] * 3 + sin(ray_angle);

		DrawLineEx((Vector2){i * scene_width, norm_scene}, (Vector2){i * scene_width, HEIGHT - norm_scene}, scene_width, ColorAlpha(GRAY, alpha));
	}
}
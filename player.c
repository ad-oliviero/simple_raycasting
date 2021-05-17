#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lib/raylib/include/raylib.h"
#include "headers/config.h"
#include "headers/player.h"

extern bool movement_enabled;

// player by reference, x and y coordinates, angle
void init_player(Player *player, Settings *settings, float x, float y, float angle)
{
	player->position = (Vector2){x, y};
	player->angle = angle;
	player->ray_length = 200;
	for (int i = 0; i < settings->ray_count; i++)
	{
		player->ray_angle_from_start[i] = (settings->fov * PI / 180.0f * i / settings->ray_count) + player->angle;
		player->rays[i].x = player->ray_length * cosf(player->ray_angle_from_start[i]) + player->position.x;
		player->rays[i].y = player->ray_length * sinf(player->ray_angle_from_start[i]) + player->position.y;
	}
	for (int i = 1; i < settings->ray_count / 2 + 1; i++)
	{
		player->ray_angle_from_center[180 - i] = (i * PI / 180);
		player->ray_angle_from_center[180 + i] = (i * PI / 180);
	}
	player->distance_between_rays = settings->fov * PI / 180.0f / settings->ray_count;
}

void init_settings(Settings *settings, const char *user_name, float fov, int ray_count, float speed, float mouse_sensibility)
{
	settings->fov = fov;
	settings->ray_count = ray_count;
	settings->speed = speed;
	settings->mouse_sensibility = mouse_sensibility;
	sprintf(settings->user_name, "%s", user_name);
}

void player(Player *player, Settings *settings)
{
	// DrawCircleV(player->position, 5, GREEN);
	p_controls(player, settings);
}

void p_controls(Player *player, Settings *settings)
{
	// position
	const Vector2 new_speed = {(settings->speed + (settings->speed * 1.5 * IsKeyDown(340))) * d_time * movement_enabled, (settings->speed + 100 * IsKeyDown(340)) * d_time * movement_enabled};
	player->position.x +=
		cos(player->angle - settings->fov * (PI / 180) / 2) * new_speed.x * IsKeyDown(65) +	 // a
		-cos(player->angle - settings->fov * (PI / 180) / 2) * new_speed.x * IsKeyDown(68) + // d
		cos(player->angle + settings->fov * (PI / 180) / 2) * new_speed.x * IsKeyDown(87) +	 // w
		-cos(player->angle + settings->fov * (PI / 180) / 2) * new_speed.x * IsKeyDown(83);	 // s

	player->position.y +=
		sin(player->angle - settings->fov * (PI / 180) / 2) * new_speed.y * IsKeyDown(65) +	 // a
		-sin(player->angle - settings->fov * (PI / 180) / 2) * new_speed.y * IsKeyDown(68) + // d
		sin(player->angle + settings->fov * (PI / 180) / 2) * new_speed.y * IsKeyDown(87) +	 // w
		-sin(player->angle + settings->fov * (PI / 180) / 2) * new_speed.y * IsKeyDown(83);	 // s

	// rotation
	float mouse_diff = (GetMousePosition().x - GetScreenWidth() / 2) * movement_enabled;
	if (mouse_diff > player->ray_length)
		mouse_diff = player->ray_length;
	if (mouse_diff < -player->ray_length)
		mouse_diff = -player->ray_length;
	float angular_distance = (acos((mouse_diff / player->ray_length)) - 90 * (PI / 180));
	if (GetMousePosition().x != WIDTH / 2 && movement_enabled)
		SetMousePosition(WIDTH / 2, 0);
	player->angle += -angular_distance * (settings->mouse_sensibility / 100);
	for (int i = 0; i < settings->ray_count; i++)
	{
		float angle = (settings->fov * PI / 180.0f * i / settings->ray_count) + player->angle;
		player->rays[i].x = player->ray_length * cosf(angle) + player->position.x;
		player->rays[i].y = player->ray_length * sinf(angle) + player->position.y;
	}
	p_collide(player, settings, new_speed);
}

void p_collide(Player *player, Settings *settings, Vector2 speed)
{
	// minimap borders
	player->position.x +=
		2 * speed.x * (player->position.x < 15) - 2 * speed.x * (player->position.x > 230);
	player->position.y +=
		2 * speed.y * (player->position.y < 15) - 2 * speed.y * (player->position.y > 155);
	/* for (int i = 0; i < settings->ray_count; i++)
	{
		if (settings->distance[i] < 4)
		{
			player->position.x += 2 * speed.x;
		}
	} */
}

void p_draw_on_map(Player *player, Settings *settings)
{
	// the triangle's vertices are named clockwise from A to C
	Vector2 t_A = {7 * cosf(player->ray_angle_from_center[settings->ray_count / 2] + player->angle + (settings->fov / 2 * PI / 180)) + player->position.x,
				   7 * sinf(player->ray_angle_from_center[settings->ray_count / 2] + player->angle + (settings->fov / 2 * PI / 180)) + player->position.y};
	Vector2 t_B = {5 * cosf(player->angle + 3.0f) + player->position.x,
				   5 * sinf(player->angle + 3.0f) + player->position.y};
	Vector2 t_C = {5 * cosf(player->angle - 1.1f) + player->position.x,
				   5 * sinf(player->angle - 1.1f) + player->position.y};
	DrawTriangle(t_B, t_A, t_C, BLUE);
}
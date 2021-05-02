#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib/include/raylib.h"
#include "headers/config.h"
#include "headers/player.h"

// player by reference, x and y coordinates, angle
void init_player(Player *player, Settings *settings, float x, float y, float angle)
{
	player->position = (Vector2){x, y};
	player->angle = angle;
	player->ray_length = 200;
	for (int i = 0; i < settings->ray_count; i++)
	{
		float angle = (settings->fov * PI / 180.0f * i / settings->ray_count) + player->angle;
		player->rays[i].x = player->ray_length * cosf(angle) + player->position.x;
		player->rays[i].y = player->ray_length * sinf(angle) + player->position.y;
	}
	for (int i = settings->ray_count / 2; i > 0; i--)
		player->ray_angle_from_center[i] = -i * PI / 180;
	for (int i = settings->ray_count / 2; i < settings->ray_count; i++)
		player->ray_angle_from_center[i] = i * PI / 180;
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
	// DrawTriangle((Vector2){player->position.x + 2, player->position.y + 7}, (Vector2){player->position.x - 2, player->position.y - 7}, (Vector2){player->position.x - 6, player->position.y + 7}, GREEN);
	// DrawTriangle((Vector2){WIDTH / 2 + 15, HEIGHT / 2 + 50}, (Vector2){WIDTH / 2 - 15, HEIGHT / 2 - 50}, (Vector2){WIDTH / 2 - 40, HEIGHT / 2 + 50}, GREEN);
	DrawCircleV(player->position, 5, GREEN);
	p_controls(player, settings);
}

void p_controls(Player *player, Settings *settings)
{
	// position
	const Vector2 new_speed = {(settings->speed + (settings->speed * 1.5 * IsKeyDown(340))) * d_time, (settings->speed + 100 * IsKeyDown(340)) * d_time};
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
	float mouse_diff = GetMousePosition().x - GetScreenWidth() / 2;
	if (mouse_diff > player->ray_length)
		mouse_diff = player->ray_length;
	if (mouse_diff < -player->ray_length)
		mouse_diff = -player->ray_length;
	float angular_distance = (acos((mouse_diff / player->ray_length)) - 90 * (PI / 180));
	if (GetMousePosition().x != WIDTH / 2)
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
	for (int i = 0; i < settings->ray_count; i++)
	{
		if (settings->distance[i] < 4)
		{
			player->position.x += 2 * speed.x;
		}
	}
}
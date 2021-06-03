#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	for (int i = 0; i < settings->ray_count; i++)
	{
		player->ray_angle_from_start[i] = (settings->fov * PI / 180.0f * i / settings->ray_count) + player->angle;
		player->rays[i].x = settings->ray_length * cosf(player->ray_angle_from_start[i]) + player->position.x;
		player->rays[i].y = settings->ray_length * sinf(player->ray_angle_from_start[i]) + player->position.y;
	}
	player->distance_between_rays = settings->fov * PI / 180.0f / settings->ray_count;
}

void load_settings(Settings *settings, const char *user_name, float fov, int ray_count, float speed, float mouse_sensibility, char *settings_file_name)
{
	FILE *settings_file;
	if (settings_file_name[0] == '~')
	{
		char temp[256] = "/home/";
		settings_file_name++;
		strcat(temp, getenv("USER"));
		strcat(temp, settings_file_name);
		settings_file = fopen(temp, "r");
	}
	else
		settings_file = fopen(settings_file_name, "r");

	load_default_settings(settings, "Player", 100, 360, 40, 38);
	if (settings_file != NULL)
	{
		char line_buffer[256];
		while (fgets(line_buffer, sizeof(line_buffer), settings_file))
		{
			char buffer[128] = {0};

			sscanf(line_buffer, "username=%s", settings->user_name);

			// if (sscanf(line_buffer, "vsync=%[truefalse]", buffer))
			// 	settings->vsync = strcmp(buffer, "false");
			// if (sscanf(line_buffer, "directional_rays=%[truefalse]", buffer))
			// 	settings->directional_rays_enabled = strcmp(buffer, "false");
			// if (sscanf(line_buffer, "fisheye=%[truefalse]", buffer))
			// 	settings->fisheye_correction = !strcmp(buffer, "false");
			// if (sscanf(line_buffer, "show_rays=%[truefalse]", buffer))
			// 	settings->show_rays = strcmp(buffer, "false");

			sscanf(line_buffer, "vsync=%i", &settings->vsync);
			sscanf(line_buffer, "directional_rays=%i", &settings->directional_rays_enabled);
			sscanf(line_buffer, "fisheye=%i", &settings->fisheye_correction);
			sscanf(line_buffer, "show_rays=%i", &settings->show_rays);

			sscanf(line_buffer, "fov=%f", &settings->fov);
			sscanf(line_buffer, "ray_count=%i", &settings->ray_count);
			// sscanf(line_buffer, "ray_length=%i", &settings->ray_length);
			settings->ray_length = 200;
			sscanf(line_buffer, "speed=%f", &settings->speed);
			sscanf(line_buffer, "mouse_sensibility=%f", &settings->mouse_sensibility);
		}
		fclose(settings_file);
	}
}

void load_default_settings(Settings *settings, const char *user_name, float fov, int ray_count, float speed, float mouse_sensibility)
{
	settings->fov = fov;
	settings->ray_count = ray_count;
	settings->ray_length = 200;
	settings->speed = speed;
	settings->mouse_sensibility = mouse_sensibility;
	settings->vsync = true;
	settings->directional_rays_enabled = true; // if true the directional_rays will be displayed, else the circle
	settings->fisheye_correction = false;
	settings->show_rays = false;
	sprintf(settings->user_name, "%s", user_name);
}

void player(Player *player, Settings *settings)
{
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
	if (mouse_diff > settings->ray_length)
		mouse_diff = settings->ray_length;
	if (mouse_diff < -settings->ray_length)
		mouse_diff = -settings->ray_length;
	float angular_distance = (acos((mouse_diff / settings->ray_length)) - 90 * (PI / 180));
	if (GetMousePosition().x != WIDTH / 2 && movement_enabled)
		SetMousePosition(WIDTH / 2, 0);
	player->angle += -angular_distance * (settings->mouse_sensibility / 100);

	// updating actual rays
	for (int i = 0; i < settings->ray_count; i++)
	{
		float angle = (settings->fov * PI / 180.0f * i / settings->ray_count) + player->angle;
		player->rays[i].x = settings->ray_length * cosf(angle) + player->position.x;
		player->rays[i].y = settings->ray_length * sinf(angle) + player->position.y;
	}

	// updating directional rays
	for (int i = 0; i < 100; i++)
	{
		float angle = (settings->fov * PI / 180.0f * i / 100) + player->angle;
		player->directional_rays[i].x = settings->ray_length / 10 * cosf(angle) + player->position.x;
		player->directional_rays[i].y = settings->ray_length / 10 * sinf(angle) + player->position.y;
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
	/* Vector2 t_A = {6 * cosf(player->ray_angle_from_center[settings->ray_count / 2] + player->angle + (settings->fov / 2 * PI / 180)) + player->position.x,
				   6 * sinf(player->ray_angle_from_center[settings->ray_count / 2] + player->angle + (settings->fov / 2 * PI / 180)) + player->position.y};
	Vector2 t_B = {3 * cosf(player->angle + 3.0f) + player->position.x,
				   3 * sinf(player->angle + 3.0f) + player->position.y};
	Vector2 t_C = {3 * cosf(player->angle - 1.1f) + player->position.x,
				   3 * sinf(player->angle - 1.1f) + player->position.y};
	DrawTriangle(t_B, t_A, t_C, BLUE); */
	// DrawCircleV(player->position, 3, BLUE);
	DrawCircleGradient(player->position.x, player->position.y, 10, BLUE, ColorAlpha(WHITE, 0));
	for (int i = 0; i < settings->ray_count * settings->show_rays; i++)
		DrawLineEx(player->position, player->colliding_rays[i], 1 * (player->colliding_rays[i].x && player->colliding_rays[i].y), RED);
	for (int i = 0; i < settings->ray_count * settings->directional_rays_enabled; i++)
		DrawLineEx(player->position, player->directional_rays[i], 1 * (player->directional_rays[i].x && player->directional_rays[i].y), ColorAlpha(BLUE, 0.05));
}
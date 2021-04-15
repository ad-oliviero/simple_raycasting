#include <raylib.h>
#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include "headers/config.h"
#include "headers/player.h"

Player local_player;

// player by reference, x and y coordinates, angle, fov, ray_count, speed (150 recommended)
void init_player(Player *player, float x, float y, float angle, float fov, int ray_count, float speed)
{
	player->position = (Vector2){x, y};
	player->angle = angle;
	player->fov = fov;
	player->ray_count = ray_count;
	player->speed = speed;
	for (int i = 0; i < player->ray_count; i++)
	{
		float angle = (player->fov * PI / 180.0f * i / player->ray_count) + player->angle;
		player->rays[i].x = 400 * cosf(angle) + player->position.x;
		player->rays[i].y = 400 * sinf(angle) + player->position.y;
	}
}

void player()
{
	DrawCircleV(local_player.position, 5, GREEN);
	update_rays(&local_player);
	p_controls(&local_player);
}

void p_controls(Player *player)
{
	const Vector2 new_speed = {(player->speed + 100 * IsKeyDown(340)) * d_time, (player->speed + 100 * IsKeyDown(340)) * d_time};
	player->position.x +=
		cos(player->angle - player->fov * 0.017 / 2) * new_speed.x * IsKeyDown(65) +  // a
		-cos(player->angle - player->fov * 0.017 / 2) * new_speed.x * IsKeyDown(68) + // d
		cos(player->angle + player->fov * 0.017 / 2) * new_speed.x * IsKeyDown(87) +  // w
		-cos(player->angle + player->fov * 0.017 / 2) * new_speed.x * IsKeyDown(83);  // s

	player->position.y +=
		sin(player->angle - player->fov * 0.017 / 2) * new_speed.y * IsKeyDown(65) +  // a
		-sin(player->angle - player->fov * 0.017 / 2) * new_speed.y * IsKeyDown(68) + // d
		sin(player->angle + player->fov * 0.017 / 2) * new_speed.y * IsKeyDown(87) +  // w
		-sin(player->angle + player->fov * 0.017 / 2) * new_speed.y * IsKeyDown(83);  // s

	// minimap block
	/*player->position.x +=
		new_speed.x * (player->position.x < 15) +
		-new_speed.x * (player->position.x > 230)// +
		//new_speed.x * (player->position.y < 15) +
		//new_speed.x * (player->position.y > 155);

	player->position.y +=
		//new_speed.y * (player->position.x < 15) +
		//new_speed.y * (player->position.x > 230) +
		new_speed.y * (player->position.y < 15) +
		-new_speed.y * (player->position.y > 155);
*/
	player->angle += (-5 * IsKeyDown(263) + 5 * IsKeyDown(262)) * d_time;
	//local_player->angle += mouse_previous - GetMousePosition().x;
	//local_player->fov += GetMouseWheelMove() * 10 - 10 * (local_player->fov > 180) + 10 * (local_player->fov < 30);// * -500 * d_time;
	//if (local_player->angle > 6.28 || local_player->angle < -6.28) local_player->angle = 0;
}

void cast_rays(Vector2 ray_s, Vector2 ray_e, Vector2 wall_s, Vector2 wall_e, Vector2 *collision_point)
{ // https://web.archive.org/web/20060911055655/http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
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

void update_rays(Player *player)
{
	for (int i = 0; i < player->ray_count; i++)
	{
		float angle = (player->fov * PI / 180.0f * i / player->ray_count) + player->angle;
		player->rays[i].x = 500 * cosf(angle) + player->position.x;
		player->rays[i].y = 500 * sinf(angle) + player->position.y;
		//DrawLineEx(player->position, player->rays[i], 1, GRAY);
	}
}
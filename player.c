#include <raylib.h>
#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include "headers/config.h"
//#include "map.h"

Vector2 rays[RAY_COUNT] = {0}, collision_point = {0}, p_pos = (Vector2) {100, 50};
float p_rotation = 0, p_speed = 150, fov = 90;
//int collision_index[RAY_COUNT + 1] = {0};

void player();
void update_rays();
void cast_rays();
void   p_controls();
void get_closest_ray();
float sort_collisions();

void player() {
	DrawCircleV(p_pos, 5, GREEN);
	update_rays();
	p_controls();
	//p_pos = GetMousePosition();
}

void p_controls() {
	const Vector2 new_speed = {(p_speed + 150 * IsKeyDown(340)) * d_time, (p_speed + 150 * IsKeyDown(340)) * d_time};
	p_rotation += (-5 * IsKeyDown(263) + 5 * IsKeyDown(262)) * d_time;
	p_pos.x += (-new_speed.x * IsKeyDown(65) + new_speed.x * IsKeyDown(68));
	p_pos.y += (-new_speed.y * IsKeyDown(87) + new_speed.y * IsKeyDown(83));
	/*const float new_speed = (p_speed + 150 * IsKeyDown(340)) * d_time;
	p_rotation += (-5 * IsKeyDown(263) + 5 * IsKeyDown(262)) * d_time;
	p_pos.x += (-new_speed * IsKeyDown(65) + new_speed * IsKeyDown(68));
	p_pos.y += (-new_speed * IsKeyDown(87) + new_speed * IsKeyDown(83));*/
	//fov += GetMouseWheelMove() * 10 - 10 * (fov > 180) + 10 * (fov < 30);// * -500 * d_time;
	//if (p_rotation > 6.28 || p_rotation < -6.28) p_rotation = 0;
	// minimap block
	if (p_pos.y < 15) p_pos.y -= -new_speed.y;
	if (p_pos.y > 155) p_pos.y -= new_speed.y;
	if (p_pos.x < 15) p_pos.x -= -new_speed.x;
	if (p_pos.x > 230) p_pos.x -= new_speed.x;
	/*if (p_pos.y < 15) p_pos.y = 155;
	if (p_pos.y > 155) p_pos.y = 15;
	if (p_pos.x < 15) p_pos.x = 230;
	if (p_pos.x > 230) p_pos.x = 15;*/
}

void cast_rays(Vector2 ray_s, Vector2 ray_e, Vector2 wall_s, Vector2 wall_e) { // https://web.archive.org/web/20060911055655/http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
	float	den = (wall_e.y - wall_s.y) * (ray_e.x - ray_s.x) - (wall_e.x - wall_s.x) * (ray_e.y - ray_s.y),
			num_a = (wall_e.x - wall_s.x) * (ray_s.y - wall_s.y) - (wall_e.y - wall_s.y) * (ray_s.x - wall_s.x),
			num_b = (ray_e.x - ray_s.x) * (ray_s.y - wall_s.y) - (ray_e.y - ray_s.y) * (ray_s.x - wall_s.x);
	if (den == 0) return; // parallel lines
	float	u_a = num_a / den,
			u_b = num_b / den;
	if (u_a >= 0 && u_a <= 1 && u_b >= 0 && u_b <= 1) {
		collision_point = (Vector2) {
			ray_s.x + u_a * (ray_e.x - ray_s.x),
			ray_s.y + u_a * (ray_e.y - ray_s.y)
		};
	}
	return;
}

void update_rays() {
	for (int i = 0; i < RAY_COUNT; i++) {
		float angle = (fov * PI / 180.0f * i / RAY_COUNT) + p_rotation;
		rays[i].x = 500 * cosf(angle) + p_pos.x;
		rays[i].y = 500 * sinf(angle) + p_pos.y;
		//DrawLineEx(p_pos, rays[i], 1, GRAY);
	}
}
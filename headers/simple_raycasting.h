#include <raylib.h>
#include <pthread.h>
#include <stdio.h>
typedef struct
{
	Vector2 position;
	float angle;
	float fov;
	float speed;
	int ray_count;
	int ray_length;
	Vector2 rays[360];
} Player;

// player by reference, x and y coordinates, angle, fov, ray_count, speed (150 recommended)
void init_player(Player *player, float x, float y, float angle, float fov, int ray_count, float speed);
void player();
// player by reference
void p_controls(Player *player);
void cast_rays(Vector2 ray_s, Vector2 ray_e, Vector2 wall_s, Vector2 wall_e, Vector2 *collision_point);
void update_rays(Player *player);
int draw_map();
int mini_map();
int draw_map(/*Map *map*/);
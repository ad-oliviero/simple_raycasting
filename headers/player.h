#include "../raylib/include/raylib.h"

typedef struct
{
	Vector2 position;
	float angle;
	float fov;
	float speed;
	int ray_count;
	int ray_length;
	float ray_angle_from_center[720];
	float distance_between_rays;
	Vector2 rays[720]; // CHANGE THIS
} Player;

// player by reference, x and y coordinates, angle, fov, ray_count, speed (150 recommended)
void init_player(Player *player, float x, float y, float angle, float fov, int ray_count, float speed);
void player(Player *player);
void p_controls(Player *player);
void cast_rays(Vector2 ray_s, Vector2 ray_e, Vector2 wall_s, Vector2 wall_e, Vector2 *collision_point);
void update_rays(Player *player);
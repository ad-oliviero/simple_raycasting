#ifndef RENDERING_H
#define RENDERING_H
void view(Player *player, Settings *settings);
float map_value(float value, float from1, float to1, float from2, float to2);
void view_3d(Player *player, Settings *settings);
void cast_rays(Vector2 ray_s, Vector2 ray_e, Vector2 wall_s, Vector2 wall_e, Vector2 *collision_point);
#endif
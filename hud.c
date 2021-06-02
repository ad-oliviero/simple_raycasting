#include "lib/raylib/include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#define RAYGUI_STATIC
#include "lib/raygui/src/raygui.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "headers/config.h"
#include "headers/player.h"
#include "headers/hud.h"
#include "headers/map.h"
#include "headers/rendering.h"

extern Vector2 linestart[128], linend[128],
	border_s[4], border_e[4],
	map1_s[4], map1_e[4];

Vector2 linestart_s[MAP_SIDES] = {0}, linend_s[MAP_SIDES] = {0};
extern bool display_settings, movement_enabled;

void draw_hud(Player *player, Settings *settings)
{
	DrawLineEx((Vector2){WIDTH / 2, HEIGHT / 2 - 10}, (Vector2){WIDTH / 2, HEIGHT / 2 + 10}, 2, WHITE);
	DrawLineEx((Vector2){WIDTH / 2 - 10, HEIGHT / 2}, (Vector2){WIDTH / 2 + 10, HEIGHT / 2}, 2, WHITE);
	draw_map(player, settings);
	if (IsKeyPressed(256))
	{
		display_settings = !display_settings;
		movement_enabled = !movement_enabled;
		SetMousePosition(WIDTH / 2, HEIGHT / 2);
		if (display_settings)
			EnableCursor();
		else
			DisableCursor();
	}
	if (display_settings)
		draw_settings(settings, SETTINGS_FILE_LOCATION);
	// DrawCircle(WIDTH / 2, HEIGHT / 2, 4, BLACK);
}

void draw_map(/*Map *map,*/ Player *player, Settings *settings)
{
	DrawRectangleRec((Rectangle){10, 10, 225, 150}, WHITE); // map background
	// for (int i = 0; i < 4; i++)DrawLineEx(border_s[i], border_e[i], 3, RED); // map background
	memcpy(linestart_s, linestart, MAP_SIDES);
	memcpy(linend_s, linend, MAP_SIDES);
	for (int i = 0; i < MAP_SIDES; i++)
	{
		linestart_s[i].x /= 2;
		linestart_s[i].y /= 2;
		linend_s[i].x /= 2;
		linend_s[i].y /= 2;
		linestart_s[i].x += 35;
		linestart_s[i].y += 45;
		linend_s[i].x += 35;
		linend_s[i].y += 45;
	}

	for (int i = 0; i < MAP_SIDES; i++)
		DrawLineEx(linestart_s[i], linend_s[i], 1, BLACK);
	// DrawLineEx(map1_s[0], map1_e[0], 1, WHITE);
	// DrawLineEx(map1_s[1], map1_e[1], 1, WHITE);
	// DrawLineEx(map1_s[2], map1_e[2], 1, WHITE);
	// DrawLineEx(map1_s[3], map1_e[3], 1, WHITE);
	p_draw_on_map(player, settings);
}

void draw_settings(Settings *settings, char *settings_file_name)
{
	Rectangle main_settings_rec = {WIDTH / 2 - (WIDTH / 4), HEIGHT / 2 - (HEIGHT / 4), WIDTH / 2, HEIGHT / 2},
			  ray_count_slider = {main_settings_rec.x + 10, main_settings_rec.y + 35, 200, 20},
			  vsync_toggle = {main_settings_rec.x + 10, ray_count_slider.y + 30, 200, 20},
			  directional_rays_button = {main_settings_rec.x + 10, vsync_toggle.y + 30, 200, 20},
			  fov_slider = {main_settings_rec.x + 10, directional_rays_button.y + 30, 200, 20},
			  mouse_sensibility_slider = {main_settings_rec.x + 10, fov_slider.y + 30, 200, 20},
			  fisheye_toggle = {main_settings_rec.x + 10, mouse_sensibility_slider.y + 30, 200, 20},
			  show_rays_toggle = {main_settings_rec.x + 10, fisheye_toggle.y + 30, 200, 20},
			  quit_button = {main_settings_rec.x + main_settings_rec.width / 2 - 35, main_settings_rec.y + main_settings_rec.height - 30, 70, 20},
			  save_settings_button = {main_settings_rec.x + 10, main_settings_rec.y + main_settings_rec.height - 90, 150, 20},
			  load_settings_file_button = {main_settings_rec.x + 10, save_settings_button.y + 30, 150, 20},
			  load_default_settings_button = {main_settings_rec.x + 10, load_settings_file_button.y + 30, 150, 20};

	DrawRectangle(0, 0, WIDTH, HEIGHT, ColorAlpha(BLACK, 0.7)); // obfuscate background
	// GuiGroupBox(main_settings_rec, "Settings");
	if (!GuiWindowBox((Rectangle){WIDTH / 2 - (WIDTH / 4), HEIGHT / 2 - (HEIGHT / 4), WIDTH / 2, HEIGHT / 2}, "Settings"))
	{
		settings->ray_count = GuiSlider(ray_count_slider, NULL, "Ray count", settings->ray_count, 10, RAY_MAX_COUNT);

		settings->vsync = GuiToggle(vsync_toggle, "Vsync", settings->vsync);
		SetTargetFPS((GetMonitorRefreshRate(GetCurrentMonitor()) + 1) * settings->vsync);

		settings->directional_rays_enabled = GuiToggle(directional_rays_button, "Directional Rays", settings->directional_rays_enabled);

		settings->fov = GuiSlider(fov_slider, NULL, "FOV", settings->fov, 0, 160);

		settings->mouse_sensibility = GuiSlider(mouse_sensibility_slider, NULL, "Mouse sensibility", settings->mouse_sensibility, 0, 200);

		settings->fisheye_correction = GuiToggle(fisheye_toggle, "Toggle Fisheye - NOT WORKING", settings->fisheye_correction);

		settings->show_rays = GuiToggle(show_rays_toggle, "Show full colliding rays", settings->show_rays);

		if (GuiButton(quit_button, "Quit game"))
		{
			CloseWindow();
			exit(0);
		}
		if (GuiButton(save_settings_button, "Save settings"))
		{
			FILE *settings_file = fopen(settings_file_name, "w");
			fprintf(settings_file,
					"fov=%i\n"
					"ray_count=%i\n"
					"ray_length=%i\n"
					"speed=%f\n"
					"mouse_sensibility=%f\n"
					"vsync=%i\n"
					"directional_rays=%i\n"
					"fisheye=%i\n"
					"show_rays=%i\n"
					"username=%s",
					(int)settings->fov, settings->ray_count, settings->ray_length, settings->speed, settings->mouse_sensibility, settings->vsync, settings->directional_rays_enabled, settings->fisheye_correction, settings->show_rays, settings->user_name);
			fclose(settings_file);
		}
		if (GuiButton(load_settings_file_button, "Load settings from file"))
			init_settings(settings, "Name", 100, 360, 40, 38, SETTINGS_FILE_LOCATION);
		if (GuiButton(load_default_settings_button, "Load default settings"))
			load_default_settings(settings, "Name", 100, 360, 40, 38);
	}
	else
	{
		display_settings = !display_settings;
		movement_enabled = !movement_enabled;
		SetMousePosition(WIDTH / 2, HEIGHT / 2);
		if (display_settings)
			EnableCursor();
		else
			DisableCursor();
	}
}
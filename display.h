// ---------------------------- display.h -------------------------------------

// James Truong

// Created: 8/29/2021

// Last Modified: 9/19/2021

// ----------------------------------------------------------------------------

// 

// ----------------------------------------------------------------------------

#ifndef DISPLAY_H
#define DISPLAY_H
#include <allegro5/allegro5.h>
#include "helpers.h"
#include "sprites.h"

extern long frames;
extern long score;


// DISPLAY
const int BUFFER_W = 320;
const int BUFFER_H = 240;

#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;

void disp_init()
{
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

	disp = al_create_display(DISP_W, DISP_H);
	must_init(disp, "display");

	buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
	must_init(buffer, "bitmap buffer");
}

void disp_deinit()
{
	al_destroy_bitmap(buffer);
	al_destroy_display(disp);
}

void disp_pre_draw()
{
	al_set_target_bitmap(buffer);
}

void disp_post_draw()
{
	al_set_target_backbuffer(disp);
	al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);

	al_flip_display();
}


// KEYBOARD
#define KEY_SEEN 1
#define KEY_RELEASED 2
unsigned char key[ALLEGRO_KEY_MAX];

void keyboard_init()
{
	memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT* event)
{
	switch (event->type)
	{
	case ALLEGRO_EVENT_TIMER:
		for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
			key[i] &= KEY_SEEN;
		break;
	case ALLEGRO_EVENT_KEY_DOWN:
		key[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
		break;
	case ALLEGRO_EVENT_KEY_UP:
		key[event->keyboard.keycode] &= KEY_RELEASED;
		break;
	}
}




// BACKGROUND
typedef struct STAR
{
	float y;
	float speed;
} STAR;

#define STARS_N ((BUFFER_W /2) - 1)
STAR stars[STARS_N];

void stars_init()
{
	for (int i = 0; i < STARS_N; i++)
	{
		stars[i].y = between_f(0, BUFFER_H);
		stars[i].speed = between_f(0.1, 1);
	}
}

void stars_update()
{
	for (int i = 0; i < STARS_N; i++)
	{
		stars[i].y += stars[i].speed;
		if (stars[i].y >= BUFFER_H)
		{
			stars[i].y = 0;
			stars[i].speed = between_f(0.1, 1);
		}
	}
}

void stars_draw()
{
	float star_x = 1.5;
	for (int i = 0; i < STARS_N; i++)
	{
		float l = stars[i].speed * 0.8;
		al_draw_pixel(star_x, stars[i].y, al_map_rgb_f(rand() % 2 - 1, rand() % 2 - 1, rand() % 2 - 1));
		star_x += 2;
	}
}


// HUD
ALLEGRO_FONT* font;
long score_display;

void hud_init()
{
	font = al_create_builtin_font();
	must_init(font, "font");

	score_display = 0;
}

void hud_deinit()
{
	al_destroy_font(font);
}

void hud_update()
{
	if (frames % 2)
		return;
	for (long i = 5; i > 0; i--)
	{
		long diff = 1 << i;
		if (score_display <= (score - diff))
			score_display += diff;
	}
}

void hud_draw()
{
	al_draw_textf(font, al_map_rgb_f(1, 1, 1), 1, 1, 0, "%06ld", score_display);

	int spacing = LIFE_W + 1;
	for (int i = 0; i < ship.lives; i++)
		al_draw_bitmap(sprites.life, 1 + (i * spacing), 10, 0);

	if (ship.lives < 0)
		al_draw_textf(font, al_map_rgb_f(1, 1, 1), BUFFER_W / 2, BUFFER_H / 2, ALLEGRO_ALIGN_CENTER, "G A M E  O V E R");
}


// TITLE SCREEN
ALLEGRO_FONT* title;
bool playing = false;

void title_init()
{
	title = al_create_builtin_font();
	must_init(font, "title");
}

void title_deinit()
{
	al_destroy_font(title);
}

void title_update()
{
	if (key[ALLEGRO_KEY_SPACE])
		playing = true;
}

void title_draw()
{
	if (!playing)
	{
		al_draw_textf(font, al_map_rgb_f(1, 1, 1), BUFFER_W / 2, BUFFER_H / 2, ALLEGRO_ALIGN_CENTER, "SPACE SHOOTER");
		al_draw_textf(font, al_map_rgb_f(1, 1, 1), BUFFER_W / 2, BUFFER_H - 100, ALLEGRO_ALIGN_CENTER, "PRESS SPACE");
	}
}

#endif
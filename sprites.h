// ---------------------------- sprites.h -------------------------------------

// James Truong

// Created: 8/29/2021

// Last Modified: 9/19/2021

// ----------------------------------------------------------------------------

// 

// ----------------------------------------------------------------------------

#ifndef SPRITES_H
#define SPRITES_H
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "helpers.h"

extern long frames;
extern long score;
extern const int BUFFER_H;
extern const int BUFFER_W;
extern unsigned char key[];

// SPRITES
#define SHIP_W 12
#define SHIP_H 13

#define SHIP_SHOT_W 2
#define SHIP_SHOT_H 9

#define LIFE_W 6
#define LIFE_H 6

const int ALIEN_W[] = { 14, 13, 45 };
const int ALIEN_H[] = { 9, 10, 27 };

#define ALIEN_BUG_W			ALIEN_W[0]
#define ALIEN_BUG_H			ALIEN_H[0]
#define ALIEN_ARROW_W		ALIEN_W[1]
#define ALIEN_ARROW_H		ALIEN_H[1]
#define ALIEN_THICCBOI_W	ALIEN_W[2]
#define ALIEN_THICCBOI_H	ALIEN_H[2]

#define ALIEN_SHOT_W 4
#define ALIEN_SHOT_H 4

#define EXPLOSION_FRAMES	4
#define SPARKS_FRAMES		3

typedef struct SPRITES
{
	ALLEGRO_BITMAP* _sheet;

	ALLEGRO_BITMAP* ship;
	ALLEGRO_BITMAP* ship_shot[2];
	ALLEGRO_BITMAP* life;

	ALLEGRO_BITMAP* alien[3];
	ALLEGRO_BITMAP* alien_shot;

	ALLEGRO_BITMAP* explosion[EXPLOSION_FRAMES];
	ALLEGRO_BITMAP* sparks[SPARKS_FRAMES];

	ALLEGRO_BITMAP* powerup[4];
} SPRITES;
SPRITES sprites;

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h)
{
	ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
	must_init(sprite, "sprite grab");
	return sprite;
}

void sprite_init()
{
	sprites._sheet = al_load_bitmap("assets/spritesheet.png");
	must_init(sprites._sheet, "spritesheet");

	sprites.ship = sprite_grab(0, 0, SHIP_W, SHIP_H);

	sprites.life = sprite_grab(0, 14, LIFE_W, LIFE_H);

	sprites.ship_shot[0] = sprite_grab(13, 0, SHIP_SHOT_W, SHIP_SHOT_H);
	sprites.ship_shot[1] = sprite_grab(16, 0, SHIP_SHOT_W, SHIP_SHOT_H);

	sprites.alien[0] = sprite_grab(19, 0, ALIEN_BUG_W, ALIEN_BUG_H);
	sprites.alien[1] = sprite_grab(19, 10, ALIEN_ARROW_W, ALIEN_ARROW_H);
	sprites.alien[2] = sprite_grab(0, 21, ALIEN_THICCBOI_W, ALIEN_THICCBOI_H);

	sprites.alien_shot = sprite_grab(13, 10, ALIEN_SHOT_W, ALIEN_SHOT_H);

	sprites.explosion[0] = sprite_grab(33, 10, 9, 9);
	sprites.explosion[1] = sprite_grab(43, 9, 11, 11);
	sprites.explosion[2] = sprite_grab(46, 21, 17, 18);
	sprites.explosion[3] = sprite_grab(46, 40, 17, 17);

	sprites.sparks[0] = sprite_grab(34, 0, 10, 8);
	sprites.sparks[1] = sprite_grab(45, 0, 7, 8);
	sprites.sparks[2] = sprite_grab(54, 0, 9, 8);

	sprites.powerup[0] = sprite_grab(0, 49, 9, 12);
	sprites.powerup[1] = sprite_grab(10, 49, 9, 12);
	sprites.powerup[2] = sprite_grab(20, 49, 9, 12);
	sprites.powerup[3] = sprite_grab(30, 49, 9, 12);
}

void sprites_deinit()
{
	al_destroy_bitmap(sprites.ship);

	al_destroy_bitmap(sprites.ship_shot[0]);
	al_destroy_bitmap(sprites.ship_shot[1]);

	al_destroy_bitmap(sprites.alien[0]);
	al_destroy_bitmap(sprites.alien[1]);
	al_destroy_bitmap(sprites.alien[2]);

	al_destroy_bitmap(sprites.sparks[0]);
	al_destroy_bitmap(sprites.sparks[1]);
	al_destroy_bitmap(sprites.sparks[2]);

	al_destroy_bitmap(sprites.explosion[0]);
	al_destroy_bitmap(sprites.explosion[1]);
	al_destroy_bitmap(sprites.explosion[2]);
	al_destroy_bitmap(sprites.explosion[3]);

	al_destroy_bitmap(sprites.powerup[0]);
	al_destroy_bitmap(sprites.powerup[1]);
	al_destroy_bitmap(sprites.powerup[2]);
	al_destroy_bitmap(sprites.powerup[3]);

	al_destroy_bitmap(sprites._sheet);
}


// AUDIO
ALLEGRO_SAMPLE* sample_shot;
ALLEGRO_SAMPLE* sample_explode[2];

void audio_init()
{
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(128);

	sample_shot = al_load_sample("assets/shot.flac");
	must_init(sample_shot, "shot sample");

	sample_explode[0] = al_load_sample("assets/explode1.flac");
	must_init(sample_explode[0], "explode[0] sample");
	sample_explode[1] = al_load_sample("assets/explode2.flac");
	must_init(sample_explode[1], "explode[1] sample");
}

void audio_deinit()
{
	al_destroy_sample(sample_shot);
	al_destroy_sample(sample_explode[0]);
	al_destroy_sample(sample_explode[1]);
}

// FX

typedef struct FX
{
	int x, y;
	int frame;
	bool spark;
	bool used;
} FX;

#define FX_N 128
FX fx[FX_N];

void fx_init()
{
	for (int i = 0; i < FX_N; i++)
	{
		fx[i].used = false;
	}
}

void fx_add(bool spark, int x, int y)
{
	if (!spark)
		al_play_sample(sample_explode[between(0, 2)], 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
	for (int i = 0; i < FX_N; i++)
	{
		if (fx[i].used)
			continue;
		fx[i].x = x;
		fx[i].y = y;
		fx[i].frame = 0;
		fx[i].spark = spark;
		fx[i].used = true;
		return;
	}
}

void fx_update()
{
	for (int i = 0; i < FX_N; i++)
	{
		for (int i = 0; i < FX_N; i++)
		{
			if (!fx[i].used)
				continue;
			fx[i].frame++;

			if ((!fx[i].spark && (fx[i].frame == (EXPLOSION_FRAMES * 2))) ||
				(fx[i].spark && (fx[i].frame == (SPARKS_FRAMES * 2))))
				fx[i].used = false;
		}
	}
}

void fx_draw()
{
	for (int i = 0; i < FX_N; i++)
	{
		if (!fx[i].used)
			continue;
		int frame_display = fx[i].frame / 2;
		ALLEGRO_BITMAP* bmp = fx[i].spark ? sprites.sparks[frame_display] : sprites.explosion[frame_display];

		int x = fx[i].x - (al_get_bitmap_width(bmp) / 2);
		int y = fx[i].y - (al_get_bitmap_height(bmp) / 2);
		al_draw_bitmap(bmp, x, y, 0);
	}
}

// SHOT
typedef struct SHOT
{
	int x, y, dx, dy;
	int frame;
	bool ship;
	bool used;
} SHOT;

#define SHOTS_N 128
SHOT shots[SHOTS_N];

void shots_init()
{
	for (int i = 0; i < SHOTS_N; i++)
		shots[i].used = false;
}

bool shots_add(bool ship, bool straight, int x, int y)
{
	al_play_sample(sample_shot, 0.3, 0, ship ? 1.0 : between_f(1.5, 1.6), ALLEGRO_PLAYMODE_ONCE, NULL);

	for (int i = 0; i < SHOTS_N; i++)
	{
		if (shots[i].used)
			continue;
		shots[i].ship = ship;

		if (ship)
		{
			shots[i].x = x - (SHIP_SHOT_W / 2);
			shots[i].y = y - (SHIP_SHOT_H / 2);
		}
		else // alien
		{
			shots[i].x = x - (ALIEN_SHOT_W / 2);
			shots[i].y = y - (ALIEN_SHOT_H / 2);

			if (straight)
			{
				shots[i].dx = 0;
				shots[i].dy = 2;
			}
			else
			{
				shots[i].dx = between(-2, 2);
				shots[i].dy = between(-2, 2);
			}

			// if the shot has no speed, don't bother
			if (!shots[i].dx && !shots[i].dy)
				return true;

			shots[i].frame = 0;
		}

		shots[i].frame = 0;
		shots[i].used = true;

		return true;
	}
	return false;
}

void shots_update()
{
	for (int i = 0; i < SHOTS_N; i++)
	{
		if (!shots[i].used)
			continue;

		if (shots[i].ship)
		{
			shots[i].y -= 5;
			if (shots[i].y < -SHIP_SHOT_H)
			{
				shots[i].used = false;
				continue;
			}
		}
		else // alien
		{
			shots[i].x += shots[i].dx;
			shots[i].y += shots[i].dy;

			if ((shots[i].x < -ALIEN_SHOT_W)
				|| (shots[i].x > BUFFER_W)
				||(shots[i].y < -ALIEN_SHOT_H)
				|| (shots[i].y > BUFFER_H)
				)
			{
				shots[i].used = false;
				continue;
			}
		}

		shots[i].frame++;
	}
}

bool shots_collide(bool ship, int x, int y, int w, int h)
{
	for (int i = 0; i < SHOTS_N; i++)
	{
		if (!shots[i].used)
			continue;

		// don't collide with one's own shots
		if (shots[i].ship == ship)
			continue;

		int sw, sh;
		if (ship)
		{
			sw = ALIEN_SHOT_W;
			sh = ALIEN_SHOT_H;
		}
		else
		{
			sw = SHIP_SHOT_W;
			sh = SHIP_SHOT_H;
		}

		if (collide(x, y, x+w, y+h, shots[i].x, shots[i].y, shots[i].x+sw, shots[i].y+sh))
		{
			fx_add(true, shots[i].x + (sw / 2), shots[i].y + (sh / 2));
			shots[i].used = false;
			return true;
		}
	}

	return false;
}

void shots_draw()
{
	for (int i = 0; i < SHOTS_N; i++)
	{
		if (!shots[i].used)
			continue;
		int frame_display = (shots[i].frame / 2) % 2;

		if (shots[i].ship)
			al_draw_bitmap(sprites.ship_shot[frame_display], shots[i].x, shots[i].y, 0);
		else // alien
		{
			ALLEGRO_COLOR tint = frame_display ? al_map_rgb_f(1, 1, 1) : al_map_rgb_f(0.5, 0.5, 0.5);
			al_draw_tinted_bitmap(sprites.alien_shot, tint, shots[i].x, shots[i].y, 0);
		}
	}
}


// READY PLAYER 1
#define SHIP_SPEED 3
#define SHIP_MAX_X (BUFFER_W - SHIP_W)
#define SHIP_MAX_Y (BUFFER_H - SHIP_H)

typedef struct ship
{
	int x, y;
	int shot_timer;
	int lives;
	int respawn_timer;
	int invincible_timer;
} SHIP;
SHIP ship;

void ship_init()
{
	ship.x = (BUFFER_W / 2) - (SHIP_W / 2);
	ship.y = (BUFFER_H / 2) - (SHIP_H / 2);
	ship.shot_timer = 0;
	ship.lives = 3;
	ship.respawn_timer = 0;
	ship.invincible_timer = 120;
}

void ship_update()
{
	if (ship.lives < 0)
		return;

	if (ship.respawn_timer)
	{
		ship.respawn_timer--;
		return;
	}

	if (key[ALLEGRO_KEY_A])
		ship.x -= SHIP_SPEED;
	if (key[ALLEGRO_KEY_D])
		ship.x += SHIP_SPEED;
	if (key[ALLEGRO_KEY_W])
		ship.y -= SHIP_SPEED;
	if (key[ALLEGRO_KEY_S])
		ship.y += SHIP_SPEED;

	if (ship.x < 0)
		ship.x = 0;
	if (ship.y < 0)
		ship.y = 0;

	if (ship.x > SHIP_MAX_X)
		ship.x = SHIP_MAX_X;
	if (ship.y > SHIP_MAX_Y)
		ship.y = SHIP_MAX_Y;

	if (ship.invincible_timer)
		ship.invincible_timer--;
	else
	{
		if (shots_collide(true, ship.x, ship.y, SHIP_W, SHIP_H))
		{
			int x = ship.x + (SHIP_W / 2);
			int y = ship.y + (SHIP_H / 2);
			fx_add(false, x, y);
			fx_add(false, x+4, y+2);
			fx_add(false, x-2, y-4);
			fx_add(false, x+1, y-5);

			ship.lives--;
			ship.respawn_timer = 90;
			ship.invincible_timer = 180;
		}
	}

	if (ship.shot_timer)
		ship.shot_timer--;
	else if (key[ALLEGRO_KEY_F])
	{
		int x = ship.x + (SHIP_W / 2);
		if (shots_add(true, false, x, ship.y))
			ship.shot_timer = 5;
	}
}

void ship_draw()
{
	if (ship.lives < 0)
		return;
	if (ship.respawn_timer)
		return;
	if (((ship.invincible_timer / 2) % 3) == 1)
		return;

	al_draw_bitmap(sprites.ship, ship.x, ship.y, 0);
}


// ENEMIES
typedef enum ALIEN_TYPE
{
	ALIEN_TYPE_BUG = 0,
	ALIEN_TYPE_ARROW,
	ALIEN_TYPE_THICCBOI,
	ALIEN_TYPE_N
} ALIEN_TYPE;

typedef struct ALIEN
{
	int x, y;
	ALIEN_TYPE type;
	int shot_timer;
	int blink;
	int life;
	bool used;
} ALIEN;

#define ALIENS_N 16
ALIEN aliens[ALIENS_N];

void aliens_init()
{
	for (int i = 0; i < ALIENS_N; i++)
		aliens[i].used = false;
}

void aliens_update()
{
	int new_quota = (frames % 120) ? 0 : between(2, 4);
	int new_x = between(10, BUFFER_W-50);

	for (int i = 0; i < ALIENS_N; i++)
	{
		if (!aliens[i].used)
		{
			// if this alien is unused, should it spawn?
			if (new_quota > 0)
			{
				new_x += between(40, 80);
				if (new_x > (BUFFER_W - 60))
					new_x -= (BUFFER_W - 60);

				aliens[i].x = new_x;

				aliens[i].y = between(-40, -30);
				aliens[i].type = (ALIEN_TYPE)between(0, (int)ALIEN_TYPE_N);
				aliens[i].shot_timer = between(1, 99);
				aliens[i].blink = 0;
				aliens[i].used = true;

				switch (aliens[i].type)
				{
				case ALIEN_TYPE_BUG:
					aliens[i].life = 4;
					break;
				case ALIEN_TYPE_ARROW:
					aliens[i].life = 2;
					break;
				case ALIEN_TYPE_THICCBOI:
					aliens[i].life = 12;
					break;
				}

				new_quota--;
			}
			continue;
		}

		switch (aliens[i].type)
		{
		case ALIEN_TYPE_BUG:
			if (frames % 2)
				aliens[i].y++;
			break;
		case ALIEN_TYPE_ARROW:
			aliens[i].y++;
			break;
		case ALIEN_TYPE_THICCBOI:
			if (!(frames % 4))
				aliens[i].y++;
			break;
		}

		if (aliens[i].y >= BUFFER_H)
		{
			aliens[i].used = false;
			continue;
		}

		if (aliens[i].blink)
			aliens[i].blink--;

		if (shots_collide(false, aliens[i].x, aliens[i].y, ALIEN_W[aliens[i].type], ALIEN_H[aliens[i].type]))
		{
			aliens[i].life--;
			aliens[i].blink = 4;
		}

		int cx = aliens[i].x + (ALIEN_W[aliens[i].type] / 2);
		int cy = aliens[i].y + (ALIEN_H[aliens[i].type] / 2);

		if (aliens[i].life <= 0)
		{
			fx_add(false, cx, cy);

			switch (aliens[i].type)
			{
			case ALIEN_TYPE_BUG:
				score += 200;
				break;
			case ALIEN_TYPE_ARROW:
				score += 150;
				break;
			case ALIEN_TYPE_THICCBOI:
				score += 800;
				fx_add(false, cx-10, cy-4);
				fx_add(false, cx+4, cy+10);
				fx_add(false, cx+8, cy+8);
			}

			aliens[i].used = false;
			continue;
		}

		aliens[i].shot_timer--;
		if (aliens[i].shot_timer == 0)
		{
			switch (aliens[i].type)
			{
			case ALIEN_TYPE_BUG:
				shots_add(false, false, cx, cy);
				aliens[i].shot_timer = 150;
				break;
			case ALIEN_TYPE_ARROW:
				shots_add(false, true, cx, aliens[i].y);
				aliens[i].shot_timer = 80;
				break;
			case ALIEN_TYPE_THICCBOI:
				shots_add(false, true, cx-5, cy);
				shots_add(false, true, cx+5, cy);
				shots_add(false, true, cx-5, cy + 8);
				shots_add(false, true, cx+5, cy + 8);
				aliens[i].shot_timer = 200;
				break;
			}
		}
	}
}

void aliens_draw()
{
	for (int i = 0; i < ALIENS_N; i++)
	{
		if (!aliens[i].used)
			continue;
		if (aliens[i].blink > 2)
			continue;
		al_draw_bitmap(sprites.alien[aliens[i].type], aliens[i].x, aliens[i].y, 0);
	}
}



#endif
// ----------------------------- main.cpp -------------------------------------

// James Truong

// Created: 8/29/2021

// Last Modified: 9/19/2021

// ----------------------------------------------------------------------------

// Learning to use Allegro Library for the first time following tutorial and
// online to reach the endgoal of making a small .exe file that is downloadable
// for both friends and recruiters to view and play.
// ----------------------------------------------------------------------------

// Notes on specifications, special algorithms, and assumptions.

// https://github.com/liballeg/allegro_wiki/wiki/Allegro-Vivace%3A-Input

/*
Seriously!? Okay, well while we imagine you'll have already staggered off to the bar by this point, there's a ton of stuff that can be improved with what we've already done. To name a few:

First off, everything's black & white! Add some color; edit the spritesheet, tint what's already there, or make the stars random colors.
Add some music. As we mentioned back in the sound section, The Mod Archive has tons of ol'skool music that Allegro will happily play.
Add high score functionality; find a way for the program to remember the best score it's seen after it restarts.
At the moment, the player can fly into aliens and nothing happens. Use collide() to check for this, and blow up both the player and the alien in question.
Make more aliens (and perhaps stronger aliens) appear as time goes on, so the game gets progressively harder.
When the player's ship (and THICCBOI aliens) explode, fx_add() plays the same sounds multiple times. This is inefficient; figure out how we can avoid this. There are a few ways to do it.
The same problem occurs when THICCBOI aliens shoot, as they spawn 4 shots at a time. Try applying a similar fix there.
Once you've done that, our call to al_reserve_samples() can be given a significantly smaller number.
By following the instructions on Resolution Independence article, make the game fullscreen, and adaptively scale-up the graphics to fill the entire display (rather than having a window with DISP_SCALE locked at 3).
Split up that absolutely huge game.c file. If you've not dabbled in the art of using multiple .c files, this Stack Overflow question is a good place to start.
Add an optional second player with separate controls and score.
*/

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#include "helpers.h"
#include "display.h"
#include "sprites.h"

long frames;
long score;


// MAIN
int main()
{
	must_init(al_init(), "allegro");
	must_init(al_install_keyboard(), "keyboard");

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
	must_init(timer, "timer");

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	must_init(queue, "queue");

	disp_init();

	audio_init();

	must_init(al_init_image_addon(), "image");
	sprite_init();

	hud_init();

	must_init(al_init_primitives_addon(), "primitives");

	must_init(al_install_audio(), "audio");
	must_init(al_init_acodec_addon(), "audio codec");
	must_init(al_reserve_samples(16), "reserver samples");

	ALLEGRO_AUDIO_STREAM* music = al_load_audio_stream("assets/0-space.xm", 2, 2048);
	must_init(music, "music");
	al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);
	al_attach_audio_stream_to_mixer(music, al_get_default_mixer());

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	keyboard_init();
	fx_init();
	shots_init();
	ship_init();
	aliens_init();
	stars_init();
	title_init();

	frames = 0;
	score = 0;

	bool done = false;
	bool redraw = true;
	ALLEGRO_EVENT event;

	al_start_timer(timer);

	while (1)
	{
		al_wait_for_event(queue, &event);

		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:
			stars_update();
			if (playing)
			{
				fx_update();
				shots_update();
				ship_update();
				aliens_update();
				hud_update();
			}
			else
			{
				title_update();
			}

			if (key[ALLEGRO_KEY_ESCAPE])
				done = true;

			redraw = true;
			frames++;
			break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			done = true;
			break;
		}

		if (done)
			break;

		keyboard_update(&event);
		
		if (redraw && al_is_event_queue_empty(queue))
		{
			disp_pre_draw();
			al_clear_to_color(al_map_rgb(0, 0, 0));

			if (playing)
				hud_draw();
			stars_draw();
			aliens_draw();
			shots_draw();
			fx_draw();
			ship_draw();

			title_draw();

			disp_post_draw();
			redraw = false;
		}
	}

	sprites_deinit();
	hud_deinit();
	audio_deinit();
	disp_deinit();
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	return 0;
}
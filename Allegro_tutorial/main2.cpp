// ----------------------------- main.cpp -------------------------------------

// James Truong

// Created: 8/22/2022

// Last Modified: 8/22/2022

// ----------------------------------------------------------------------------

// Learning to use Allegro Library for the first time following tutorial and
// online to reach the endgoal of making a small .exe file that is downloadable
// for both friends and recruiters to view and play.
// ----------------------------------------------------------------------------

// Notes on specifications, special algorithms, and assumptions.

// https://github.com/liballeg/allegro_wiki/wiki/Allegro-Vivace%3A-Input

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>


static int DISPLAY_SIZE_WIDTH = 640;
static int DISPLAY_SIZE_HEIGHT = 480;


void must_init(bool test, const char* description)
{
	if (test) return;

	printf("Failed: initializing %s\n", description);
	exit(1);
}


enum BOUNCER_TYPE {
	BT_MYSHA = 0,
	BT_HELLO,
	BT_TRIANGLE,
	BT_RECTANGLE_1,
	BT_RECTANGLE_2,
	BT_CIRCLE,
	BT_LINE1,
	BT_LINE2,
	BT_N
};


typedef struct BOUNCER
{
	float x, y;
	float dx, dy;
	int type;
} BOUNCER;


int main()
{

	must_init(al_init(), "allegro");
	must_init(al_install_keyboard(), "keyboard");

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
	must_init(timer, "timer");

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	must_init(queue, "event_queue");

	// antialiasing
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	ALLEGRO_DISPLAY* disp = al_create_display(DISPLAY_SIZE_WIDTH,
												DISPLAY_SIZE_HEIGHT);
	must_init(disp, "display");

	ALLEGRO_FONT* font = al_create_builtin_font();
	must_init(font, "font");

	must_init(al_init_image_addon(), "image addon");
	ALLEGRO_BITMAP* mysha = al_load_bitmap("mysha.png");
	must_init(mysha, "mysha");

	must_init(al_init_primitives_addon(), "primitives");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	bool done = false;
	bool redraw = true;
	ALLEGRO_EVENT event;

	BOUNCER obj[BT_N * 2];
	for (int i = 0; i < BT_N * 2; i++)
	{
		BOUNCER* b = &obj[i];
		b->x = rand() % DISPLAY_SIZE_WIDTH;
		b->y = rand() % DISPLAY_SIZE_HEIGHT;
		b->dx = (((float)rand()) / RAND_MAX - 0.5) * 2 * 4;
		b->dy = (((float)rand()) / RAND_MAX - 0.5) * 2 * 4;
		b->type = i % BT_N;
	}

	al_start_timer(timer);
	while (1)
	{
		al_wait_for_event(queue, &event);

		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:
			// game logic
			for (int i = 0; i < BT_N * 2; i++)
			{
				BOUNCER* b = &obj[i];
				b->x += b->dx;
				b->y += b->dy;

				if (b->x < 0)
				{
					b->x  *= -1;
					b->dx *= -1;
				}

				if (b->y < 0)
				{
					b->y  *= -1;
					b->dy *= -1;
				}

				switch (b->type)
				{
				case BT_MYSHA:
					if (b->x > DISPLAY_SIZE_WIDTH - 320)
					{
						b->x -= (b->x - DISPLAY_SIZE_WIDTH + 320);
						b->dx *= -1;
					}
					if (b->y > DISPLAY_SIZE_HEIGHT - 200)
					{
						b->y -= (b->y - DISPLAY_SIZE_HEIGHT + 200);
						b->dy *= -1;
					}
					break;
				default:
					if (b->x > DISPLAY_SIZE_WIDTH)
					{
						b->x  -= (b->x - DISPLAY_SIZE_WIDTH);
						b->dx *= -1;
					}
					if (b->y > DISPLAY_SIZE_HEIGHT)
					{
						b->y -= (b->y - DISPLAY_SIZE_HEIGHT);
						b->dy *= -1;
					}
				}
			}

			redraw = true;
			break;

		case ALLEGRO_EVENT_KEY_DOWN:
			break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			done = true;
			break;
		}

		if (done)
		{
			break;
		}

		if (redraw && al_is_event_queue_empty(queue))
		{
			ALLEGRO_VERTEX v[4];
			al_clear_to_color(al_map_rgb(0, 0, 0));

			for (int i = 0; i < BT_N * 2; i++)
			{
				BOUNCER* b = &obj[i];
				switch (b->type)
				{
				case BT_HELLO:
					al_draw_text(font, al_map_rgb(255, 255, 255),
						b->x, b->y, 0, "Hello World!");
					break;

				case BT_MYSHA:
					al_draw_bitmap(mysha, b->x, b->y, 0);
					break;

				case BT_TRIANGLE:
					al_draw_filled_triangle(b->x, b->y, b->x + 50, b->y + 25,
						b->x, b->y + 50, al_map_rgb_f(0, 1, 0));
					break;

				case BT_RECTANGLE_1:
					al_draw_filled_rectangle(b->x, b->y, b->x + 100, b->y + 80,
						al_map_rgba_f(0, 0, 0.5, 0.5));
					break;

				case BT_RECTANGLE_2:
					v[0].x = b->x;		 v[0].y = b->y;		  v[0].z = 0; v[0].color = al_map_rgb_f(1, 0, 0);
					v[1].x = b->x + 120; v[1].y = b->y;		  v[1].z = 0; v[1].color = al_map_rgb_f(0, 1, 0);
					v[2].x = b->x;		 v[2].y = b->y + 100; v[2].z = 0; v[2].color = al_map_rgb_f(0, 0, 1);
					v[3].x = b->x + 120; v[3].y = b->y + 100; v[3].z = 0; v[3].color = al_map_rgb_f(1, 1, 0);

					al_draw_prim(v, NULL, NULL, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP);
					break;

				case BT_CIRCLE:
					al_draw_circle(b->x, b->y, 30, al_map_rgb_f(1, 0, 1), 2);
					break;

				case BT_LINE1:
					al_draw_line(b->x, b->y, b->x + 20, b->y + 100, al_map_rgb_f(1, 0, 0), 1);
					break;

				case BT_LINE2:
					al_draw_line(b->x, b->y, b->x + 70, b->y - 20, al_map_rgb_f(1, 1, 0), 1);
					break;
				}
			}


			al_flip_display();
			redraw = false;
		}
	}

	al_destroy_bitmap(mysha);
	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	return 0;
}
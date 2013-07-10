#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "mini-printf.h"

//#define DEBUG
	
#define SCREEN_HEIGHT 168
#define SCREEN_WIDTH 144
#define BACKCOLOR GColorBlack
#define FORECOLOR GColorWhite
	
#define LAYER_COUNT 3
#define LAYER_MINUTE 0
#define LAYER_MODIFIER 1
#define LAYER_HOUR 2
	
#define BUFFER_SIZE 32
#define container_of(ptr, type, member) \
	({ \
		char *__mptr = (char *)(uintptr_t) (ptr); \
		(type *)(__mptr - offsetof(type,member) ); \
	 })
	
#define MY_UUID { 0x47, 0x1F, 0x4C, 0xDC, 0x27, 0x8C, 0x4C, 0xDB, 0x91, 0x52, 0xBE, 0x0E, 0x48, 0xF4, 0x66, 0x17 }
PBL_APP_INFO(MY_UUID,
			  #ifndef DEBUG
               "Filipino Time",
             #else
               "FilipinoTime-debug",
             #endif
			 "ihopethisnamecounts",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
static PblTm now;

#define FONT_COUNT 3
static GFont fonts[FONT_COUNT];
static int current_font;

typedef struct
{
	int id;
	Layer layer;
	int flag;
	int font_size;
	char text[BUFFER_SIZE];
} layer_info;

static layer_info layers[LAYER_COUNT] =
{
	{ .id = LAYER_MINUTE, .flag = 0, .font_size = 0 },
	{ .id = LAYER_MODIFIER, .flag = 0, .font_size = 0 },
	{ .id = LAYER_HOUR, .flag = 0, .font_size = 0 }
};

static bool check_value(Layer *me)
{
	layer_info *parent = container_of(me, layer_info, layer);
	
	#ifndef DEBUG
		int h = now.tm_hour;
		int m = now.tm_min;
	#else
		int h = now.tm_min % 24;
		int m = now.tm_sec;
	#endif
	
	bool first_half = m < 40;
	if (first_half == false) 
	{
		h = h + 1;
		
		//we moved the hour forward, so force a refresh of the text
		if(parent->id == LAYER_HOUR) parent->flag = 0;
	}
	
	int twelve_hour = h % 12;
	if (twelve_hour == 0) twelve_hour = 12;
	
	bool has_changed = true;
	
	if (parent->id == LAYER_MINUTE)
	{
		if (m == 0)
		{
			snprintf(parent->text, BUFFER_SIZE, "Ika-%d", twelve_hour);
		}
		else if (first_half == true)
		{
			snprintf(parent->text, BUFFER_SIZE, "%d minuto", m);
		}
		else
		{
			snprintf(parent->text, BUFFER_SIZE, "%d minuto", 60 - m);
		}
	}
	else if(parent->id == LAYER_MODIFIER)
	{
		if(m == 0 && parent->flag != 1)
		{
			snprintf(parent->text, BUFFER_SIZE, "%s", "na ng");
			parent->flag = 1;
		}
		else if (first_half == true && parent->flag != 2)
		{
			snprintf(parent->text, BUFFER_SIZE, "makalipas ang");
			parent->flag = 2;
		}
		else if (first_half == false && parent->flag != 3)
		{
			snprintf(parent->text, BUFFER_SIZE, "bago mag");
			parent->flag = 3;
		}
		else
		{
			has_changed = false;
		}
	}
	else if(parent->id == LAYER_HOUR)
	{
		if(m == 0)
		{
			//"minute == 0" text only happens once per hour
			//and since the previous minute (minute == 59) is still under the same condition as the current hour,
			//we need to reset the flag to force a refresh on the text
			//this will then retrieve the "minute != 0" text
			parent->flag = 0;
		}
		
		if(h == 0 && parent->flag != 1) 
		{
			if(m == 0)
				snprintf(parent->text, BUFFER_SIZE, "%s", "hating gabi");
			else
				snprintf(parent->text, BUFFER_SIZE, "ika-%d ng hating gabi", twelve_hour);
			
			parent->flag = 1;
		}
		else if(h > 0 && h < 6 && parent->flag != 2) 
		{
			if(m == 0)
				snprintf(parent->text, BUFFER_SIZE, "%s", "madaling araw");
			else
				snprintf(parent->text, BUFFER_SIZE, "ika-%d ng madaling araw", twelve_hour);
			
			parent->flag = 2;
		}
		else if(h >= 6 && h < 12 && parent->flag != 3)
		{
			if(m == 0)
				snprintf(parent->text, BUFFER_SIZE, "%s", "umaga");
			else
				snprintf(parent->text, BUFFER_SIZE, "ika-%d ng umaga", twelve_hour);
			
			parent->flag = 3;
		}
		else if(h == 12 && parent->flag != 4)
		{
			if(m == 0)
				snprintf(parent->text, BUFFER_SIZE, "%s", "tanghali");
			else
				snprintf(parent->text, BUFFER_SIZE, "ika-%d ng tanghali", twelve_hour);
			
			parent->flag = 4;
		}
		else if(h > 12 && h < 18 && parent->flag != 5) 
		{
			if(m == 0)
				snprintf(parent->text, BUFFER_SIZE, "%s", "hapon");
			else
				snprintf(parent->text, BUFFER_SIZE, "ika-%d ng hapon", twelve_hour);
			
			parent->flag = 5;
		}
		else if(h >= 18 && h < 24 && parent->flag != 6)
		{
			if(m == 0)
				snprintf(parent->text, BUFFER_SIZE, "%s", "gabi");
			else
				snprintf(parent->text, BUFFER_SIZE, "ika-%d ng gabi", twelve_hour);
			
			parent->flag = 6;
		}
		else
		{
			has_changed = false;
		}
		
		if(m == 0)
		{
			//also, the succeeding minute (minute == 1) is still under the same condition as minute == 0
			//so we need to reset the flag to force a refresh on the text
			//this will then retrieve the "minute != 0" text
			parent->flag = 0;
		}
	}
	else
	{
		snprintf(parent->text, BUFFER_SIZE, "%s", "May problema");
	}
	
	return has_changed;
}

static void layer_update(Layer *const me, GContext *ctx)
{
	(void) ctx;
	
	layer_info *parent = container_of(me, layer_info, layer);
	
	const int width = me->bounds.size.w;
	const int height = me->bounds.size.h;
	graphics_context_set_fill_color(ctx, BACKCOLOR);
	graphics_context_set_text_color(ctx, FORECOLOR);
	graphics_fill_rect(ctx, GRect(0, 0, width, height), 0, 0);

	parent->font_size = 0;
	
	GSize content_size = 
		graphics_text_layout_get_max_used_size
		(
			ctx, 
			parent->text, 
			fonts[parent->font_size], 
			GRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), 
			GTextOverflowModeWordWrap, 
			GTextAlignmentCenter, 
			NULL
		);
	
	while(content_size.w > width || content_size.h > height)
	{
		parent->font_size++;
		content_size = 
			graphics_text_layout_get_max_used_size
			(
				ctx, 
				parent->text, 
				fonts[parent->font_size], 
				GRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), 
				GTextOverflowModeWordWrap, 
				GTextAlignmentCenter, 
				NULL
			);
	}
	
	graphics_text_draw(ctx, 
					   parent->text, 
					   fonts[parent->font_size], 
					   GRect(0, 0, width, height), 
					   GTextOverflowModeWordWrap, 
					   GTextAlignmentCenter,
					   NULL);
}

static void handle_tick(AppContextRef ctx, PebbleTickEvent *const event)
{
	(void) ctx;
	
	now = *event->tick_time;
	
	for (int i = 0; i < LAYER_COUNT; i++) 
	{
		bool has_changed = check_value(&layers[i].layer);
		if (has_changed == true) layer_mark_dirty(&layers[i].layer);
	}
}

void handle_init(AppContextRef ctx)
{
	(void)ctx;
	
	window_init(&window, "Main");
	window_stack_push(&window, true /* Animated */);
	
	resource_init_current_app(&APP_RESOURCES);
	
	for (int x = 0; x < FONT_COUNT; x++)
	{
		if (x == 0) fonts[x] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUTIEPATOOTIE_30));
		else if (x == 1) fonts[x] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUTIEPATOOTIE_25));
		else if (x == 2) fonts[x] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUTIEPATOOTIE_20));
		else fonts[x] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUTIEPATOOTIE_30));
	}
		
	get_time(&now);
	
	for (int i = 0; i < LAYER_COUNT; i++)
	{
		/*
		The layers are initially 1/3 of the screen
		Layer 1 height is adjusted to -20px
		Layer 2 height is not adjusted
		Layer 3 height is adjusted to +20px
		Layer 2 y-offset is adjusted to follow Layer 1 height
		Layer 3 y-offset is adjusted to follow Layer 1 height + Layer 2 height
			Layer 1 height = x - 20
			Layer 2 height = x
			Layer 3 height = Layer 1 height + Layer 2 height
						   = (x - 20)       + (x)
						   = 2x - 20
		*/
		GRect frame;
		if(i == LAYER_MINUTE) frame = GRect(0, 0, SCREEN_WIDTH, (SCREEN_HEIGHT / LAYER_COUNT) - 20);
		else if(i == LAYER_MODIFIER) frame = GRect(0, (SCREEN_HEIGHT / LAYER_COUNT) - 20, SCREEN_WIDTH, (SCREEN_HEIGHT / LAYER_COUNT));
		else if(i == LAYER_HOUR) frame = GRect(0, (2 * SCREEN_HEIGHT / LAYER_COUNT) - 20, SCREEN_WIDTH, (SCREEN_HEIGHT / LAYER_COUNT) + 20);
		else frame = GRect(0, i * (SCREEN_HEIGHT / LAYER_COUNT), SCREEN_WIDTH, (SCREEN_HEIGHT / LAYER_COUNT));
			
		layer_init(&layers[i].layer, frame);
		layer_set_update_proc(&layers[i].layer, layer_update);
		layer_add_child(&window.layer, &layers[i].layer);
		
		bool has_changed = check_value(&layers[i].layer);
		if (has_changed == true) layer_mark_dirty(&layers[i].layer);
	}
}

static void handle_deinit(AppContextRef ctx)
{
	(void) ctx;
	
	for (int x = 0; x < FONT_COUNT; x++)
	{
		fonts_unload_custom_font(fonts[x]);
	}
}

void pbl_main(void *params) 
{
	PebbleAppHandlers handlers = 
	{
		.init_handler = &handle_init,
		.deinit_handler = &handle_deinit,
		.tick_info      = 
		{
			.tick_handler = &handle_tick,
			
			#ifndef DEBUG
				.tick_units = MINUTE_UNIT
			#else
				.tick_units = SECOND_UNIT
			#endif
		},
	};
	app_event_loop(params, &handlers);
}

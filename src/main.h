#include "pebble.h"

#define SPLASH_DELAY 2000
	
#define LAYER_COUNT 3
#define LAYER_MINUTE 0
#define LAYER_MODIFIER 1
#define LAYER_HOUR 2
	
#define SCREEN_HEIGHT 168
#define SCREEN_WIDTH 144
	
#define BUFFER_SIZE 32

#define FONT_COUNT 4
GFont fonts[FONT_COUNT];
int current_font;
char *hour_text[12] = 
{
	"alas dose", "ala una", "alas dos", "alas tres", 
	"alas kwatro", "alas singko", "alas sais", "alas siyete", 
	"alas otso", "alas nwebe", "alas diyes", "alas onse"
};

Window *window;
AppTimer *timer;
struct tm *now;

int current_day;
bool is_holiday;

typedef struct 
{
	TextLayer *layer;
	int id;
	int flag;
	int font_size;
	char *text;
} layer_info;

layer_info layers[LAYER_COUNT] =
{
	{ .id = LAYER_MINUTE, .flag = 0, .font_size = 0, .text = " " },
	{ .id = LAYER_MODIFIER, .flag = 0, .font_size = 0, .text = " " },
	{ .id = LAYER_HOUR, .flag = 0, .font_size = 0, .text = " " }
};

char *splash_text[LAYER_COUNT] = 
{
	"Created by",
	"ihtnc",
	"(c) 2013 FilipinoTime"
};

typedef struct 
{
	bool is_animating;
	uint32_t duration;
	bool flags[5];
	bool current_flag;
	int index;
} blink_info;

blink_info animation =
{
	.duration = 200,
	.current_flag = false,
	.index = 0,
	.is_animating = false,
	.flags = {true, false, true, false, true}
};

bool is_splash_showing;


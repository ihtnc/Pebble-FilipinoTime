#include "pebble.h"

#define SPLASH_DELAY 2000
	
#define LAYER_COUNT 3
#define LAYER_MINUTE 0
#define LAYER_MODIFIER 1
#define LAYER_HOUR 2
	
#define SCREEN_HEIGHT 168
#define SCREEN_WIDTH 144

#define JAN 0
#define FEB 1
#define MAR 2
#define APR 3
#define MAY 4
#define JUN 5
#define JUL 6
#define AUG 7
#define SEP 8
#define OCT 9
#define NOV 10
#define DEC 11
	
#define BUFFER_SIZE 32

#define FONT_COUNT 4
#define FONT_SMALLEST 3
#define FONT_SMALL 2
#define FONT_MEDIUM 1
#define FONT_LARGE 0
	
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
bool is_splash_showing;

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
	{ .id = LAYER_MINUTE, .flag = 0, .font_size = FONT_SMALLEST, .text = " " },
	{ .id = LAYER_MODIFIER, .flag = 0, .font_size = FONT_SMALLEST, .text = " " },
	{ .id = LAYER_HOUR, .flag = 0, .font_size = FONT_SMALLEST, .text = " " }
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

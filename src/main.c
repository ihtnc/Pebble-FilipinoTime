#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "mini-printf.h"
#include "monitor.h"
#include "app_info.h"
#include "app_options.h"
	
/* 
Because of the way that httpebble works, a different UUID is needed for Android and iOS compatibility. 
If you are building this to use with Android, then leave the #define ANDROID line alone, and if 
you're building for iOS then remove or comment out that line.
*/

#ifdef ANDROID
	#define MY_UUID { 0x91, 0x41, 0xB6, 0x28, 0xBC, 0x89, 0x49, 0x8E, 0xB1, 0x47, 0x10, 0x34, 0xBF, 0xBE, 0x12, 0x98 }
#else
	#define MY_UUID HTTP_UUID
#endif
	
PBL_APP_INFO(MY_UUID, APP_NAME, APP_AUTHOR,
             APP_VER_MAJOR, APP_VER_MINOR, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
			 #ifndef DEBUG
               APP_INFO_WATCH_FACE
             #else
               APP_INFO_STANDARD_APP
             #endif             
			);
	
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

#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6

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

Window window;
static PblTm now;

#define FONT_COUNT 4
static GFont fonts[FONT_COUNT];
static int current_font;
const char *hour_text[12] = 
{
	"alas dose", "ala una", "alas dos", "alas tres", 
	"alas kwatro", "alas singko", "alas sais", "alas siyete", 
	"alas otso", "alas nwebe", "alas diyes", "alas onse"
};
	
typedef struct
{
	const int id;
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

static const char *splash_text[LAYER_COUNT] = 
{
	"Created by",
	"ihtnc",
	"(c) 2013 FilipinoTime"
};

typedef struct
{
	bool is_animating;
	uint32_t duration;
	Layer blank_layer;
	bool flags[5];
	bool current_flag;
	int index;
	AppContextRef bctx;
} blink_info;

static blink_info animation =
{
	.duration = 200,
	.current_flag = false,
	.index = 0,
	.is_animating = false,
	.flags = {true, false, true, false, true}
};

static bool is_splash_showing;

typedef struct 
{
	int y;
	int m;
	int d;
} date;

static int current_day;
bool is_holiday;

static bool get_holiday_text(int layer_id, char *text)
{
	if(current_day == now.tm_mday && is_holiday == false)
	{
		return false;
	}
	
	current_day = now.tm_mday;	
	is_holiday = false;
	
	if (now.tm_mon == JAN && now.tm_mday == 1) 
	{
		is_holiday = true; // New year's day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "bagong");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "taon!");
		else is_holiday = false;
	}
	else if (now.tm_mon == APR && now.tm_mday == 9)
	{
		is_holiday = true; // Day of valor
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ng");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "kagitingan!");
		else is_holiday = false;
	}
	else if (now.tm_mon == MAY && now.tm_mday == 1) 
	{
		is_holiday = true; // Labor day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ng");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "manggagawa!");
		else is_holiday = false;
	}
	else if (now.tm_mon == JUN && now.tm_mday == 12)
	{
		is_holiday = true; // Independence day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ng");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "kalayaan!");
		else is_holiday = false;
	}
	else if (now.tm_mon == AUG && now.tm_mday == 21)
	{
		is_holiday = true; // Ninoy Aquino's day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ni");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "Ninoy!");
		else is_holiday = false;
	}
	else if (now.tm_mon == AUG && now.tm_mday == 26)
	{
		is_holiday = true; // National heroes day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ng mga");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "bayani!");
		else is_holiday = false;
	}
	else if (now.tm_mon == NOV && now.tm_mday == 1)
	{
		is_holiday = true; // All saints day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ng mga");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "santo!");
		else is_holiday = false;
	}
	else if (now.tm_mon == NOV && now.tm_mday == 2)
	{
		is_holiday = true; // All souls day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ng mga");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "patay!");
		else is_holiday = false;
	}
	else if (now.tm_mon == NOV && now.tm_mday == 30)
	{
		is_holiday = true; // Bonifacio day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ni");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "Bonifacio!");
		else is_holiday = false;
	}
	else if (now.tm_mon == DEC && now.tm_mday == 24) 
	{
		is_holiday = true; // Christmas eve
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "bisperas ng");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "pasko!");
		else is_holiday = false;
	}
	else if (now.tm_mon == DEC && now.tm_mday == 25) 
	{
		is_holiday = true; // Christmas day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ng");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "pasko!");
		else is_holiday = false;
	}
	else if (now.tm_mon == DEC && now.tm_mday == 30) 
	{
		is_holiday = true; // Rizal day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ni");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "Rizal!");
		else is_holiday = false;
	}
	else if (now.tm_mon == DEC && now.tm_mday == 31) 
	{
		is_holiday = true; // New year's eve
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "bisperas ng");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "bagong taon!");
		else is_holiday = false;
	}
	else if((now.tm_mon == MAR && now.tm_mday >= 19)
			|| (now.tm_mon == APR && now.tm_mday <= 25)) 
	{
		//calculate easter sunday and from there calculate maundy thursday and good friday
		//easter can never occur before March 22 or later than April 25
		//so this condition should cover from March 19 (earliest possible maundy thursday)
		
		int year = now.tm_year + 1900;
		int cen = year % 19;
		int leap = year % 4;
		int days = year % 7;
		int temp1 = (19 * cen + 24) % 30;
		int temp2 = (2 * leap + 4 * days + 6 * temp1 + 5) % 7;
		int easter = 22 + temp1 + temp2;
		
		date easter_sun;
		//min easter = 22
		//max easter = 56
		if (easter > 31) 
		{
			easter_sun.y = now.tm_year;
			easter_sun.m = APR;
			easter_sun.d = (easter - 31);
		}
		else
		{
			easter_sun.y = now.tm_year;
			easter_sun.m = MAR;
			easter_sun.d = easter;
		}
		
		date maundy_thu = 
		{
			.y = easter_sun.y,
			.m = (easter_sun.d - 3 >= 1) ? easter_sun.m : MAR,
			.d = (easter_sun.d - 3 >= 1) ? easter_sun.d - 3 : (31 + easter_sun.d - 3)
		};
		
		date good_fri = 
		{
			.y = easter_sun.y,
			.m = (easter_sun.d - 2 >= 1) ? easter_sun.m : MAR,
			.d = (easter_sun.d - 2 >= 1) ? easter_sun.d - 2 : (31 + easter_sun.d - 2)
		};
		
		date black_sat = 
		{
			.y = easter_sun.y,
			.m = (easter_sun.d - 1 >= 1) ? easter_sun.m : MAR,
			.d = (easter_sun.d - 1 >= 1) ? easter_sun.d - 1 : (31 + easter_sun.d - 1)
		};
		
		if(now.tm_mon == maundy_thu.m && now.tm_mday == maundy_thu.d)
		{
			is_holiday = true; // Maundy thursday
			
			if(layer_id == LAYER_MINUTE)
				snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
			else if(layer_id == LAYER_MODIFIER)
				snprintf(text, BUFFER_SIZE, "%s", "huwebes");
			else if(layer_id == LAYER_HOUR)
				snprintf(text, BUFFER_SIZE, "%s", "santo!");
			else
				is_holiday = false;
		}
		else if(now.tm_mon == good_fri.m && now.tm_mday == good_fri.d)
		{
			is_holiday = true; // Good friday
			
			if(layer_id == LAYER_MINUTE)
				snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
			else if(layer_id == LAYER_MODIFIER)
				snprintf(text, BUFFER_SIZE, "%s", "biyernes");
			else if(layer_id == LAYER_HOUR)
				snprintf(text, BUFFER_SIZE, "%s", "santo!");
			else
				is_holiday = false;
		}	
		else if(now.tm_mon == black_sat.m && now.tm_mday == black_sat.d)
		{
			is_holiday = true; // Black saturday
			
			if(layer_id == LAYER_MINUTE)
				snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
			else if(layer_id == LAYER_MODIFIER)
				snprintf(text, BUFFER_SIZE, "%s", "sabado");
			else if(layer_id == LAYER_HOUR)
				snprintf(text, BUFFER_SIZE, "%s", "de gloria!");
			else
				is_holiday = false;
		}
	}
	
	return is_holiday;
}

static bool check_text(Layer *me)
{
	layer_info *parent = container_of(me, layer_info, layer);
	
	int h = now.tm_hour;
	int m = now.tm_min;
	
	bool first_half = m < COUNT_UP_CUTOVER;
	if (first_half == false) 
	{
		h = (h + 1) % 24;
		
		//we moved the hour forward, so force a refresh of the text
		if(parent->id == LAYER_HOUR) parent->flag = 0;
	}
	
	#ifdef DEBUG
		//if we're debugging, always refresh the hour layer (since we could be counting up or down)
		if(parent->id == LAYER_HOUR) parent->flag = 0;
	#endif

	int twelve_hour = h % 12;
	
	bool has_changed = true;
	bool is_holiday = false;
	
	if (parent->id == LAYER_MINUTE)
	{
		if (m == 0) 
		{
			if(INCLUDE_HOLIDAY == true)
				is_holiday = get_holiday_text(parent->id, parent->text);
			
			if(is_holiday == false)
				snprintf(parent->text, BUFFER_SIZE, "%s", hour_text[twelve_hour]);
		}
		else if (first_half == true) snprintf(parent->text, BUFFER_SIZE, "%d minuto", m);
		else snprintf(parent->text, BUFFER_SIZE, "%d minuto", 60 - m);
	}
	else if(parent->id == LAYER_MODIFIER)
	{
		if(m == 0 && parent->flag != 1)
		{
			if(INCLUDE_HOLIDAY == true)
				is_holiday = get_holiday_text(parent->id, parent->text);
			
			if(is_holiday == false)
				snprintf(parent->text, BUFFER_SIZE, "%s", "na ng");
			
			parent->flag = 1;
		}
		else if (m != 0 && first_half == true && parent->flag != 2)
		{
			snprintf(parent->text, BUFFER_SIZE, "makalipas ang");
			parent->flag = 2;
		}
		else if (m != 0 && first_half == false && parent->flag != 3)
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
		//"minute == 0" text only happens once per hour
		//and since the previous minute (minute == 59) is still under the same condition as the current hour,
		//we need to reset the flag to force a refresh on the text
		//this will then retrieve the "minute != 0" text	
		if(m == 0) parent->flag = 0;
		
		if(h == 0 && parent->flag != 1) 
		{
			if(m == 0) 
			{
				if(INCLUDE_HOLIDAY == true)
					is_holiday = get_holiday_text(parent->id, parent->text);
			
				if(is_holiday == false)
					snprintf(parent->text, BUFFER_SIZE, "%s", "hating gabi");
			}
			else
			{
				snprintf(parent->text, BUFFER_SIZE, "%s ng hating gabi", hour_text[twelve_hour]);
			}
			
			parent->flag = 1;
		}
		else if(h > 0 && h < 6 && parent->flag != 2) 
		{
			if(m == 0) 
			{
				if(INCLUDE_HOLIDAY == true)
					is_holiday = get_holiday_text(parent->id, parent->text);
			
				if(is_holiday == false)
					snprintf(parent->text, BUFFER_SIZE, "%s", "madaling araw");
			}
			else
			{
				snprintf(parent->text, BUFFER_SIZE, "%s ng madaling araw", hour_text[twelve_hour]);
			}
			
			parent->flag = 2;
		}
		else if(h >= 6 && h < 12 && parent->flag != 3)
		{
			if(m == 0) 
			{
				if(INCLUDE_HOLIDAY == true)
					is_holiday = get_holiday_text(parent->id, parent->text);
			
				if(is_holiday == false)
					snprintf(parent->text, BUFFER_SIZE, "%s", "umaga");
				else if(h == 9)
					vibes_double_pulse();
			}
			else 
			{
				snprintf(parent->text, BUFFER_SIZE, "%s ng umaga", hour_text[twelve_hour]);
			}
			
			parent->flag = 3;
		}
		else if(h == 12 && parent->flag != 4)
		{
			if(m == 0) 
			{
				if(INCLUDE_HOLIDAY == true)
					is_holiday = get_holiday_text(parent->id, parent->text);
			
				if(is_holiday == false)
					snprintf(parent->text, BUFFER_SIZE, "%s", "tanghali");
				else
					vibes_double_pulse();
			}
			else snprintf(parent->text, BUFFER_SIZE, "%s ng tanghali", hour_text[twelve_hour]);
			
			parent->flag = 4;
		}
		else if(h > 12 && h < 18 && parent->flag != 5) 
		{
			if(m == 0) 
			{
				if(INCLUDE_HOLIDAY == true)
					is_holiday = get_holiday_text(parent->id, parent->text);
			
				if(is_holiday == false)
					snprintf(parent->text, BUFFER_SIZE, "%s", "hapon");
				else if(h == 15)
					vibes_double_pulse();
			}
			else
			{
				snprintf(parent->text, BUFFER_SIZE, "%s ng hapon", hour_text[twelve_hour]);
			}
			
			parent->flag = 5;
		}
		else if(h >= 18 && h < 24 && parent->flag != 6)
		{
			if(m == 0)
			{
				if(INCLUDE_HOLIDAY == true)
					is_holiday = get_holiday_text(parent->id, parent->text);
			
				if(is_holiday == false)
					snprintf(parent->text, BUFFER_SIZE, "%s", "gabi");
				else if(h == 18)
					vibes_double_pulse();
			}
			else
			{
				snprintf(parent->text, BUFFER_SIZE, "%s ng gabi", hour_text[twelve_hour]);
			}
			
			parent->flag = 6;
		}
		else
		{
			has_changed = false;
		}
		
		//also, the succeeding minute (minute == 1) is still under the same condition as minute == 0
		//so we need to reset the flag to force a refresh on the text
		//this will then retrieve the "minute != 0" text	
		if(m == 0) parent->flag = 0;
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
	
	if(DYNAMIC_FONT_SIZE == true)
	{
		//start with the largest font
		//try to calculate the resulting size of the layer given the text and the font
		//if the size is greater than the bounds of the layer,
		//    go to the next largest font
		//    this is the reason why the fonts are ordered in a descending manner on the font array
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
	}
	else
	{
		//show the second to the smallest font
		parent->font_size = 2;
	}
		
	graphics_text_draw(ctx, 
					   parent->text, 
					   fonts[parent->font_size],
					   GRect(0, 0, width, height), 
					   GTextOverflowModeWordWrap, 
					   GTextAlignmentCenter,
					   NULL);
}

static void blank_layer_update(Layer *const me, GContext *ctx)
{
	graphics_context_set_fill_color(ctx, BACKCOLOR);
	graphics_context_set_text_color(ctx, FORECOLOR);
	graphics_fill_rect(ctx, GRect(0, 0, me->bounds.size.w,  me->bounds.size.h), 0, 0);
}

static void show_splash()
{
	if (animation.is_animating == true) return;
	
	for (int i = 0; i < LAYER_COUNT; i++) 
	{
		snprintf(layers[i].text, BUFFER_SIZE, "%s", splash_text[i]);
	}
	
	is_splash_showing = true;
	animation.current_flag = false;
	animation.index = 0;
	animation.is_animating = true;
	app_timer_send_event(animation.bctx, SPLASH_DELAY, 0);
}

static void blink_screen()
{
	if (ENABLE_BLINK == false) return;
	if (animation.is_animating == true) return;
	
	animation.current_flag = false;
	animation.index = 0;
	animation.is_animating = true;
	app_timer_send_event(animation.bctx, animation.duration, 0);
}

static void get_time_value(PblTm *time)
{
	(void) *time;
	
	#ifndef DEBUG
		get_time(time);
	#else
		now.tm_year = 113;
		now.tm_mon = DEC;
		now.tm_mday = 30;
		now.tm_min = 59;
		now.tm_hour = 8;
		time = &now;
	#endif
}

//for each tick of the timer, walk through the flag list of the animation class
//a false flag will show the original layers, while a true flag will show the blank layer
//do not do anything if the flag has not changed from the last iteration
//at the end of the flag list, stop the animation
//    if the animation is for the splash screen,
//        force a redraw of the the main screen since the minute tick may still be a while
static void handle_timer(AppContextRef ctx, AppTimerHandle handle, uint32_t cookie)
{
	if(animation.index >= (int) sizeof(animation.flags)) 
	{
		if(animation.current_flag == true) 
		{
			layer_remove_child_layers(&window.layer);
			
			for (int i = 0; i < LAYER_COUNT; i++) 
			{
				layer_add_child(&window.layer, &layers[i].layer);
			}
		}
		
		animation.is_animating = false;
		app_timer_cancel_event(ctx, handle);
		
		if(is_splash_showing == true) is_splash_showing = false;
		
		//blink animation has just ended
		//so draw the main watch face even if the minute did not tick yet
		//this is to clear the previous screen and show the time immediately
		get_time_value(&now);
		
		for (int i = 0; i < LAYER_COUNT; i++) 
		{
			bool has_changed = check_text(&layers[i].layer);
			if (has_changed == true) 
			{
				if (animation.is_animating == false) layer_mark_dirty(&layers[i].layer);
			}
		}
		
		return;
	}
	
	if(animation.current_flag != animation.flags[animation.index])
	{
		animation.current_flag = animation.flags[animation.index];
			
		if(animation.current_flag == true)
		{
			layer_remove_child_layers(&window.layer);
			layer_add_child(&window.layer, &animation.blank_layer);
		}
		else
		{
			layer_remove_child_layers(&window.layer);
			
			for (int i = 0; i < LAYER_COUNT; i++) 
			{
				layer_add_child(&window.layer, &layers[i].layer);
			}
		}
	}
	
	animation.index++;
	app_timer_send_event(animation.bctx, animation.duration, cookie);
}

static void manual_handle_tick()
{
	blink_screen();
	ping();
}

static void handle_tick(AppContextRef ctx, PebbleTickEvent *const event)
{
	if(is_splash_showing == true) return;
	
	(void) ctx;
		
	#ifndef DEBUG
		now = *event->tick_time;
		manual_handle_tick();
	#endif
}

#ifdef DEBUG
	static bool isLeapYear(int year) 
	{
		year = year + 1900;
		return (((year%4 == 0) && (year%100 != 0)) || (year%400 == 0));
	}
	
	static int numDaysInMonth(const int month, const int year) 
	{
		static const int nDays[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		
		return nDays[month] + (month == FEB)*isLeapYear(year);
	}

	static void add_minutes(const int minutes)
	{
		int value = (now.tm_min + minutes);
		now.tm_min = (60 + value) % 60;
	 	value = value / 60 - (value < 0 && value % 60 != 0 ? 1 : 0); //because -value/60 seems to rounds down to -inf instead of 0
		
		value = (now.tm_hour + value);
		now.tm_hour = (24 + value) % 24;
		value = value / 24 - (value < 0 && value % 24 != 0 ? 1 : 0); //because -value/24 seems to  rounds down to -inf instead of 0
		
		now.tm_mday = now.tm_mday + value;
		int daysInMonth = numDaysInMonth(now.tm_mon, now.tm_year);
		if(now.tm_mday > daysInMonth)
		{
			do
			{
				now.tm_mday = now.tm_mday - daysInMonth;
				
				if(now.tm_mon == DEC) 
				{
					now.tm_mon = JAN;
					now.tm_year = now.tm_year + 1;
				}
				else 
				{
					now.tm_mon = now.tm_mon + 1;
				}
				
				daysInMonth = numDaysInMonth(now.tm_mon, now.tm_year);	
			}
			while(now.tm_mday > daysInMonth);			
		}
		else if(now.tm_mday < 0)
		{
			do
			{
				if(now.tm_mon == JAN)
				{
					now.tm_mon = DEC;
					now.tm_year = now.tm_year - 1;
				}
				else
				{
					now.tm_mon = now.tm_mon - 1;
				}
				
				daysInMonth = numDaysInMonth(now.tm_mon, now.tm_year);
				now.tm_mday = now.tm_mday + daysInMonth;
			}
			while(now.tm_mday < 0);	
		}
	}

	static void add_hours(const int hours)
	{
		add_minutes(hours * 60);
	}

	static void add_days(const int days)
	{
		add_hours(days * 24);
	}

	void handle_up_single_click(ClickRecognizerRef recognizer, Window *window) 
	{	
		add_minutes(1);
		manual_handle_tick();
	}
	
	void handle_up_multi_click(ClickRecognizerRef recognizer, Window *window) 
	{
		const uint16_t count = click_number_of_clicks_counted(recognizer);
		if(count == 2) add_hours(1);
		else if(count == 3) add_days(1);
		else add_minutes(1);
		
		manual_handle_tick();
	}

	void handle_select_single_click(ClickRecognizerRef recognizer, Window *window) 
	{
		get_time(&now);
		manual_handle_tick();
	}
	
	void handle_down_single_click(ClickRecognizerRef recognizer, Window *window) 
	{			
		add_minutes(-1);
		manual_handle_tick();
	}

	void handle_down_multi_click(ClickRecognizerRef recognizer, Window *window) 
	{
		const uint16_t count = click_number_of_clicks_counted(recognizer);
		if(count == 2) add_hours(-1);
		else if(count == 3) add_days(-1);
		else add_minutes(-1);
		
		manual_handle_tick();
	}

	void config_provider(ClickConfig **config, Window *window) 
	{
		config[BUTTON_ID_UP]->click.handler = (ClickHandler) handle_up_single_click;
		config[BUTTON_ID_UP]->click.repeat_interval_ms = 250;
		
		config[BUTTON_ID_UP]->multi_click.handler = (ClickHandler) handle_up_multi_click;
  		config[BUTTON_ID_UP]->multi_click.min = 2;
  		config[BUTTON_ID_UP]->multi_click.max = 3;
  		config[BUTTON_ID_UP]->multi_click.last_click_only = true;
		
		config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) handle_down_single_click;
		config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 250;	
		
		config[BUTTON_ID_DOWN]->multi_click.handler = (ClickHandler) handle_down_multi_click;
  		config[BUTTON_ID_DOWN]->multi_click.min = 2;
  		config[BUTTON_ID_DOWN]->multi_click.max = 3;
  		config[BUTTON_ID_DOWN]->multi_click.last_click_only = true;
		
		config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) handle_select_single_click;
	}
#endif
	
void handle_init(AppContextRef ctx)
{
	(void)ctx;
	
	window_init(&window, "Main");
	
	#ifdef DEBUG
		window_set_fullscreen(&window, true);
		window_set_click_config_provider(&window, (ClickConfigProvider) config_provider);
	#endif
		
	window_stack_push(&window, true /* Animated */);
	
	resource_init_current_app(&APP_RESOURCES);
	
	for (int x = 0; x < FONT_COUNT; x++)
	{
		if (x == 0) fonts[x] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUTIEPATOOTIE_40));
		else if (x == 1) fonts[x] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUTIEPATOOTIE_30));
		else if (x == 2) fonts[x] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUTIEPATOOTIE_25));
		else if (x == 3) fonts[x] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUTIEPATOOTIE_20));
		else fonts[x] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUTIEPATOOTIE_40));
	}
	
	layer_init(&animation.blank_layer, GRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	layer_set_update_proc(&animation.blank_layer, blank_layer_update);
	animation.bctx = ctx;
	
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
		
		layers[i].flag = 0;		
	}
	
	get_time(&now);
	current_day = 0;
	is_holiday = false;
	
	monitor_init(ctx); 
	show_splash();
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
		
		#ifndef DEBUG
			.tick_info = 
			{
				.tick_handler = &handle_tick,
				.tick_units = MINUTE_UNIT
			},
		#endif
		.messaging_info = 
		{
			.buffer_sizes = 
			{
				.inbound = 124,
				.outbound = 256
			}
		},
		.timer_handler = &handle_timer
	};
	app_event_loop(params, &handlers);
}

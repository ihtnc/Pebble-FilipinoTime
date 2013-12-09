#include "pebble.h"
#include "main.h"
#include "btmonitor.h"
#include "thincfg.h"
#include "calendar.h"
#include "options.h"

static void handle_timer(void *data);

static bool get_holiday_text(int layer_id, char *text)
{
	if(current_day == now->tm_mday && is_holiday == false)
	{
		return false;
	}
	
	current_day = now->tm_mday;	
	is_holiday = false;
	
	if (now->tm_mon == JAN && now->tm_mday == 1) 
	{
		is_holiday = true; // New year's day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "bagong");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "taon!");
		else is_holiday = false;
	}
	else if (now->tm_mon == APR && now->tm_mday == 9)
	{
		is_holiday = true; // Day of valor
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ng");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "kagitingan!");
		else is_holiday = false;
	}
	else if (now->tm_mon == MAY && now->tm_mday == 1) 
	{
		is_holiday = true; // Labor day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ng");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "manggagawa!");
		else is_holiday = false;
	}
	else if (now->tm_mon == JUN && now->tm_mday == 12)
	{
		is_holiday = true; // Independence day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ng");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "kalayaan!");
		else is_holiday = false;
	}
	else if (now->tm_mon == AUG && now->tm_mday == 21)
	{
		is_holiday = true; // Ninoy Aquino's day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ni");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "Ninoy!");
		else is_holiday = false;
	}
	else if (now->tm_mon == AUG && now->tm_mday == 26)
	{
		is_holiday = true; // National heroes day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ng mga");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "bayani!");
		else is_holiday = false;
	}
	else if (now->tm_mon == NOV && now->tm_mday == 1)
	{
		is_holiday = true; // All saints day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ng mga");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "santo!");
		else is_holiday = false;
	}
	else if (now->tm_mon == NOV && now->tm_mday == 2)
	{
		is_holiday = true; // All souls day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ng mga");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "patay!");
		else is_holiday = false;
	}
	else if (now->tm_mon == NOV && now->tm_mday == 30)
	{
		is_holiday = true; // Bonifacio day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ni");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "Bonifacio!");
		else is_holiday = false;
	}
	else if (now->tm_mon == DEC && now->tm_mday == 24) 
	{
		is_holiday = true; // Christmas eve
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "bisperas ng");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "pasko!");
		else is_holiday = false;
	}
	else if (now->tm_mon == DEC && now->tm_mday == 25) 
	{
		is_holiday = true; // Christmas day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ng");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "pasko!");
		else is_holiday = false;
	}
	else if (now->tm_mon == DEC && now->tm_mday == 30) 
	{
		is_holiday = true; // Rizal day
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "araw ni");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "Rizal!");
		else is_holiday = false;
	}
	else if (now->tm_mon == DEC && now->tm_mday == 31) 
	{
		is_holiday = true; // New year's eve
		
		if(layer_id == LAYER_MINUTE) snprintf(text, BUFFER_SIZE, "%s", "ngayon ay");
		else if(layer_id == LAYER_MODIFIER) snprintf(text, BUFFER_SIZE, "%s", "bisperas ng");
		else if(layer_id == LAYER_HOUR) snprintf(text, BUFFER_SIZE, "%s", "bagong taon!");
		else is_holiday = false;
	}
	else if((now->tm_mon == MAR && now->tm_mday >= 19)
			|| (now->tm_mon == APR && now->tm_mday <= 25)) 
	{
		//calculate easter sunday and from there calculate maundy thursday and good friday
		//easter can never occur before March 22 or later than April 25
		//so this condition should cover from March 19 (earliest possible maundy thursday)
		
		int year = now->tm_year + 1900;
		int cen = year % 19;
		int leap = year % 4;
		int days = year % 7;
		int temp1 = (19 * cen + 24) % 30;
		int temp2 = (2 * leap + 4 * days + 6 * temp1 + 5) % 7;
		int easter = 22 + temp1 + temp2;
		
		struct tm easter_sun;
		//min easter = 22
		//max easter = 56
		if (easter > 31) 
		{
			easter_sun.tm_year = now->tm_year;
			easter_sun.tm_mon = APR;
			easter_sun.tm_mday = (easter - 31);
		}
		else
		{
			easter_sun.tm_year = now->tm_year;
			easter_sun.tm_mon = MAR;
			easter_sun.tm_mday = easter;
		}
		
		struct tm maundy_thu = 
		{
			.tm_year = easter_sun.tm_year,
			.tm_mon = (easter_sun.tm_mday - 3 >= 1) ? easter_sun.tm_mon : MAR,
			.tm_mday = (easter_sun.tm_mday - 3 >= 1) ? easter_sun.tm_mday - 3 : (31 + easter_sun.tm_mday - 3)
		};
		
		struct tm good_fri = 
		{
			.tm_year = easter_sun.tm_year,
			.tm_mon = (easter_sun.tm_mday - 2 >= 1) ? easter_sun.tm_mon : MAR,
			.tm_mday = (easter_sun.tm_mday - 2 >= 1) ? easter_sun.tm_mday - 2 : (31 + easter_sun.tm_mday - 2)
		};
		
		struct tm black_sat = 
		{
			.tm_year = easter_sun.tm_year,
			.tm_mon = (easter_sun.tm_mday - 1 >= 1) ? easter_sun.tm_mon : MAR,
			.tm_mday = (easter_sun.tm_mday - 1 >= 1) ? easter_sun.tm_mday - 1 : (31 + easter_sun.tm_mday - 1)
		};
		
		if(now->tm_mon == maundy_thu.tm_mon && now->tm_mday == maundy_thu.tm_mday)
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
		else if(now->tm_mon == good_fri.tm_mon && now->tm_mday == good_fri.tm_mday)
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
		else if(now->tm_mon == black_sat.tm_mon && now->tm_mday == black_sat.tm_mday)
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

static bool check_text(layer_info *me)
{
	int h = now->tm_hour;
	int m = now->tm_min;

	bool first_half = m < get_count_up_cutover_value();
	if (first_half == false) 
	{
		h = (h + 1) % 24;
		
		//we moved the hour forward, so force a refresh of the text
		if(me->id == LAYER_HOUR) me->flag = 0;
	}
	
	#ifdef DEBUG
		//if we're debugging, always refresh the hour layer (since we could be counting up or down)
		if(me->id == LAYER_HOUR) me->flag = 0;
	#endif

	int twelve_hour = h % 12;
	
	bool has_changed = true;
	bool is_holiday = false;
	bool include_holiday = get_include_holiday_value();
	
	if (me->id == LAYER_MINUTE)
	{
		if (m == 0) 
		{
			if(include_holiday == true)
				is_holiday = get_holiday_text(me->id, me->text);
			
			if(is_holiday == false)
				snprintf(me->text, BUFFER_SIZE, "%s", hour_text[twelve_hour]);
		}
		else if (first_half == true) snprintf(me->text, BUFFER_SIZE, "%d minuto", m);
		else snprintf(me->text, BUFFER_SIZE, "%d minuto", 60 - m);
	}
	else if(me->id == LAYER_MODIFIER)
	{
		if(m == 0 && me->flag != 1)
		{
			if(include_holiday == true)
				is_holiday = get_holiday_text(me->id, me->text);
			
			if(is_holiday == false)
				snprintf(me->text, BUFFER_SIZE, "%s", "na ng");
			
			me->flag = 1;
		}
		else if (m != 0 && first_half == true && me->flag != 2)
		{
			snprintf(me->text, BUFFER_SIZE, "makalipas ang");
			me->flag = 2;
		}
		else if (m != 0 && first_half == false && me->flag != 3)
		{
			snprintf(me->text, BUFFER_SIZE, "bago mag");
			me->flag = 3;
		}
		else
		{
			has_changed = false;
		}
	}
	else if(me->id == LAYER_HOUR)
	{
		//"minute == 0" text only happens once per hour
		//and since the previous minute (minute == 59) is still under the same condition as the current hour,
		//we need to reset the flag to force a refresh on the text
		//this will then retrieve the "minute != 0" text	
		if(m == 0) me->flag = 0;
		
		if(h == 0 && me->flag != 1) 
		{
			if(m == 0) 
			{
				if(include_holiday == true)
					is_holiday = get_holiday_text(me->id, me->text);
			
				if(is_holiday == false)
					snprintf(me->text, BUFFER_SIZE, "%s", "hating gabi");
			}
			else
			{
				snprintf(me->text, BUFFER_SIZE, "%s ng hating gabi", hour_text[twelve_hour]);
			}
			
			me->flag = 1;
		}
		else if(h > 0 && h < 6 && me->flag != 2) 
		{
			if(m == 0) 
			{
				if(include_holiday == true)
					is_holiday = get_holiday_text(me->id, me->text);
			
				if(is_holiday == false)
					snprintf(me->text, BUFFER_SIZE, "%s", "madaling araw");
			}
			else
			{
				snprintf(me->text, BUFFER_SIZE, "%s ng madaling araw", hour_text[twelve_hour]);
			}
			
			me->flag = 2;
		}
		else if(h >= 6 && h < 12 && me->flag != 3)
		{
			if(m == 0) 
			{
				if(include_holiday == true)
					is_holiday = get_holiday_text(me->id, me->text);
			
				if(is_holiday == false)
					snprintf(me->text, BUFFER_SIZE, "%s", "umaga");
				else if(h == 9)
					vibes_double_pulse();
			}
			else 
			{
				snprintf(me->text, BUFFER_SIZE, "%s ng umaga", hour_text[twelve_hour]);
			}
			
			me->flag = 3;
		}
		else if(h == 12 && me->flag != 4)
		{
			if(m == 0) 
			{
				if(include_holiday == true)
					is_holiday = get_holiday_text(me->id, me->text);
			
				if(is_holiday == false)
					snprintf(me->text, BUFFER_SIZE, "%s", "tanghali");
				else
					vibes_double_pulse();
			}
			else snprintf(me->text, BUFFER_SIZE, "%s ng tanghali", hour_text[twelve_hour]);
			
			me->flag = 4;
		}
		else if(h > 12 && h < 18 && me->flag != 5) 
		{
			if(m == 0) 
			{
				if(include_holiday == true)
					is_holiday = get_holiday_text(me->id, me->text);
			
				if(is_holiday == false)
					snprintf(me->text, BUFFER_SIZE, "%s", "hapon");
				else if(h == 15)
					vibes_double_pulse();
			}
			else
			{
				snprintf(me->text, BUFFER_SIZE, "%s ng hapon", hour_text[twelve_hour]);
			}
			
			me->flag = 5;
		}
		else if(h >= 18 && h < 24 && me->flag != 6)
		{
			if(m == 0)
			{
				if(include_holiday == true)
					is_holiday = get_holiday_text(me->id, me->text);
			
				if(is_holiday == false)
					snprintf(me->text, BUFFER_SIZE, "%s", "gabi");
				else if(h == 18)
					vibes_double_pulse();
			}
			else
			{
				snprintf(me->text, BUFFER_SIZE, "%s ng gabi", hour_text[twelve_hour]);
			}
			
			me->flag = 6;
		}
		else
		{
			has_changed = false;
		}
		
		//also, the succeeding minute (minute == 1) is still under the same condition as minute == 0
		//so we need to reset the flag to force a refresh on the text
		//this will then retrieve the "minute != 0" text	
		if(m == 0) me->flag = 0;
	}
	else
	{
		snprintf(me->text, BUFFER_SIZE, "%s", "May problema");
	}
	
	return has_changed;
}

static void layer_update(layer_info *me)
{
	if(get_invert_screen_value() == true)
	{
		text_layer_set_background_color(me->layer, GColorBlack);
		text_layer_set_text_color(me->layer, GColorWhite);
	}
	else
	{
		text_layer_set_background_color(me->layer, GColorWhite);
		text_layer_set_text_color(me->layer, GColorBlack);
	}
	
	if(get_dynamic_font_size_value() == true)
	{
		//start with the largest font
		//try to calculate the resulting size of the layer given the text and the font
		//if the size is greater than the bounds of the layer,
		//    go to the next largest font
		//    this is the reason why the fonts are ordered in a descending manner on the font array
		me->font_size = 2;
			
		//GSize content_size = 
		//graphics_text_layout_get_max_used_size
		//(
		//	ctx, 
		//	parent->text, 
		//	fonts[parent->font_size], 
		//	GRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), 
		//	GTextOverflowModeWordWrap, 
		//	GTextAlignmentCenter, 
		//	NULL
		//);
	
		//while(content_size.w > width || content_size.h > height)
		//{
		//	parent->font_size++;
		//	content_size = 
		//		graphics_text_layout_get_max_used_size
		//		(
		//			ctx, 
		//			parent->text, 
		//			fonts[parent->font_size], 
		//			GRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), 
		//			GTextOverflowModeWordWrap, 
		//			GTextAlignmentCenter, 
		//			NULL
		//		);
		//}
	}
	else
	{
		//show the second to the smallest font
		me->font_size = 2;

		APP_LOG(APP_LOG_LEVEL_DEBUG, "######### layer_update: done!!!!!!");
	}
	
	text_layer_set_text(me->layer, me->text);
	text_layer_set_font(me->layer, fonts[me->font_size]);
	text_layer_set_overflow_mode(me->layer, GTextOverflowModeWordWrap);
	text_layer_set_text_alignment(me->layer, GTextAlignmentCenter);
}

static void show_splash()
{
	if (animation.is_animating == true) return;
	
	for (int i = 0; i < LAYER_COUNT; i++) 
	{
		layers[i].text = malloc(BUFFER_SIZE);
		snprintf(layers[i].text, BUFFER_SIZE, "%s", splash_text[i]);
		layer_update(&layers[i]);
	}

	is_splash_showing = true;
	animation.current_flag = false;
	animation.index = 0;
	animation.is_animating = true;
	timer = app_timer_register(SPLASH_DELAY, handle_timer, NULL);
}

static void blink_screen()
{
	if (get_enable_blink_value() == false) return;
	if (animation.is_animating == true) return;
	
	animation.current_flag = false;
	animation.index = 0;
	animation.is_animating = true;
	timer = app_timer_register(animation.duration, handle_timer, NULL);
}

static void get_time_value(struct tm *local)
{
	(void) local;
	
	#ifndef DEBUG
		//time_t now;
		time_t temp;
		time(&temp);
		local = localtime(&temp);
	#else
		local.tm_year = 113;
		local.tm_mon = DEC;
		local.tm_mday = 30;
		local.tm_min = 59;
		local.tm_hour = 8;
	#endif
}

//for each tick of the timer, walk through the flag list of the animation class
//a false flag will show the original layers, while a true flag will show the blank layer
//do not do anything if the flag has not changed from the last iteration
//at the end of the flag list, stop the animation
//    if the animation is for the splash screen,
//        force a redraw of the the main screen since the minute tick may still be a while
static void handle_timer(void *data)
{
	if(animation.index >= (int) sizeof(animation.flags)) 
	{
		if(animation.current_flag == true) 
		{
			for (int i = 0; i < LAYER_COUNT; i++) 
			{
				layer_set_hidden(text_layer_get_layer(layers[i].layer), false);
			}
		}
		
		animation.is_animating = false;
		app_timer_cancel(timer);
		
		if(is_splash_showing == true) is_splash_showing = false;
		
		//blink animation has just ended
		//so draw the main watch face even if the minute did not tick yet
		//this is to clear the previous screen and show the time immediately
		get_time_value(now);
		
		for (int i = 0; i < LAYER_COUNT; i++) 
		{
			bool has_changed = check_text(&layers[i]);
			if (has_changed == true) 
			{
				if (animation.is_animating == false) layer_update(&layers[i]);
			}
		}
		
		return;
	}
	
	if(animation.current_flag != animation.flags[animation.index])
	{
		animation.current_flag = animation.flags[animation.index];
		
		for (int i = 0; i < LAYER_COUNT; i++) 
		{
			layer_set_hidden(text_layer_get_layer(layers[i].layer), animation.current_flag);
		}
	}
	
	animation.index++;
	timer = app_timer_register(animation.duration, handle_timer, NULL);
}

static void manual_handle_tick()
{
	blink_screen();
}

static void handle_tick(struct tm *tick_time, TimeUnits units_changed) 
{
	if(is_splash_showing == true) return;
			
	#ifndef DEBUG
		now = tick_time;
		manual_handle_tick();
	#endif
}

#ifdef DEBUG
	
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

static void window_unload(Window *window) 
{
	thincfg_deinit();
	btmonitor_deinit();

	tick_timer_service_unsubscribe();
	app_timer_cancel(timer);
	free(timer);	
	
	for (int x = 0; x < FONT_COUNT; x++)
	{
		fonts_unload_custom_font(fonts[x]);
	}
	
	for (int i = 0; i < LAYER_COUNT; i++)
	{
		text_layer_destroy(layers[i].layer);
	}
}

static void window_load(Window *window) 
{
	tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
	
	for (int x = 0; x < FONT_COUNT; x++)
	{
		if (x == 0) fonts[x] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUTIEPATOOTIE_40));
		else if (x == 1) fonts[x] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUTIEPATOOTIE_30));
		else if (x == 2) fonts[x] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUTIEPATOOTIE_25));
		else if (x == 3) fonts[x] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUTIEPATOOTIE_20));
		else fonts[x] = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CUTIEPATOOTIE_40));
	}

	thincfg_init();
	btmonitor_init(get_bt_notification_value());
	
	time_t temp;
	time(&temp);
	now = localtime(&temp);
	current_day = 0;
	is_holiday = false;

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
		
		layers[i].layer = text_layer_create(frame);
		layer_add_child(window_get_root_layer(window), text_layer_get_layer(layers[i].layer));
		
		layers[i].flag = 0;		
	}
	
	show_splash();
}

static void handle_init()
{
	window = window_create();
	window_set_window_handlers(window, 
							   (WindowHandlers)
							   {
								   .load = window_load,
								   .unload = window_unload,
							   });
	window_set_background_color(window, GColorBlack);
	
	#ifdef DEBUG
		window_set_fullscreen(window, true);
		//window_set_click_config_provider(window, (ClickConfigProvider) config_provider);
	#endif
		
	window_stack_push(window, true);
}

static void handle_deinit()
{
	window_destroy(window);
}

int main(void) 
{
	handle_init();
	app_event_loop();
	handle_deinit();
}


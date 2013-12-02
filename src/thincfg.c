#include "thincfg.h"
#include "options.h"

static bool include_holiday;
static bool enable_blink;
static bool dynamic_font_size;
static bool bt_notification;
static bool invert_screen;
static int count_up_cutover;

bool get_include_holiday_value(void) { return invert_screen; }
bool get_enable_blink_value(void) { return enable_blink; }
bool get_dynamic_font_size_value(void) { return dynamic_font_size; }
bool get_bt_notification_value(void) { return bt_notification; }
bool get_invert_screen_value(void) { return invert_screen; }
int get_count_up_cutover_value(void) { return count_up_cutover; }

static void read_config() 
{
	if (persist_exists(CONFIG_KEY_INCLUDE_HOLIDAY)) 
	{
		include_holiday = persist_read_bool(CONFIG_KEY_INCLUDE_HOLIDAY);

		#ifdef ENABLE_LOGGING
		if(include_holiday == true) APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: include_holiday=true");
		else APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: include_holiday=false");
		#endif
	}
	else
	{
		include_holiday = DEFAULT_ENABLE_BLINK; //default value

		#ifdef ENABLE_LOGGING
		if(DEFAULT_INCLUDE_HOLIDAY == true) APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: include_holiday not configured. default=true");
		else APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: include_holiday not configured. default=false");
		#endif
	}
	
	if (persist_exists(CONFIG_KEY_ENABLE_BLINK)) 
	{
		enable_blink = persist_read_bool(CONFIG_KEY_ENABLE_BLINK);

		#ifdef ENABLE_LOGGING
		if(enable_blink == true) APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: enable_blink=true");
		else APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: enable_blink=false");
		#endif
	}
	else
	{
		enable_blink = DEFAULT_ENABLE_BLINK; //default value

		#ifdef ENABLE_LOGGING
		if(DEFAULT_ENABLE_BLINK == true) APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: enable_blink not configured. default=true");
		else APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: enable_blink not configured. default=false");
		#endif
	}
	
	if (persist_exists(CONFIG_KEY_DYNAMIC_FONT_SIZE)) 
	{
		dynamic_font_size = persist_read_bool(CONFIG_KEY_DYNAMIC_FONT_SIZE);

		#ifdef ENABLE_LOGGING
		if(dynamic_font_size == true) APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: dynamic_font_size=true");
		else APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: dynamic_font_size=false");
		#endif
	}
	else
	{
		dynamic_font_size = DEFAULT_DYNAMIC_FONT_SIZE; //default value

		#ifdef ENABLE_LOGGING
		if(DEFAULT_DYNAMIC_FONT_SIZE == true) APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: dynamic_font_size not configured. default=true");
		else APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: dynamic_font_size not configured. default=false");
		#endif
	}
	
	if (persist_exists(CONFIG_KEY_BT_NOTIFICATION)) 
	{
		bt_notification = persist_read_bool(CONFIG_KEY_BT_NOTIFICATION);

		#ifdef ENABLE_LOGGING
		if(bt_notification == true) APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: bt_notification=true");
		else APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: bt_notification=false");
		#endif
	}
	else
	{
		bt_notification = DEFAULT_BT_NOTIFICATION; //default value

		#ifdef ENABLE_LOGGING
		if(DEFAULT_BT_NOTIFICATION == true) APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: bt_notification not configured. default=true");
		else APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: bt_notification not configured. default=false");
		#endif
	}
	
	if (persist_exists(CONFIG_KEY_INVERT_SCREEN)) 
	{
		invert_screen = persist_read_bool(CONFIG_KEY_INVERT_SCREEN);

		#ifdef ENABLE_LOGGING
		if(invert_screen == true) APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: invert_screen=true");
		else APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: invert_screen=false");
		#endif
	}
	else
	{
		invert_screen = DEFAULT_INVERT; //default value

		#ifdef ENABLE_LOGGING
		if(DEFAULT_INVERT == true) APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: invert_screen not configured. default=true");
		else APP_LOG(APP_LOG_LEVEL_DEBUG, "read_config: invert_screen not configured. default=false");
		#endif
	}
	
	if (persist_exists(CONFIG_KEY_COUNT_UP_CUTOVER)) 
	{
		count_up_cutover = persist_read_int(CONFIG_KEY_COUNT_UP_CUTOVER);

		#ifdef ENABLE_LOGGING
		char *output = "read_config: count_up_cutover=XXX";
		snprintf(output, strlen(output), "read_config: count_up_cutover=%d", count_up_cutover);
		APP_LOG(APP_LOG_LEVEL_DEBUG, output);
		#endif
	}
	else
	{
		count_up_cutover = DEFAULT_COUNT_UP_CUTOVER; //default value

		#ifdef ENABLE_LOGGING
		char *warning = "read_config: count_up_cutover not configured. default=XXX";
		snprintf(warning, strlen(warning), "read_config: count_up_cutover not configured. default=%d", DEFAULT_COUNT_UP_CUTOVER);
		APP_LOG(APP_LOG_LEVEL_DEBUG, warning);
		#endif
	}
}

static void in_dropped_handler(AppMessageResult reason, void *context) 
{
	#ifdef ENABLE_LOGGING
	APP_LOG(APP_LOG_LEVEL_DEBUG, "in_dropped_handler: done");
	#endif
}

static void in_received_handler(DictionaryIterator *received, void *context) 
{
	Tuple *holiday = dict_find(received, CONFIG_KEY_INCLUDE_HOLIDAY);
	if(holiday) 
	{
		persist_write_bool(CONFIG_KEY_INCLUDE_HOLIDAY, holiday->value->int32 == 1);
		include_holiday = (holiday->value->int32 == 1);

		#ifdef ENABLE_LOGGING
		if(include_holiday == true) APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler: include_holiday=true");
		else APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler: include_holiday=false");
		#endif
	}
	else
	{
		//since thinCFG won't pass fields that are not selected, we set the bt_notification to false if its key is not returned
		persist_write_bool(CONFIG_KEY_INCLUDE_HOLIDAY, false);
		include_holiday = false;

		#ifdef ENABLE_LOGGING
		APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler: include_holiday=false");
		#endif
	}
	
	Tuple *blink = dict_find(received, CONFIG_KEY_ENABLE_BLINK);
	if(blink) 
	{
		persist_write_bool(CONFIG_KEY_ENABLE_BLINK, blink->value->int32 == 1);
		enable_blink = (blink->value->int32 == 1);

		#ifdef ENABLE_LOGGING
		if(enable_blink == true) APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler: enable_blink=true");
		else APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler: enable_blink=false");
		#endif
	}
	else
	{
		//since thinCFG won't pass fields that are not selected, we set the bt_notification to false if its key is not returned
		persist_write_bool(CONFIG_KEY_ENABLE_BLINK, false);
		enable_blink = false;

		#ifdef ENABLE_LOGGING
		APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler: enable_blink=false");
		#endif
	}
	
	Tuple *font = dict_find(received, CONFIG_KEY_DYNAMIC_FONT_SIZE);
	if(font) 
	{
		persist_write_bool(CONFIG_KEY_DYNAMIC_FONT_SIZE, font->value->int32 == 1);
		dynamic_font_size = (font->value->int32 == 1);

		#ifdef ENABLE_LOGGING
		if(dynamic_font_size == true) APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler: dynamic_font_size=true");
		else APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler: dynamic_font_size=false");
		#endif
	}
	else
	{
		//since thinCFG won't pass fields that are not selected, we set the bt_notification to false if its key is not returned
		persist_write_bool(CONFIG_KEY_DYNAMIC_FONT_SIZE, false);
		dynamic_font_size = false;

		#ifdef ENABLE_LOGGING
		APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler: dynamic_font_size=false");
		#endif
	}
	
	Tuple *bt = dict_find(received, CONFIG_KEY_BT_NOTIFICATION);
	if(bt) 
	{
		persist_write_bool(CONFIG_KEY_BT_NOTIFICATION, bt->value->int32 == 1);
		bt_notification = (bt->value->int32 == 1);

		#ifdef ENABLE_LOGGING
		if(bt_notification == true) APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler: bt_notification=true");
		else APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler: bt_notification=false");
		#endif
	}
	else
	{
		//since thinCFG won't pass fields that are not selected, we set the bt_notification to false if its key is not returned
		persist_write_bool(CONFIG_KEY_BT_NOTIFICATION, false);
		bt_notification = false;

		#ifdef ENABLE_LOGGING
		APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler: bt_notification=false");
		#endif
	}
	
	Tuple *invert = dict_find(received, CONFIG_KEY_INVERT_SCREEN);
	if(invert) 
	{
		persist_write_bool(CONFIG_KEY_INVERT_SCREEN, invert->value->int32 == 1);
		invert_screen = (invert->value->int32 == 1);

		#ifdef ENABLE_LOGGING
		if(invert_screen == true) APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler: invert_screen=true");
		else APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler: invert_screen=false");
		#endif
	}
	else
	{
		//since thinCFG won't pass fields that are not selected, we set the bt_notification to false if its key is not returned
		persist_write_bool(CONFIG_KEY_INVERT_SCREEN, false);
		invert_screen = false;

		#ifdef ENABLE_LOGGING
		APP_LOG(APP_LOG_LEVEL_DEBUG, "in_received_handler: invert_screen=false");
		#endif
	}
	
	Tuple *cutover = dict_find(received, CONFIG_KEY_COUNT_UP_CUTOVER);
	if(cutover) 
	{
		persist_write_int(CONFIG_KEY_COUNT_UP_CUTOVER, cutover->value->int32);
		count_up_cutover = (int)cutover->value->int32;
	
		#ifdef ENABLE_LOGGING
		char *output = "in_received_handler: count_up_cutover=XXX";
		snprintf(output, strlen(output), "in_received_handler: count_up_cutover=%d", (int)cutover->value->int32);
		APP_LOG(APP_LOG_LEVEL_DEBUG, output);
		#endif
	}
}

static void app_message_init(void) 
{
	app_message_register_inbox_received(in_received_handler);
	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_open(64, 64);
}

void thincfg_init() 
{
    app_message_init();
	read_config();
}

void thincfg_deinit()
{
	app_message_deregister_callbacks();
}

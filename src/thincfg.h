#include "pebble.h"

#ifndef THINCFG
#define THINCFG

#define DEFAULT_ENABLE_BLINK true
#define DEFAULT_DYNAMIC_FONT_SIZE true
#define DEFAULT_INCLUDE_HOLIDAY true
#define DEFAULT_INVERT false
#define DEFAULT_BT_NOTIFICATION false
#define DEFAULT_COUNT_UP_CUTOVER 40
	
enum 
{
	CONFIG_KEY_INCLUDE_HOLIDAY = 0,
	CONFIG_KEY_ENABLE_BLINK = 1,
	CONFIG_KEY_DYNAMIC_FONT_SIZE = 2,
	CONFIG_KEY_BT_NOTIFICATION = 3,
	CONFIG_KEY_INVERT_SCREEN = 4,
	CONFIG_KEY_COUNT_UP_CUTOVER = 5,
	CONFIG_KEY_ACTION = 6
};

bool get_include_holiday_value(void);
bool get_enable_blink_value(void);
bool get_dynamic_font_size_value(void);
bool get_bt_notification_value(void);
bool get_invert_screen_value(void);
int get_count_up_cutover_value(void);

void thincfg_init(void);
void thincfg_deinit(void);
	
#endif

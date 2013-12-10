#include "pebble.h"

#ifndef THINCFG
#define THINCFG

#define DEFAULT_ENABLE_BLINK true
#define DEFAULT_DYNAMIC_FONT_SIZE false
#define DEFAULT_INCLUDE_HOLIDAY true
#define DEFAULT_INVERT true
#define DEFAULT_BT_NOTIFICATION true
#define DEFAULT_COUNT_UP_CUTOVER 40
	
enum 
{
	CONFIG_KEY_INCLUDE_HOLIDAY = 29020,
	CONFIG_KEY_ENABLE_BLINK = 29021,
	CONFIG_KEY_DYNAMIC_FONT_SIZE = 29022,
	CONFIG_KEY_BT_NOTIFICATION = 29023,
	CONFIG_KEY_INVERT_SCREEN = 29024,
	CONFIG_KEY_COUNT_UP_CUTOVER = 29025,
	CONFIG_KEY_ACTION = 29026
};

typedef void(*ThinCFGFieldChangedHandler)(const uint32_t key, const void *old_value, const void *new_value);

typedef struct {
        ThinCFGFieldChangedHandler field_changed;
} ThinCFGCallbacks;

bool get_include_holiday_value(void);
bool get_enable_blink_value(void);
bool get_dynamic_font_size_value(void);
bool get_bt_notification_value(void);
bool get_invert_screen_value(void);
int get_count_up_cutover_value(void);

void thincfg_init(void);
void thincfg_deinit(void);
void thincfg_subscribe(ThinCFGCallbacks callback);
	
#endif

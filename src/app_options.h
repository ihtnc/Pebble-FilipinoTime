//#define DEBUG
#define PHONE_HAS_HTTPPEBBLE
//#define ANDROID

#define ENABLE_BLINK true
#define DYNAMIC_FONT_SIZE true
#define INCLUDE_HOLIDAY true
#define INVERT false
#define SPLASH_DELAY 2000
#define COUNT_UP_CUTOVER 40
	
//Frequency is in minutes
#define PING_FREQUENCY 5
	
#if INVERT == true
	#define BACKCOLOR GColorWhite
	#define FORECOLOR GColorBlack
#else
	#define BACKCOLOR GColorBlack
	#define FORECOLOR GColorWhite
#endif
	
#define SCREEN_HEIGHT 168
#define SCREEN_WIDTH 144
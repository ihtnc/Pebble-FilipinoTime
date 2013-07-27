#include "app_options.h"

#define APP_VER_MAJOR 1	
#define APP_VER_MINOR 6
	
#ifndef DEBUG
	#ifdef PHONE_HAS_HTTPPEBBLE
		#ifdef ANDROID
			#define APP_NAME "Filipino Time [A]"
		#else
			#define APP_NAME "Filipino Time [I]"
		#endif
	#else
		#define APP_NAME "Filipino Time"
	#endif
#else
	#ifdef PHONE_HAS_HTTPPEBBLE
		#ifdef ANDROID
			#define APP_NAME "Debug: FilipinoTime [A]"
		#else
			#define APP_NAME "Debug: FilipinoTime [I]"
		#endif
	#else
		#define APP_NAME "Debug: FilipinoTime"
	#endif
#endif
	
#define APP_AUTHOR "ihopethisnamecounts"
#include "app_options.h"

#define APP_VER_MAJOR 1	
#define APP_VER_MINOR 7
	
#ifndef DEBUG
	#ifdef PHONE_HAS_HTTPPEBBLE
		#ifdef ANDROID
			#if INVERT == true
				#define APP_NAME "Filipino Time [AV]"
			#else
				#define APP_NAME "Filipino Time [A]"
			#endif
		#else
			#if INVERT == true
				#define APP_NAME "Filipino Time [IV]"
			#else
				#define APP_NAME "Filipino Time [I]"
			#endif
		#endif
	#else
		#if INVERT == true
			#define APP_NAME "Filipino Time [V]"
		#else
			#define APP_NAME "Filipino Time"
		#endif
	#endif
#else
	#ifdef PHONE_HAS_HTTPPEBBLE
		#ifdef ANDROID
			#if INVERT == true
				#define APP_NAME "Debug: Filipino Time [AV]"
			#else
				#define APP_NAME "Debug: Filipino Time [A]"
			#endif
		#else
			#if INVERT == true
				#define APP_NAME "Debug: Filipino Time [IV]"
			#else
				#define APP_NAME "Debug: Filipino Time [I]"
			#endif
		#endif
	#else
		#if INVERT == true
			#define APP_NAME "Filipino Time [V]"
		#else
			#define APP_NAME "Filipino Time"
		#endif
	#endif
#endif
	
#define APP_AUTHOR "ihopethisnamecounts"
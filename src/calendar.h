#include "pebble.h"

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
	
bool isLeapYear(int year);
int numDaysInMonth(const int month, const int year) ;
void add_minutes(struct tm *now, const int minutes);
void add_hours(struct tm *now, const int hours);
void add_days(struct tm *now, const int days);

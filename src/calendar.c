#include "pebble.h"
#include "calendar.h"

bool isLeapYear(int year) 
{
	year = year + 1900;
	return (((year%4 == 0) && (year%100 != 0)) || (year%400 == 0));
}
	
int numDaysInMonth(const int month, const int year) 
{
	static const int nDays[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	
	return nDays[month] + (month == FEB)*isLeapYear(year);
}

void add_minutes(struct tm *now, const int minutes)
{
	int value = (now->tm_min + minutes);
	now->tm_min = (60 + value) % 60;
 	value = value / 60 - (value < 0 && value % 60 != 0 ? 1 : 0); //because -value/60 seems to rounds down to -inf instead of 0
	
	value = (now->tm_hour + value);
	now->tm_hour = (24 + value) % 24;
	value = value / 24 - (value < 0 && value % 24 != 0 ? 1 : 0); //because -value/24 seems to  rounds down to -inf instead of 0
	
	now->tm_mday = now->tm_mday + value;
	int daysInMonth = numDaysInMonth(now->tm_mon, now->tm_year);
	if(now->tm_mday > daysInMonth)
	{
		do
		{
			now->tm_mday = now->tm_mday - daysInMonth;
			
			if(now->tm_mon == DEC) 
			{
				now->tm_mon = JAN;
				now->tm_year = now->tm_year + 1;
			}
			else 
			{
				now->tm_mon = now->tm_mon + 1;
			}
			
			daysInMonth = numDaysInMonth(now->tm_mon, now->tm_year);	
		}
		while(now->tm_mday > daysInMonth);			
	}
	else if(now->tm_mday < 0)
	{
		do
		{
			if(now->tm_mon == JAN)
			{
				now->tm_mon = DEC;
				now->tm_year = now->tm_year - 1;
			}
			else
			{
				now->tm_mon = now->tm_mon - 1;
			}
			
			daysInMonth = numDaysInMonth(now->tm_mon, now->tm_year);
			now->tm_mday = now->tm_mday + daysInMonth;
		}
		while(now->tm_mday < 0);	
	}
}

void add_hours(struct tm *now, const int hours)
{
	add_minutes(now, hours * 60);
}
void add_days(struct tm *now, const int days)
{
	add_hours(now, days * 24);
}

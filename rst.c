#include <time.h>
#include "rst.h"

time_t (*o_mktime) (struct tm * tp) = &mktime;
struct tm * (*o_localtime_r) (const time_t * t, struct tm * tp) = &localtime_r;
struct tm * (*o_localtime) (const time_t * t) = &localtime;

void make_rst (struct tm * tp)
{
	if(tp->tm_hour < 6) {
		int hour = tp->tm_hour, min = tp->tm_min, sec = tp->tm_sec,
		    isdst = tp->tm_isdst;
		tp->tm_hour = -12;
		o_mktime(tp);
		tp->tm_hour = hour + 24;
		tp->tm_min = min;
		tp->tm_sec = sec;
		tp->tm_isdst = isdst;
	}
}

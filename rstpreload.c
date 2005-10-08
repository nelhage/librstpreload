#define _GNU_SOURCE
#include <time.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

time_t my_mktime (struct tm * tp);
struct tm * my_gmtime_r (const time_t * t, struct tm * tp);
struct tm * my_gmtime (const time_t * t);
struct tm * my_localtime_r (const time_t * t, struct tm * tp);
struct tm * my_localtime (const time_t * t);

time_t (*o_mktime) (struct tm * tp);
struct tm * (*o_gmtime_r) (const time_t * t, struct tm * tp);
struct tm * (*o_gmtime) (const time_t * t);
struct tm * (*o_localtime_r) (const time_t * t, struct tm * tp);
struct tm * (*o_localtime) (const time_t * t);

void make_rst (struct tm * tp)
{
	if(tp->tm_hour < 6) {
		tp->tm_mday--;
		o_mktime(tp);
		tp->tm_hour += 24;
	}
}

time_t my_mktime (struct tm * tp)
{
	time_t ret = o_mktime (tp);
	make_rst(tp);
	return ret;
}

struct tm * my_gmtime_r (const time_t * t, struct tm * tp)
{		    
	return o_gmtime_r (t, tp);
}		    
		    
struct tm * my_gmtime (const time_t * t)
{		    
	return o_gmtime (t);
}		    
		    
struct tm * my_localtime_r (const time_t * t, struct tm * tp)
{
	if(t == NULL || tp == NULL)
		return o_localtime_r (t, tp);
	o_localtime_r(t, tp);
	make_rst(tp);
	
	return tp;
}		    
		    
struct tm * my_localtime (const time_t * t)
{
	if(t == NULL)
		return o_localtime(t);

	struct tm * ret = o_localtime (t);
	make_rst(ret);
	
	return ret;
}


void _init()
{
	o_mktime = dlsym(RTLD_NEXT, "mktime");
	if(!o_mktime) {
		fprintf(stderr, "rstpreload: dlsym: %s\n", dlerror());
		exit(2);
	}
	o_gmtime_r = dlsym(RTLD_NEXT, "gmtime_r");
	if(!o_gmtime_r) {
		fprintf(stderr, "rstpreload: dlsym: %s\n", dlerror());
		exit(2);		
	}

	o_gmtime = dlsym(RTLD_NEXT, "gmtime");
	if(!o_gmtime) {
		fprintf(stderr, "rstpreload: dlsym: %s\n", dlerror());
		exit(2);		
	}

	o_localtime_r = dlsym(RTLD_NEXT, "localtime_r");
	if(!o_localtime_r) {
		fprintf(stderr, "rstpreload: dlsym: %s\n", dlerror());
		exit(2);
	}

	o_localtime = dlsym(RTLD_NEXT, "localtime");
	if(!o_localtime) {
		fprintf(stderr, "rstpreload: dlsym: %s\n", dlerror());
		exit(2);
	}
}

__asm(".symver my_mktime,mktime@@GLIBC_2.0");
__asm(".symver my_gmtime_r,gmtime_r@@GLIBC_2.0");
__asm(".symver my_gmtime,gmtime@@GLIBC_2.0");
__asm(".symver my_localtime_r,localtime_r@@GLIBC_2.0");
__asm(".symver my_localtime,localtime@@GLIBC_2.0");

#define _GNU_SOURCE
#include <time.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "rst.h"

time_t my_mktime (struct tm * tp);
struct tm * my_localtime_r (const time_t * t, struct tm * tp);
struct tm * my_localtime (const time_t * t);
char * my_ctime_r (const time_t * t, char * buf);
char * my_ctime (const time_t * t);

time_t my_mktime (struct tm * tp)
{
	time_t ret = o_mktime (tp);
	make_rst(tp);
	return ret;
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

char * my_ctime_r (const time_t * t, char * buf)
{
	struct tm tm;
	return asctime_r (my_localtime_r (t, &tm), buf);
}

char * my_ctime (const time_t * t)
{
	return asctime (my_localtime (t));
}


void _init()
{
	o_mktime = dlsym(RTLD_NEXT, "mktime");
	if(!o_mktime) {
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
__asm(".symver my_localtime_r,localtime_r@@GLIBC_2.0");
__asm(".symver my_localtime,localtime@@GLIBC_2.0");
__asm(".symver my_ctime_r,ctime_r@@GLIBC_2.0");
__asm(".symver my_ctime,ctime@@GLIBC_2.0");

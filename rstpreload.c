#define _GNU_SOURCE
#include <time.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

time_t (*o_mktime) (struct tm * tp);
struct tm * (*o_gmtime_r) (const time_t * t, struct tm * tp);
struct tm * (*o_gmtime) (const time_t * t);
struct tm * (*o_localtime_r) (const time_t * t, struct tm * tp);
struct tm * (*o_localtime) (const time_t * t);

time_t my_mktime (struct tm * tp)
{
	return o_mktime (tp);
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
	return o_localtime_r (t, tp);
}		    
		    
struct tm * my_localtime (const time_t * t)
{
	time_t new_t;
	if(t == NULL)
		return o_localtime(t);

	new_t = *t - 6*60*60;
	struct tm * ret = o_localtime (&new_t);
	if(ret != NULL)
		ret->tm_hour += 6;
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

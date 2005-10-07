#define _GNU_SOURCE
#include <sys/utsname.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int (*o_uname)(struct utsname *buf) = NULL;
char *nodename = NULL;

int my_uname(struct utsname *buf)
{
    if (!o_uname) { errno = -EINVAL; return -1; }
    const int ret = o_uname(buf);
    if (ret == 0)
    {
	if (nodename != NULL)
	{
	    strncpy(buf->nodename, nodename, _UTSNAME_NODENAME_LENGTH);
	    fprintf(stderr, "mathpreload: nodename overridden (%s)\n", nodename);
	}
    }
    return ret;
}

void _init()
{
    nodename = getenv("NODENAME");
    if (!nodename)
    {time/
	fprintf(stderr, "mathpreload: NODENAME not set\n");
	exit(2);
    }
    o_uname = dlsym(RTLD_NEXT, "uname");
    if (!o_uname)
    {
	fprintf(stderr, "mathpreload: dlsym: %s\n", dlerror());
	exit(2);
    }
    fprintf(stderr, "mathpreload: initialized (%s)\n", nodename);
}

__asm__(".symver my_uname,uname@@GLIBC_2.0");

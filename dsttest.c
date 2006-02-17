#include <time.h>
#include <stdio.h>
#include "rst.h"

int main()
{
	struct tm tt;
	char normal[127], rst[127];
	time_t t;
	for (t = 1143867600; t <= 1144123200; t += 3600)
	{
        	localtime_r(&t, &tt);
		strftime(normal, 127, "%F %T", &tt);
		make_rst(&tt);
		strftime(rst, 127, "%F %T", &tt);
		printf("%s | %s\n", normal, rst);
	}

	for (t = 1162008000; t <= 1162270800; t += 3600)
	{
        	localtime_r(&t, &tt);
		strftime(normal, 127, "%F %T", &tt);
		make_rst(&tt);
		strftime(rst, 127, "%F %T", &tt);
		printf("%s | %s\n", normal, rst);
	}

	return 0;
}

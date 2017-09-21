#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/time.h>
#include <time.h>

#include "../include/util.h"

uint64_t hover_timestamp_now()
{
	struct timespec val;
	clock_gettime(CLOCK_REALTIME, &val);

	return val.tv_sec * 1000 + val.tv_nsec / 1000000;
}

int hover_timestamp_tostring(char *buff, size_t maxlen, uint64_t timestamp)
{
	size_t written;
	struct tm val;

	tzset();

	time_t sec = timestamp / 1000;
	if(localtime_r(&sec, &val) == NULL)
		return -1;

	written = strftime(buff, maxlen, "%F %T", &val);
	if(written == 0)
		return -1;

	written += snprintf(buff + written, maxlen - written, ".%03"PRIu64, timestamp % 1000);
	if(written == maxlen)
		return -1;

	return written;
}

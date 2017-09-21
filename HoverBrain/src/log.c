#include <stdio.h>
#include <stdlib.h>
#include "../include/events.h"
#include "../include/util.h"

struct context
{
	FILE *fd;
	int loglevel; //TODO
};

void hover_handle_log(const hover_event_t *event, void *_ctx)
{
	struct context *ctx = _ctx;
	char buff[128];

	hover_timestamp_tostring(buff, 128, event->timestamp);

	fprintf(ctx->fd, "[%s] EVENT %4d length %4d", buff, event->kind, event->argLength);

	if(event->argLength > 0)
	{
		fprintf(ctx->fd, " | ");

		for(int i = 0; i < event->argLength; i++)
		{
			fprintf(ctx->fd, "%02hhx ", event->arg[i]);
		}
	}

	fprintf(ctx->fd, "\n");
	fflush(ctx->fd);
}

void hover_log_init(const char *file)
{
	struct context *ctx = malloc(sizeof(struct context));
	ctx->fd = fopen(file, "a");
	ctx->loglevel = 42; //TODO

	char buff[128];
	hover_timestamp_tostring(buff, 128, hover_timestamp_now());
	fprintf(ctx->fd, "[%s] RESTART\n", buff);
	fflush(ctx->fd);

	for(int i = 0; i < HOVER_NUM_EVENTS; i++)
	{
		hover_event_add_handler(i, hover_handle_log, ctx);
	}
}

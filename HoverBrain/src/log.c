#include <stdio.h>
#include <stdlib.h>
#include "../include/events.h"

struct context
{
	FILE *fd;
	int loglevel; //TODO
};

void hover_handle_log(const hover_event_t *event, void *_ctx)
{
	struct context *ctx = _ctx;
	fprintf(ctx->fd, "EVENT %4d length %4d", event->kind, event->argLength);

	if(event->argLength > 0)
	{
		fprintf(ctx->fd, " | ");

		for(int i = 0; i < event->argLength; i++)
		{
			fprintf(ctx->fd, "%hhx ", event->arg[i]);
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

	for(int i = 0; i < HOVER_NUM_EVENTS; i++)
	{
		hover_event_add_handler(i, hover_handle_log, ctx);
	}
}

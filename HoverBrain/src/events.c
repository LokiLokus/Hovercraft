#include <stdlib.h>

#include "../include/events.h"

struct handler_info
{
	hover_eventhandler_t handler;
	void *data;
	struct handler_info *next;
};

static struct handler_info *handlers[HOVER_NUM_EVENTS] = {NULL};

void hover_event_init()
{
	//nothing to do for now
}

void hover_event_add_handler(hover_event_kind_t kind, hover_eventhandler_t handler, void *data)
{
	struct handler_info *info = malloc(sizeof(struct handler_info));
	info->handler = handler;
	info->data = data;

	info->next = handlers[kind];
	handlers[kind] = info;
}

void *hover_event_remove_handler(hover_event_kind_t kind, hover_eventhandler_t handler)
{
	void *data = NULL;
	struct handler_info *prev = NULL;
	struct handler_info *curr = handlers[kind];

	while(curr != NULL)
	{
		if(curr->handler == handler)
		{
			if(prev == NULL)
				handlers[kind] = curr->next;
			else
				prev->next = curr->next;

			data = curr->data;
			break;
		}

		prev = curr;
		curr = curr->next;
	}

	return data;
}

void hover_event_emit(hover_event_t *event)
{
	struct handler_info *curr = handlers[event->kind];

	while(curr != NULL)
	{
		curr->handler(event, curr->data);
		curr = curr->next;
	}
}

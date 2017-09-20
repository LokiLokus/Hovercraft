#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

//a package looks like this:
// 4 bytes - event id (responses should set the same id)
// 4 bytes - event kind
// 4 bytes - timestamp in ms since epoch
// 4 bytes - length of the argument field
// n bytes - argument

typedef enum
{
	//control related events
	HOVER_EVENT_HOVER,
	HOVER_EVENT_FORWARD,
	HOVER_EVENT_STEERING,
	HOVER_EVENT_STOPALL,
	HOVER_EVENT_TURBO,

	//network related events
	HOVER_EVENT_PING,
	HOVER_EVENT_PONG,

	//misc events
	HOVER_EVENT_PHOTO,
	HOVER_EVENT_AUTOPILOT,

	HOVER_NUM_EVENTS
} hover_event_kind_t;

typedef struct
{
	uint32_t id;
	uint32_t kind; //hover_event_type_t
	uint32_t timestamp; //ms since 1.1.1970
	uint32_t argLength;
	uint8_t arg[]; //len = argLength
} hover_event_t;

//this macro allows us to get a regular value out of the event argument
//double arg = hover_event_argument(event, double);
//bool arg = hover_event_argument(event, bool);
//etc.
#define hover_event_argument_as(event, type) (*(type *)(event)->arg)

typedef void (*hover_eventhandler_t)(const hover_event_t *event, void *data);

void hover_event_init();

void hover_event_add_handler(hover_event_kind_t kind, hover_eventhandler_t handler, void *data);
void *hover_event_remove_handler(hover_event_kind_t kind, hover_eventhandler_t handler);

void hover_event_emit(hover_event_t *event);
void hover_event_emit_new(hover_event_kind_t kind, void *arg, size_t argLength);

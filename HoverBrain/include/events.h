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
	HOVER_EVENT_HOVER, //sets the speed of the hover motor
	HOVER_EVENT_FORWARD, //sets the speed of the manouvering motor
	HOVER_EVENT_STEERING, //sets the steering
	HOVER_EVENT_STOPALL, //stops all motors
	HOVER_EVENT_TURBO, //enable/disable the turbo
	HOVER_EVENT_AUTOPILOT, //enable/disable the autopilot

	//network related events
	HOVER_EVENT_CONNECT, //emitted when a new main client connects
	HOVER_EVENT_DISCONNECT, //emitted when the main client disconnects
	HOVER_EVENT_PING, //sent by a client to ping the sever
	HOVER_EVENT_PONG, //response to ping

	//misc events
	HOVER_EVENT_INIT, //used to tell a client if he is in control
	HOVER_EVENT_PHOTO, //used to send a photo to clients (periodically?)

	HOVER_NUM_EVENTS
} hover_event_kind_t;

typedef struct
{
	uint64_t timestamp; //ms since 1.1.1970
	uint32_t kind; //hover_event_type_t
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

#define hover_event_size(event) (sizeof(hover_event_t) + (event)->argLength)

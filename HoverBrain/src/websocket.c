#include <websock.h>
#include <stdio.h>
#include "../include/events.h"

struct context
{
	libwebsock_client_state *mainClient;
};
#define contextFromClient(client) ((struct context *)((libwebsock_context *)client->ctx)->user_data)

//this callback gets registered to all events that need to be sent to the client
static void hover_websocket_transmit_event(const hover_event_t *event, void *_ctx)
{
	struct context *ctx = _ctx;
	//TODO send HOVER_EVENT_PHOTO to all other clients too
	libwebsock_send_binary(ctx->mainClient, (void *)event, hover_event_size(event));
}

static int hover_websocket_onopen(libwebsock_client_state *state)
{
	struct context *ctx = contextFromClient(state);
	bool isMain = false;

	if(ctx->mainClient == NULL)
	{
		isMain = true;
		ctx->mainClient = state;
	}

	hover_event_emit_new(HOVER_EVENT_INIT, &isMain, sizeof(bool));
	hover_event_emit_new(HOVER_EVENT_CONNECT, NULL, 0);
}
static int hover_websocket_onclose(libwebsock_client_state *state)
{
	struct context *ctx = contextFromClient(state);
	if(ctx->mainClient == state)
	{
		ctx->mainClient = NULL;
		hover_event_emit_new(HOVER_EVENT_DISCONNECT, NULL, 0);
	}
}

static int hover_websocket_onmessage(libwebsock_client_state *state, libwebsock_message *msg)
{
	if(msg->payload_len < sizeof(hover_event_t))
		return 0; //not a hover event packet

	hover_event_t *event = (hover_event_t *)msg->payload;
	if(msg->payload_len != hover_event_size(event))
		return 0; //malformed packet

	struct context *ctx = contextFromClient(state);
	if(ctx->mainClient != state)
	{
		switch(event->kind)
		{
			case HOVER_EVENT_PING:
			case HOVER_EVENT_PHOTO:
				break;
			default:
				return 0; //don't allow control events from secondary clients
		}
	}

	hover_event_emit(event);
}

void hover_websocket_run(char *address, char *port)
{
	struct context *ctx = malloc(sizeof(struct context));
	ctx->mainClient = NULL;

	hover_event_add_handler(HOVER_EVENT_PONG, hover_websocket_transmit_event, ctx);
	hover_event_add_handler(HOVER_EVENT_INIT, hover_websocket_transmit_event, ctx);
	hover_event_add_handler(HOVER_EVENT_PHOTO, hover_websocket_transmit_event, ctx);

	libwebsock_context *wss = libwebsock_init();
	wss->onopen = hover_websocket_onopen;
	wss->onclose = hover_websocket_onclose;
	wss->onmessage = hover_websocket_onmessage;
	wss->user_data = ctx;

	libwebsock_bind(wss, address, port);
	libwebsock_wait(wss);
}

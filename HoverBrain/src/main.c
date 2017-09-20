#include "../include/events.h"

extern void hover_websocket_run(char *addr, char *port);

int main(int argc, char **argv)
{
	hover_event_init();

	//TODO init other things

	//this needs to be the last line of main as the function never returns
	hover_websocket_run("0.0.0.0", "1337");
}

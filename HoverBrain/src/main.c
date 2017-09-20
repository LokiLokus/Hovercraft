#include "../include/events.h"

void hover_log_init(const char *file);
extern void hover_websocket_run(char *addr, char *port);

int main(int argc, char **argv)
{
	hover_event_init();
	hover_log_init("hoverbrain.log");

	//TODO init other things

	//this needs to be the last line of main as the function never returns
	hover_websocket_run("0.0.0.0", "1337");
}

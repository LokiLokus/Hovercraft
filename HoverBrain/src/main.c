#include "../include/events.h"
#include <unistd.h>

void hover_log_init(const char *file);
void hover_hardware_init(uint8_t steering, uint8_t hover, uint8_t forward);
void hover_websocket_run(char *addr, char *port);

int main(int argc, char **argv)
{
	hover_event_init();
	hover_log_init("/dev/stdout");

	//TODO are the ports correct?
	hover_hardware_init(13, 12, 11);

	//TODO init other things

	//this needs to be the last line of main as the function never returns
	hover_websocket_run("0.0.0.0", "1337");
}

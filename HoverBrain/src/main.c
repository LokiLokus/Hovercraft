#include "../include/events.h"
#include "../config.h"
#include <unistd.h>

void hover_log_init(const char *file);
void hover_hardware_init(uint8_t steering, uint8_t hover, uint8_t forward);
void hover_websocket_run(char *addr, char *port);

int main(int argc, char **argv)
{
	hover_event_init();
	hover_log_init(HOVER_CONFIG_LOGFILE);

	hover_hardware_init(
		HOVER_CONFIG_PORT_STEERING,
		HOVER_CONFIG_PORT_HOVER,
		HOVER_CONFIG_PORT_FORWARD
	);

	//TODO init other things

	//this needs to be the last line of main as the function never returns
	hover_websocket_run(HOVER_CONFIG_WEBSOCKET_ADDRESS, HOVER_CONFIG_WEBSOCKET_PORT);
}

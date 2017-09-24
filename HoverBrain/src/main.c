#include "../include/events.h"
#include "../include/hw_access.h"
#include <unistd.h>

void hover_log_init(const char *file);
extern void hover_websocket_run(char *addr, char *port);

int main(int argc, char **argv)
{
	hover_event_init();
	hover_log_init("hoverbrain.log");

	hover_hw_controller hwc;
	hover_hw_controller_init(&hwc);

	hover_hwo hwo;
	hover_hwo_init(&hwo, &hwc, true, HOVER_HWO_TYPE_SERVO);

//	hover_hwo_set_duty_fraction(&hwo, 0.5);

	for(float i = 0.2; ; i += 0.1)
	{
		hover_hwo_set_duty_fraction(&hwo, i);
		sleep(1);

		if(i > 0.8)
			i = 0;
	}

	hover_hwo_fin(&hwo);
	hover_hw_controller_fin(&hwc);

	//TODO init other things

	//this needs to be the last line of main as the function never returns
	//hover_websocket_run("0.0.0.0", "1337");
}

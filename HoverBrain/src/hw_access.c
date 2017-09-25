#include "../include/types.h"
#include "../include/events.h"

#include <stdlib.h>
#include <pigpio.h>


//TODO are these values correct?
#define HOVER_SERVO_FREQ 50
#define HOVER_SERVO_CYCLE_MIN 1000
#define HOVER_SERVO_CYCLE_MAX 2000

//TODO are these values correct?
#define HOVER_MOTOR_FREQ 50
#define HOVER_MOTOR_CYCLE_MIN 0
#define HOVER_MOTOR_CYCLE_MAX 2500

typedef enum {
	HOVER_HWO_TYPE_SERVO,
	HOVER_HWO_TYPE_MOTOR,
} hover_hwo_type;

//hardware controller
typedef struct {
	//apparently there is no pigpio instance and it uses some weird static bs
	u32 used_ports;	//bitfield of used ports
} hover_hw_controller;

//hardware object
typedef struct {
	hover_hw_controller* hwc;
	hover_hwo_type type;
	//the gpio port
	u8 gpiop;
	//specifys whether or not this gpio port supports hardware assisted pwm
	bool pwm_hwa;
	ureg pwm_freq;	//frequency of pwm signal in hertz
	ureg pwm_duty_cycle;	//duty cycle of pwm signal in microseconds
	ureg pwm_min_duty_cycle;
	ureg pwm_max_duty_cycle;
} hover_hwo;

typedef struct {
	hover_hw_controller hwc;
	hover_hwo hover;
	hover_hwo forward;
	hover_hwo steering;
} hover_hw_context;

int hover_hwc_init(hover_hw_controller* hwc)
{
	gpioTerminate();

	hwc->used_ports = 0;
	int ec;
	ec = gpioInitialise();
	if(ec < 0)return ec;
}
int hover_hwc_fin(hover_hw_controller* hwc)
{
	for(int i = 0; i < 8 * sizeof(hwc->used_ports); i++){
		if(hwc->used_ports>>i == 1){

		}
	}
	gpioTerminate();
}

int hover_hwo_init(hover_hwo* hwo, hover_hw_controller* hwc,
				   hover_hwo_type hwot, u8 port)
{
	hwo->hwc = hwc;
	hwo->type = hwot;
	hwo->gpiop = port;

	switch (hwot){
		case HOVER_HWO_TYPE_SERVO:
			hwo->pwm_freq = HOVER_SERVO_FREQ;
			hwo->pwm_min_duty_cycle = HOVER_SERVO_CYCLE_MIN;
			hwo->pwm_max_duty_cycle = HOVER_SERVO_CYCLE_MAX;
			break;
		case HOVER_HWO_TYPE_MOTOR:
			hwo->pwm_freq = HOVER_MOTOR_FREQ;
			hwo->pwm_min_duty_cycle = HOVER_MOTOR_CYCLE_MIN;
			hwo->pwm_max_duty_cycle = HOVER_MOTOR_CYCLE_MAX;
			break;
	}

	switch (port) {
		//list of hardware PWM's
		case 12:
		case 13:
		case 18:
		case 19:
			hwo->pwm_hwa = true;
			gpioHardwarePWM(hwo->gpiop, hwo->pwm_freq, 0);
			break;
		default:
			hwo->pwm_hwa = false;
			//see http://abyz.co.uk/rpi/pigpio/cif.html#gpioServo
			gpioPWM(hwo->gpiop, 0);
			gpioSetPWMfrequency(hwo->gpiop, hwo->pwm_freq);
			gpioSetPWMrange(hwo->gpiop, 1000000 / hwo->pwm_freq);
			break;
	}
}
int hover_hwo_fin(hover_hwo* hwo)
{
	int ec = 0;
	if(hwo->pwm_hwa == false){
		ec = gpioPWM(hwo->gpiop, 0);
	}
	else{
		ec = gpioHardwarePWM(hwo->gpiop, 0, 0);
	}
	hwo->hwc->used_ports &= ~(1 << hwo->gpiop);
	return ec;
}

int hover_hwo_set_duty_fraction(hover_hwo* hwo, float fraction)
{
	u32 duty = hwo->pwm_min_duty_cycle;
	duty += (hwo->pwm_max_duty_cycle - hwo->pwm_min_duty_cycle) * fraction;
	duty *= hwo->pwm_freq;

	if(hwo->pwm_hwa){
		gpioHardwarePWM(hwo->gpiop, hwo->pwm_freq, duty);
	}
	else{
		gpioPWM(hwo->gpiop, duty);
	}
}

void hover_hardware_event_handler(const hover_event_t *event, void *data)
{
	if(event->argLength != 4)
		return;

	hover_hwo *hwo = data;
	s32 value = hover_event_argument_as(event, s32);

	//TODO let the frontend send the duty cycle? or the fraction?
	hover_hwo_set_duty_fraction(hwo, (float)value / 1000);
}
void hover_hardware_init(u8 steering, u8 hover, u8 forward)
{
	hover_hw_context *ctx = malloc(sizeof(hover_hw_context));
	hover_hwc_init(&ctx->hwc);

	hover_hwo_init(&ctx->steering, &ctx->hwc, HOVER_HWO_TYPE_SERVO, steering);
	hover_hwo_init(&ctx->hover, &ctx->hwc, HOVER_HWO_TYPE_MOTOR, hover);
	hover_hwo_init(&ctx->forward, &ctx->hwc, HOVER_HWO_TYPE_MOTOR, forward);

	hover_event_add_handler(HOVER_EVENT_STEERING, hover_hardware_event_handler, &ctx->steering);
	hover_event_add_handler(HOVER_EVENT_HOVER, hover_hardware_event_handler, &ctx->hover);
	hover_event_add_handler(HOVER_EVENT_FORWARD, hover_hardware_event_handler, &ctx->forward);
}

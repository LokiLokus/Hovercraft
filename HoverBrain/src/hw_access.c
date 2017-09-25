#include "../include/hw_access.h"
#include "../include/hw_access.h"
#include <pigpio.h>

#define HOVER_INVALID_GPIOP 255

int hover_hw_controller_init(hover_hw_controller* hwc)
{
	gpioTerminate();

	hwc->used_ports = 0;
	int ec;
	ec = gpioInitialise();
	if(ec < 0)return ec;
}

int hover_hw_controller_fin(hover_hw_controller* hwc)
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

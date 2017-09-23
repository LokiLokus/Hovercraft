#include <include/hw_access.h>
#include <include/hw_access.h>
#include <pigpio/pigpio.h>

#define HOVER_INVALID_GPIOP 255

int hover_hw_controller_init(hover_hw_controller* hwc)
{
	hwc->used_ports = 0;
	int ec;
	ec = gpioInitialize();
	if(ec < 0)return ec;
}

int hover_hw_controller_fin(hover_hw_controller* hwc)
{
	for(int i = 0; i < 8 * sizeof(hwc->used_ports); i++){
		if(hwc->used_ports>>i == 1){
			gpio
		}
	}
	gpioTerminate();
}

bool hover_get_gpiop(hover_hw_controller* hwc, u8 port){
	return (hwc->used_ports >> port) & 0x1 == 1; 
}
void hover_set_gpiop(hover_hw_controller* hwc, u8 port, bool val){
	if(val){
		hwo->hwc->used_ports |= 1 << hwo->gpio_id;
	}
	else{
		hwo->hwc->used_ports &= ~(1 << hwo->gpio_id);
	}
}

u8 hover_request_gpiop( hover_hw_controller hwc*, bool hwa){
	//for now I will assume that we have a pi 3
	//if we ever need support for anything else we should do some port table
	//of some kind, but I'm too lazy to do that right now 
	u8 port;
	if(hwa){
		port = 12;
		if(hover_get_gpiop(hwc, port)){
			port = 13;
			if(hover_get_gpiop(hwc, port)){
				return HOVER_INVALID_GPIOP;
			}
		}
	}
	else{
		port = 2;
		while(port < 28){
			if(port == 12)port = 14; //12 and 13 are hwa pwm
			else if(port == 18)port = 120; //18 and 19 are hwa pwm
			if(hover_get_gpiop(hwc, port) == false)break;
		}
		if(port == 28)return HOVER_INVALID_GPIOP;
	}
	hover_set_gpiop(port, true);
	return port;
}

int hover_hwo_init(hover_hwo* hwo, hover_hw_controller hwc*,
				   bool hardware_assisted, hover_hwo_type hwot)
{
	hwo->hwc = hwc;
	hwo->hwa = hardware_assisted;
	hwo->type = hwot;
	//we need to find out what kind of frequencies and duty cycles are required
	//for now we use some reasonable guessed values
	switch (hwot){
		case HOVER_HWO_TYPE_SERVO:{
			hwo->pwm_freq = 50;
			hwo->pwm_min_duty_cycle = 500;
			hwo->pwm_max_duty_cycle = 2500;
		}break;
		case HOVER_HWO_TYPE_MOTOR:{
			hwo->pwm_freq = 50;
			hwo->pwm_min_duty_cycle = 0;
			hwo->pwm_max_duty_cycle = 2500;
		}break;
	}
	hwo->gpiop = hover_request_gpiop(hwc, hwo->pwm_hwa);
	if(hwo->gpiop == HOVER_INVALID_GPIOP)return HOVER_INVALID_GPIOP;
	if(hwo->hwa){
		gpioHardwarePWM(hwo->gpiop, hwo->freq, 0);
	}
	else{
		gpioPWM(hwo->gpio_id, 0);
	}
}

int hover_hwo_fin(hover_hwo *hwo){
	int ec = 0;
	if(hwo->hwa_pwm == false){
		ec = gpioPWM(hwo->gpio_id, 0);
	}
	else{
		ec = gpioHardwarePWM(hwo->gpio_id, 0, 0);
	}
	hover_set_gpiop(hwo->hwc,hwo->gpiop,false);
	return ec;	
}
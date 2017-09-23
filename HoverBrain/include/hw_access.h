#pragma once
#include <include/types.h>
#include <include/dbuffer.h>
//HWA stands for hardware assisted
typedef enum {
	HOVER_HWO_TYPE_SERVO,
	HOVER_HWO_TYPE_MOTOR,
} hover_hwo_type;

//hardware controller
typedef struct {
	//apparently there is no pigpio instance and it uses some weird static bs
	u32 used_ports;	//bitfield of used ports
}hover_hw_controller;

int hover_hw_controller_init(hover_hw_controller* hwc);
int hover_hw_controller_fin(hover_hw_controller* hwc);

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
	ureg pwm_min_duty_cycle; //min duty cycle (0%) 
	ureg pwm_max_duty_cycle; //max duty cycle (100%) 

} hover_hwo;


int hover_hwo_init(hover_hwo* hwo, hover_hw_controller hwc*,
				   bool hwa, hover_hwo_type hwot);

int hover_hwo_fin(hover_hwo *hwo);

int hover_hwo_set_motor_speed(hover_hwo* hwo, float fraction);
int hover_set_servo_pos(hover_hwo* hwo, float fraction);

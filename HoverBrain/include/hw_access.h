#pragma once
#include "types.h"

#define HOVER_HWO_SERVO_FREQ 50
#define HOVER_HWO_SERVO_CYCLE_MIN 1000
#define HOVER_HWO_SERVO_CYCLE_MAX 2000

#define HOVER_HWO_MOTOR_FREQ 50
#define HOVER_HWO_MOTOR_CYCLE_MIN 0
#define HOVER_HWO_MOTOR_CYCLE_MAX 2500

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
	ureg pwm_min_duty_cycle;
	ureg pwm_max_duty_cycle;
} hover_hwo;

int hover_hwo_init(hover_hwo* hwo, hover_hw_controller* hwc,
				   bool hardware_assisted, hover_hwo_type hwot);

int hover_hwo_fin(hover_hwo *hwo);

//we might need specialized methods for motor and servo but I
//have no Idea what kind of values we will need so ... lets do nothing for now
int hover_hwo_set_duty_fraction(hover_hwo* hwo, float fraction);

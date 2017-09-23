#pragma once
#include <std_types.h>
#include <dbuffer.h>
//HWA stands for hardware assisted
typedef enum {
	HOVER_HWO_PRESET_HWA_SERVO,
	HOVER_HWO_PRESET_SERVO,
	HOVER_HWO_PRESET_MOTOR,
} hover_hwo_preset;

//hardware controller
typedef struct {
	u32 pigpoi_inst;
	dbuffer used_ports;
}hover_hw_controller;

int hover_hw_controller_init(hover_hw_controller* hwc);
int hover_hw_controller_fin(hover_hw_controller* hwc);

//hardware object
typedef struct {
	u8 gpio_id;	//pigpoi representation specifying which gpio port is used for this hwo
	bool hwa_pwm; //specifys whether or not this hwo's gpio port supports hardware pwm
	hover_hw_controller* hwc;
	ureg lamda;	//length of one wave in microseconds
	ureg duty_cycle;	//duty cycle of pwm signal in microseconds
	ureg max_duty_cicle; //max duty cycle (100%) 
} hover_hwo;


int hover_hwo_init(hover_hw_controller, hover_hwo *hwo, hover_hwo_preset preset);
int hover_hwo_fin(hover_hwo *hwo);

int hover_hwo_set_duty_cycle(float fraction);



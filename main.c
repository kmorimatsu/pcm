#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "./wav.h"

int main() {
	float counter=0.0;
	unsigned int port,slice,channel;
	int cpos,wpos;
	stdio_init_all();
	sleep_ms(3000);
	printf("Hello World!");

	port=28;
	slice=6;
	channel=PWM_CHAN_A;

	// Allocate GPIO to the PWM
	gpio_set_function(port, GPIO_FUNC_PWM);
	// Set clock divier for frequency
	// f = sysclock / (( wrap + 1 ) x clkdiv)
	// clkdiv = sysclock / ( wrap + 1 ) x f 
	// ( 1.0 <= clkdiv < 256.0 ) 
	pwm_set_clkdiv(slice,1.0); // The fastest clock
	// 1000 cycles PWM
	pwm_set_wrap(slice, 255);
	// Set duty
	pwm_set_chan_level(slice, channel, 128);
	// Enable
	pwm_set_enabled(slice, true);
	
	cpos=time_us_32();
	wpos=0;
	while(wpos < sizeof wav){
		// 10000000 / 16000 = 62.5
		cpos+=63;
		sleep_until(cpos);
		pwm_set_chan_level(slice, channel, wav[wpos++]);
		cpos+=62;
		sleep_until(cpos);
		pwm_set_chan_level(slice, channel, wav[wpos++]);
	}
}

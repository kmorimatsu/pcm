#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

int main() {
	float counter=0.0;
	unsigned int port,slice,channel;
	int i;
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
	pwm_set_clkdiv(slice,1.0);
	// 1000 cycles PWM
	pwm_set_wrap(slice, 255);
	// Set duty
	pwm_set_chan_level(slice, channel, 128);
	// Enable
	pwm_set_enabled(slice, true);
	
	i=time_us_32();
	
	while(1){
		i+=63;
		sleep_until(i);
		counter+=0.1728; //3.141593*2.0*440.0/16000.0;
		pwm_set_chan_level(6, PWM_CHAN_A, (int)(128.0 + sin(counter)*127.9));
	}
}

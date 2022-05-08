#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

int main() {
	unsigned int port,slice,channel;
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
	pwm_set_clkdiv(slice,62500.00/440.0);
	// 1000 cycles PWM
	pwm_set_wrap(slice, 1999);
	// Set duty
	pwm_set_chan_level(slice, channel, 1000);
	// Enable
	pwm_set_enabled(slice, true);

}

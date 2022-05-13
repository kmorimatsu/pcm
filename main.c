#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "./wav.h"
#include "./b5th.h"

// Use port 28 for PCM audio output
#define PCM_PORT 28
#define PCM_SLICE 6
#define PCM_CHAN PWM_CHAN_A

/*
	Initialize PWM for PCM
	
	void pwm_init_for_pcm(void);
*/

void pwm_init_for_pcm(void){
	int i;
	// Allocate GPIO to the PWM
	gpio_set_function(PCM_PORT, GPIO_FUNC_PWM);
	// Set clock divier for fastest frequency
	pwm_set_clkdiv(PCM_SLICE,1.0);
	// 256 cycles PWM
	pwm_set_wrap(PCM_SLICE, 255);
	// Set duty
	pwm_set_chan_level(PCM_SLICE, PCM_CHAN, 0);
	// Enable
	pwm_set_enabled(PCM_SLICE, true);
	// Wake up
	for(i=0;i<=0x80;i++){
		pwm_set_chan_level(PCM_SLICE, PCM_CHAN,i);
		sleep_us(100);
	}
}

/*
	Sleep until at seconds after starting
	
	void sleep_until_seconds(unsigned int sec);
	
	Maximum value: 4294
*/
void sleep_until_seconds(unsigned int sec){
	static unsigned int cpos;
	if (sec) {
		// Sleep
		sleep_until(cpos+sec*1000000);
	} else {
		// Init
		cpos=time_us_32();
	}
}

/*
	Check the battery status
	
	int check_battery(void);
	
	This returns 1 if low battery.
	Returns 0 if not.
*/
int check_battery(void){
	int i;
	unsigned short res,sum,min,max;
	// Use GPIO29 for reading VSUS/3
    adc_gpio_init(29);
    // Select ADC input 3
    adc_select_input(3);
	// Read 6 times, remove minimum and maximum values, and calculate average.
	min=4096;
	max=0;
	sum=0;
	for(i=0;i<6;i++){
		sleep_ms(1);
		res=adc_read();
		if (res<min) min=res;
		if (max<res) max=res;
		sum+=res;
	}
	sum=sum-min-max;
	res=sum>>2;
	// 3.0 V: 1241
	// 2.4 V: 993
	// 2.2 V: 910
	// 2.0 V: 827
	// 1.9 V: 786
	// 1.8 V: 745
	return res<827 ? 1:0;
}

/*
	Macro/function to play wav file array
	
	void play_wav(const unsigned char[] wav_data);
*/

#define play_wav(a) _play_wav (&a[0],sizeof a)
void _play_wav(const unsigned char* wav_data, unsigned int wav_size){
	unsigned int cpos,wpos;
	wav_size&=0xfffffffe;
	cpos=time_us_32();
	wpos=0;
	while(wpos < wav_size){
		// 10000000 / 16000 = 62.5
		cpos+=63;
		sleep_until(cpos);
		pwm_set_chan_level(PCM_SLICE, PCM_CHAN, wav_data[wpos++]);
		cpos+=62;
		sleep_until(cpos);
		pwm_set_chan_level(PCM_SLICE, PCM_CHAN, wav_data[wpos++]);
	}
}

int main() {
	// Initializations
	stdio_init_all();
	adc_init();
	sleep_until_seconds(0);
	pwm_init_for_pcm();
	
	// Check battery and play wav in the beginning
	if (check_battery()) play_wav(err0);
	//else play_wav(wav0);
	else play_wav(b5th);
	
	// Wait for 10 minutes and play
	sleep_until_seconds(600);
	play_wav(wav1);
	sleep_until_seconds(610);
	play_wav(wav1);
	sleep_until_seconds(620);
	play_wav(wav1);
	sleep_until_seconds(630);
	play_wav(wav1);
	sleep_until_seconds(640);
	play_wav(wav1);
	sleep_until_seconds(650);
	play_wav(wav1);
	sleep_until_seconds(660);
	play_wav(wav1);
}

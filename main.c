#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "./wav.h"
#include "./b5th.h"
#include "./tc1.h"

/*
	Configurations
*/

// Use port 28 for PCM audio output
#define PCM_PORT 28
#define PCM_SLICE 6
#define PCM_CHAN PWM_CHAN_A

// Use port 22 for controlling audio output
#define CS_AUDIO 22

// Select PCM setting of either 16k Hz 8 bit or 12k Hz 12 bit
//#define PCM_16k_8
#define PCM_12k_12

/*
	Initialize PWM for PCM
	
	void pwm_init_for_pcm(void);
*/
#if defined PCM_16k_8
	#define pwm_init_for_pcm() pwm_init_for_pcm_16k_8()
#elif defined PCM_12k_12
	#define pwm_init_for_pcm() pwm_init_for_pcm_12k_12()
#endif

void pwm_init_for_pcm_16k_8(void){
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

void pwm_init_for_pcm_12k_12(void){
	int i;
	// Allocate GPIO to the PWM
	gpio_set_function(PCM_PORT, GPIO_FUNC_PWM);
	// Set clock divier for fastest frequency
	pwm_set_clkdiv(PCM_SLICE,1.0);
	// 4096 cycles PWM
	pwm_set_wrap(PCM_SLICE, 4095);
	// Set duty
	pwm_set_chan_level(PCM_SLICE, PCM_CHAN, 0);
	// Enable
	pwm_set_enabled(PCM_SLICE, true);
	// Wake up
	for(i=0;i<=0x800;i+=0x10){
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
	Play wav file array
	
	void play_wav(const unsigned char[] wav_data);
*/
#if defined PCM_16k_8
	#define play_wav(a) _play_wav_16k_8 (&a[0],sizeof a)
#elif defined PCM_12k_12
	#define play_wav(a) _play_wav_12k_12 (&a[0],sizeof a)
#endif

void _play_wav_16k_8(const unsigned char* wav_data, unsigned int wav_size){
	unsigned int cpos,wpos;
	gpio_put(CS_AUDIO,0);
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
	gpio_put(CS_AUDIO,1);
}

void _play_wav_12k_12(const unsigned char* wav_data, unsigned int wav_size){
	unsigned int cpos,wpos,i;
	unsigned short d;
	gpio_put(CS_AUDIO,0);
	cpos=time_us_32();
	wpos=0;
	d=wav_data[wpos++];
	d|=(wav_data[wpos]&0x0f)<<8;
	for(i=0;wpos < wav_size;i++){
		// 10000000 / 12000 = 83.333333
		switch(i){
			case 0:
			case 3:
				cpos+=83;
				break;
			case 1:
			case 4:
				cpos+=83;
				break;
			case 2:
			case 5:
				cpos+=84;
				break;
		}
		switch(i){
			case 0:
			case 2:
			case 4:
				sleep_until(cpos);
				pwm_set_chan_level(PCM_SLICE, PCM_CHAN, d);
				d=wav_data[wpos++]>>4;
				d|=wav_data[wpos++]<<4;
				break;
			case 5:
				i=-1;
			case 1:
			case 3:
				sleep_until(cpos);
				pwm_set_chan_level(PCM_SLICE, PCM_CHAN, d);
				d=wav_data[wpos++];
				d|=(wav_data[wpos]&0x0f)<<8;
				break;
		}
	}
	gpio_put(CS_AUDIO,1);
}

int main() {
	int i;
	// Initializations
	stdio_init_all();
	adc_init();
	sleep_until_seconds(0);
	pwm_init_for_pcm();
	gpio_init(CS_AUDIO);
	gpio_set_dir(CS_AUDIO, GPIO_OUT);
	gpio_put(CS_AUDIO,1);
	
	// Check battery and play wav in the beginning
	if (check_battery()) play_wav(b5thi);
	else play_wav(b6thi);
	
	// Wait for 7 minutes and play every 3 minutes
	for(i=420;true;i+=180){
		sleep_until_seconds(i);
		play_wav(tc1);
	}
}

// delay_intr.c

#include "audio.h"

volatile int16_t audio_chR=0;    
volatile int16_t audio_chL=0;    

#define DELAY_BUF_SIZE 48000

int16_t buffer[DELAY_BUF_SIZE];
int16_t i = 0;
 

void I2S_HANDLER(void) {   /****** I2S Interruption Handler*****/

int16_t delayed_sample;
int16_t audio_out_chL = 0;	
	
audio_IN = i2s_rx();	
audio_chL = (audio_IN & 0x0000FFFF);      
audio_chR = ((audio_IN >>16)& 0x0000FFFF); 

    delayed_sample = buffer[i];
    audio_out_chL = delayed_sample + audio_chL;
    buffer[i] = audio_chL;
    i = (i+1) % DELAY_BUF_SIZE;
	
audio_OUT = ((audio_chR<<16 & 0xFFFF0000)) + (audio_out_chL & 0x0000FFFF);	
i2s_tx(audio_OUT);

}

int main(void)
{
  audio_init ( hz48000, mic_in, intr, I2S_HANDLER); 
 
  while(1){}
}


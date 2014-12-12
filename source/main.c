#include "stm32f4xx.h"                  // Device header

void timer2_config(void);
void timer2_delay_ms(int time_delay);

void led_config(void);
void led_output(int pin_val);

int main(){
	int ledState=0;
	led_config();
	timer2_config();
	
	while(1){
		led_output(ledState);
		timer2_delay_ms(500);
		ledState=(ledState==0)?1:0;
	}
}

void timer2_config(void){
	RCC->APB1ENR|=(0x1);//Enable TIM2
	
	//configure timer 
	TIM2->CR1=0;
	TIM2->PSC=(unsigned short)15999;//freq div by 1600
	TIM2->CR1|=((0x0<<8)//CKD 00: tds=tcd_int/1
						|(0x0<<7)//ARPE 0: TIMxARR is not buffered
						|(0x0<<5)//CMS don't care
						|(0x0<<4)//DIR 0:counter is used upcounter
						|(0x1<<3)//OPM 1:Counter stop counting ar the next update event
						|(0x1<<2)//URS 1:Only overflows/ubdeflow generate an update interrupt
						|(0x0<<1)//UDIS 0:Update events enabled
						|(0x0<<0)//CEN 0:Counter disabled;
							);
}



void timer2_delay_ms(int delay_time){
	/*
	int i;
	for(i=0;i<(delay_time*0xFFF);i++){
	}
	*/
	TIM2->CNT=0;
	TIM2->ARR=(delay_time-1);//set overflow value
	TIM2->SR=0;//clear event flags
	TIM2->CR1|=(0x1<<0);//enable counter 
	while(((TIM2->SR)&0x1)==0);//wait for overflow event
}

void led_config(void){
	RCC->AHB1ENR|=(0x1);//Enable GPIOA
	GPIOA->MODER|=(0x1<<10);// Pin_A5 as general purpose output
}

void led_output(int pin_val){
	if(pin_val==0){
		GPIOA->BSRRH=(0x1<<5);// reset Pin_A5
	}else{
		GPIOA->BSRRL=(0x1<<5);// set Pin_A5
	}
}



//call libs
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "tm4c123gh6pm.h" 

//define clock register
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
//define port a registers
#define GPIO_PORTA_DATA_R       (*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_PUR_R        (*((volatile unsigned long *)0x40004510))
#define GPIO_PORTA_LOCK_R       (*((volatile unsigned long *)0x40004520))
#define GPIO_PORTA_CR_R         (*((volatile unsigned long *)0x40004524))
#define GPIO_PORTA_AMSEL_R      (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile unsigned long *)0x4000452C))
//define port e registers
#define GPIO_PORTE_DATA_R     (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R      (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_DEN_R      (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AFSEL_R    (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R      (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_LOCK_R     (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R       (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R    (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R     (*((volatile unsigned long *)0x4002452C))
//define port f registers
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
	
//define systick register
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))

//define functions
void PortA_Init(void);
void PortE_Init(void);
void PortF_Init(void);
void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void systick_wait_N_ms(unsigned long N);

int state=0,off=0,walk=1,south=2,west=3;


int main(void){
	PortA_Init();
	PortE_Init();
	PortF_Init();
	SysTick_Init();
	
	GPIO_PORTA_DATA_R=0x00;
	GPIO_PORTE_DATA_R=0x00;
	GPIO_PORTF_DATA_R=0x00;
	
int last_state=0;
	
	while(1){
	
		switch(state){
			
			case 0:
						state=last_state++;
						break;
					
			case 1:
					if((GPIO_PORTA_DATA_R&0x80)!=0x80)
						GPIO_PORTA_DATA_R|=0x40;
					if(((GPIO_PORTA_DATA_R&0x10)!=0x10))
						GPIO_PORTA_DATA_R|=0x08; // pa3 are high
					systick_wait_N_ms(1);
			
					GPIO_PORTA_DATA_R=0x90;
					GPIO_PORTF_DATA_R=0x08;
					
					systick_wait_N_ms(10);

					for(int i=0;i<3;i++){
					GPIO_PORTF_DATA_R=0x02;
					systick_wait_N_ms(1);
					GPIO_PORTF_DATA_R=0x00;
					systick_wait_N_ms(1);
					}
					
				
					if((GPIO_PORTE_DATA_R&0x02)==0x02){
						state=south;
						break;
					}
					else if((GPIO_PORTE_DATA_R&0x01)==0x01){
						state=west;
						break;
					}
					else if((GPIO_PORTE_DATA_R&0x04)==0x04){
						systick_wait_N_ms(2);	
						if((GPIO_PORTE_DATA_R&0x04)==0x04){					
							state=walk;
							break;
						}
					}
					else{
						state=off;
						break;
					}
			case 2:
					if((GPIO_PORTA_DATA_R&0x20)!=0x20)
						GPIO_PORTA_DATA_R|=0x40;
					if(((GPIO_PORTA_DATA_R&0x10)!=0x10))
						GPIO_PORTA_DATA_R|=0x08; // pa3 is high
					systick_wait_N_ms(1);
			
					GPIO_PORTA_DATA_R=0x30;
					GPIO_PORTF_DATA_R=0x02;
					
					systick_wait_N_ms(10);
					if((GPIO_PORTE_DATA_R&0x04)==0x04){
						systick_wait_N_ms(2);	
						if((GPIO_PORTE_DATA_R&0x04)==0x04){					
							state=walk;
							break;
						}
					}
					else if((GPIO_PORTE_DATA_R&0x01)==0x01){
						state=west;
						break;
					}
					else if((GPIO_PORTE_DATA_R&0x02)==0x02){
						state=south;
						break;
					}
					else{
						state=off;
						break;
					}
			
			case 3:
					
					
					if((GPIO_PORTA_DATA_R&0x80)!=0x80)
						GPIO_PORTA_DATA_R|=0x40;
					if(((GPIO_PORTA_DATA_R&0x04)!=0x04))
						GPIO_PORTA_DATA_R|=0x08; // pa3 is high
					systick_wait_N_ms(1);
			
					GPIO_PORTA_DATA_R=0x84;
					GPIO_PORTF_DATA_R=0x02;
					
					systick_wait_N_ms(10);	
					
					last_state=0;			
					if((GPIO_PORTE_DATA_R&0x04)==0x04){
						systick_wait_N_ms(2);	
						if((GPIO_PORTE_DATA_R&0x04)==0x04){					
							state=walk;
							break;
						}
					}
					else if((GPIO_PORTE_DATA_R&0x02)==0x02){
						state=south;
						break;
					}
					else if((GPIO_PORTE_DATA_R&0x01)==0x01){
						state=west;
						break;
					}
					else{
						state=off;
						break;
					}
			default:
			state=off;
			break;
			
		}
	}
}



//write functions
void PortA_Init(void){				       //initialize PortA
	
	volatile unsigned long delay;      // define unsigned long named with delay
  SYSCTL_RCGC2_R |= 0x00000001;      // 1) set the clock for port A
  delay = SYSCTL_RCGC2_R;            // reading register adds a delay   
  GPIO_PORTA_LOCK_R = 0x4C4F434B;    // 2) unlock PortA  
  GPIO_PORTA_CR_R = 0xFF;            // 3) allow changes to PA6-0       
  GPIO_PORTA_AMSEL_R = 0x00;         // 4) disable analog function
  GPIO_PORTA_PCTL_R = 0x00000000;    // 5) GPIO clear bit PCTL  
  GPIO_PORTA_DIR_R = 0xFF;           // 6) PA6-0 output   
  GPIO_PORTA_AFSEL_R = 0x00;         // 7) no alternate function
  GPIO_PORTA_PUR_R = 0x00;           // 8) enable pull up resistors for PA6-0
  GPIO_PORTA_DEN_R = 0xFF;           // 9) enable digital pins PF6-0 

}

void PortE_Init(void){				       //initialize PortE
	
	volatile unsigned long delay;      // define unsigned long named with delay
  SYSCTL_RCGC2_R |= 0x00000010;      // 1) set the clock for port E
  delay = SYSCTL_RCGC2_R;            // reading register adds a delay   
  GPIO_PORTE_LOCK_R = 0x4C4F434B;    // 2) unlock PortE 
  GPIO_PORTE_CR_R = 0x07;            // 3) allow changes to PE2-0       
  GPIO_PORTE_AMSEL_R = 0x00;         // 4) disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;    // 5) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R = 0x00;           // 6) ALL OF PORT E IS INPUT  
  GPIO_PORTE_AFSEL_R = 0x00;         // 7) no alternate function
  GPIO_PORTE_PUR_R = 0x00;           // 8) enable pull-up resistors on PE2-0      
  GPIO_PORTE_DEN_R = 0x07;           // 9) enable digital pins PE2-0     

}

void PortF_Init(void){				       //initialize PortF
	
	volatile unsigned long delay;      // define unsigned long named with delay
  SYSCTL_RCGC2_R |= 0x00000020;      // 1) set the clock for port f
  delay = SYSCTL_RCGC2_R;            // reading register adds a delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;  	 // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;            // 3) allow changes to PA4-0         
  GPIO_PORTF_AMSEL_R = 0x00;         // 4) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;    // 5) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;           // 6) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;         // 7) no alternate function
  GPIO_PORTF_PUR_R = 0x00;           // 8) enable pull-up resistors on PF4,PF0     
  GPIO_PORTF_DEN_R = 0x1F;           // 9) enable digital pins PF4-PF0              

}

void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 0x00FFFFFF;        // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it             
  NVIC_ST_CTRL_R = 0x00000005;          // enable SysTick with core clock
}

// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(unsigned long delay){
  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait, the reload value
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ 		// wait for count flag
		
  }
}

void systick_wait_N_ms(unsigned long N){ // if N is 5, wait 5 ms 
  unsigned long i;
  for(i=0; i<(5*N); i++){
    SysTick_Wait(80000000);  // 80mhz core clock
  }
}

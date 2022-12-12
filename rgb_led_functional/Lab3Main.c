#include <stdint.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"

#define SYSCTL_RCG2_R 		(*((volatile unsigned long *)0x400FE108)) // define the address of clk register
#define SYSCTL_RCG2_GPIOF 0x00000020 // PORT F CLOCK GATING CONTROL // define the hexadecimal number that sets the clock for port f
#define GPIO_PORTF_DATA_R (*((volatile unsigned long *)0x400253FC)) // define the address of data register
#define GPIO_PORTF_DIR_R 	(*((volatile unsigned long *)0x40025400)) // define the address of direction register
#define GPIO_PORTF_DEN_R	(*((volatile unsigned long *)0x4002551C)) // define the address of digital enable register
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420)) // define the address of analog function register
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510)) // define the address of pull-up resistor register
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520)) // define the address of lock register
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524)) // define the address of control register
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528)) // define the address of analog mode selection register
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C)) // define the address of pctl register

void delay(int sec); // define the delay function	

void Hz(int freq);  // define the frequency function	

void PortF_Init(void); // define the port f init function

int sw1control(int number); // define the function that looks for sw1 pressed or not

int cond=0; // define the integer that sets the sw1's actions 

int main(void){
	SYSCTL_RCG2_R = SYSCTL_RCG2_GPIOF; // set the clock for port f
	GPIO_PORTF_DATA_R = 0x00; // clear the portf's data register
	PortF_Init(); // call the function that initializates the port f 
	
	
	while(1){
		checkpoint:
		if((GPIO_PORTF_DATA_R&0x10)==0x10){
			
			for (int k=0; k<=100;k++){
				for (int j=0; j<=k; j++){
					GPIO_PORTF_DATA_R = 0x02; // red
					Hz(8000);
					if((GPIO_PORTF_DATA_R&0x10)==0x00){
						goto checkpoint;
					}
				}
				
				for (int g=0;g<=(100-k);g++){
					GPIO_PORTF_DATA_R = 0x00; // off
					Hz(8000);
					if((GPIO_PORTF_DATA_R&0x10)==0x00){
						goto checkpoint;
					}
				}
			}
		
			for (int k=0; k<=100;k++){
				for (int j=0; j<=k; j++){
					GPIO_PORTF_DATA_R = 0x00; // off
					Hz(8000);
					if((GPIO_PORTF_DATA_R&0x10)==0x00){
						goto checkpoint;
					}
				}
				
				for (int g=0;g<=(100-k);g++){
					GPIO_PORTF_DATA_R = 0x02; // red
					Hz(8000);
					if((GPIO_PORTF_DATA_R&0x10)==0x00){
						goto checkpoint;
					}
				}
			}
			
				
			for (int k=0; k<=25;k++){
				for (int j=0; j<=25; j++){
					GPIO_PORTF_DATA_R = 0x00; // off
				}			
			}
		}
		else {
			
			GPIO_PORTF_DATA_R = 0x0C; // blue
			Hz(40); // (1/8)x(0.2)=1/40 so 40Hz
			GPIO_PORTF_DATA_R = 0x00; // off LED
			Hz(10); // (1/8)x(0.2)x4=1/10 so 10Hz
			
		}
	}	
	return 0;
}


void delay(int sec){ // delay function
	
	int c = 1, d = 1; // define 2 integers
	for ( c = 1 ; c <= sec ; c++) // for loop to wait for expecting seconds
		for ( d = 1 ; d <=5000000 ; d++) {} // for loop to set a second
}

void Hz(int freq){ // frequency function
	
	int	d = 1; // define an integer
	for ( d = 1 ; d <=(5000000/freq) ; d++) {} // for loop to set frequency
}


void PortF_Init(void){				//initialize PortF
	
	volatile unsigned long delay; // define unsigned long named with delay
  SYSCTL_RCG2_R |= 0x00000020;      // 1) set the clock for port f
  delay = SYSCTL_RCG2_R;            // reading register adds a delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // 3) allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 4) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 5) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 6) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 7) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // 8) enable pull-up resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 9) enable digital pins PF4-PF0        

}


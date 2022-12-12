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

void PortF_Init(void); // define the port f init function

int sw1control(int number); // define the function that looks for sw1 pressed or not

int sw2control(int nmbr); // define the function that looks for sw2 pressed or not

int cond=0; // define the integer that sets the sw1's actions 
int ylw=0; // define the integer that sets the sw1's actions 

int main(void){
	SYSCTL_RCG2_R = SYSCTL_RCG2_GPIOF; // set the clock for port f
	GPIO_PORTF_DATA_R = 0x00; // clear the portf's data register
	
	PortF_Init(); // call the function that initializates the port f 
	
	while(1){ // endless loop
		
		if(ylw==0){ // if yellow enable from sw2
			if(cond==0){ // for case 0

				GPIO_PORTF_DATA_R = 0x02; // red 
				delay(10+cond*5); // delay for: 10 + (case number) * 5 = 10 + 0 * 5 = 10
				GPIO_PORTF_DATA_R = 0x0A; // yellow
				delay(3+cond*5); // delay for: 3 + (case number) * 5 = 3 + 0 * 5 = 3
				GPIO_PORTF_DATA_R = 0x08; // green
				delay(6+cond*5); // delay for: 6 + (case number) * 5 = 6 + 0 * 5 = 6
			}
			else if(cond==1){
				GPIO_PORTF_DATA_R = 0x02; // red 
				delay(10+cond*5); // delay for: 10 + (case number) * 5 = 10 + 1 * 5 = 15 
				GPIO_PORTF_DATA_R = 0x0A;
				delay(3+cond*5); // delay for: 3 + (case number) * 5 = 3 + 1 * 5 = 8 
				GPIO_PORTF_DATA_R = 0x08; // green
				delay(6+cond*5); // delay for: 6 + (case number) * 5 = 6 + 1 * 5 = 11 
			}
			else if(cond==2){
				GPIO_PORTF_DATA_R = 0x02; // red 
				delay(10+cond*5); // delay for: 10 + (case number) * 5 = 10 + 2 * 5 = 20 
				GPIO_PORTF_DATA_R = 0x0A; // yellow
				delay(3+cond*5); // delay for: 3 + (case number) * 5 = 3 + 2 * 5 = 13
				GPIO_PORTF_DATA_R = 0x08; // green
				delay(6+cond*5); // delay for: 6 + (case number) * 5 = 6 + 2 * 5 = 16
			}
			else if(cond==3){
				GPIO_PORTF_DATA_R = 0x02; // red 
				delay(10+cond*5); // delay for: 10 + (case number) * 5 = 10 + 3 * 5 = 25 
				GPIO_PORTF_DATA_R = 0x0A; // yellow
				delay(3+cond*5); // delay for: 3 + (case number) * 5 = 3 + 3 * 5 = 18 
				GPIO_PORTF_DATA_R = 0x08; // green
				delay(6+cond*5); // delay for: 6 + (case number) * 5 = 6 + 3 * 5 = 21
			}		
			else if(cond==4){
				GPIO_PORTF_DATA_R = 0x02; // red 
				delay(10+cond*5); // delay for: 10 + (case number) * 5 = 10 + 4 * 5 = 30 
				GPIO_PORTF_DATA_R = 0x0A; // yellow
				delay(3+cond*5); // delay for: 3 + (case number) * 5 = 3 + 4 * 5 = 23
				GPIO_PORTF_DATA_R = 0x08; // green
				delay(6+cond*5); // delay for: 6 + (case number) * 5 = 6 + 4 * 5 = 26
			}			
		}

		if(ylw==1){
			if(cond==0){
				GPIO_PORTF_DATA_R = 0x02; // red 
				delay(10+cond*5); // delay for: 10 + (case number) * 5 = 10 + 0 * 5 = 10 
				GPIO_PORTF_DATA_R = 0x08; // green
				delay(6+cond*5); // delay for: 6 + (case number) * 5 = 6 + 0 * 5 = 6
			}
			else if(cond==1){
				GPIO_PORTF_DATA_R = 0x02; // red 
				delay(10+cond*5); // delay for: 10 + (case number) * 5 = 10 + 1 * 5 = 15 
				GPIO_PORTF_DATA_R = 0x08; // green
				delay(6+cond*5); // delay for: 6 + (case number) * 5 = 6 + 1 * 5 = 11
			}
			else if(cond==2){
				GPIO_PORTF_DATA_R = 0x02; // red 
				delay(10+cond*5); // delay for: 10 + (case number) * 5 = 10 + 2 * 5 = 20 
				GPIO_PORTF_DATA_R = 0x08; // green
				delay(6+cond*5); // delay for: 6 + (case number) * 5 = 6 + 2 * 5 = 16
			}
			else if(cond==3){
				GPIO_PORTF_DATA_R = 0x02; // red 
				delay(10+cond*5); // delay for: 10 + (case number) * 5 = 10 + 3 * 5 = 25 
				GPIO_PORTF_DATA_R = 0x08; // green
				delay(6+cond*5); // delay for: 6 + (case number) * 5 = 6 + 3 * 5 = 21
			}		
			else if(cond==4){
				GPIO_PORTF_DATA_R = 0x02; // red 
				delay(10+cond*5); // delay for: 10 + (case number) * 5 = 10 + 4 * 5 = 30 
				GPIO_PORTF_DATA_R = 0x08; // green
				delay(6+cond*5); // delay for: 6 + (case number) * 5 = 6 + 4 * 5 = 26
			}			
		}	
		
		cond = sw1control(cond); // call the function that looks for sw1 and returned value equals to cond
		ylw = sw2control(ylw); 	 // call the function that looks for sw2 and returned value equals to ylw
		
	}
	return 0;
}

void delay(int sec){ // delay function
	
	int c = 1, d = 1; // define 2 integers
	for ( c = 1 ; c <= sec ; c++) // for loop to wait for expecting seconds
		for ( d = 1 ; d <=5000000 ; d++) {} // for loop to set a second
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

int sw1control(int number){ 		 // sw1 control function
	unsigned long sw1; 						 // define unsigned long named with sw1
	sw1=GPIO_PORTF_DATA_R & 0x10;  // (SW1 = 0x10 AND DATA REGISTER) --> to make enable to read for just sw1's data bit
	if(	sw1 == 0x00){ 						 // if sw1 is pressed
	
			number = number + 1;			 // increase the number for 1
			if(number==5){ 						 // if number == 5 
				number = 0; 						 // equalize the number to 0
			}
	}
	return number; 								 // return the number's value
}

int sw2control(int nmbr){ 				// sw2 control function
	unsigned long sw2; 							// define unsigned long named with sw2
	sw2=GPIO_PORTF_DATA_R & 0x01;		// (SW1 = 0x01 AND DATA REGISTER) --> to make enable to read for just sw2's data bit
	if( sw2 == 0x00){ 							// if sw2 is pressed
		nmbr^=1;											// toggle the nmbr
	}  
	return nmbr; 										// return the nmbr's value
}
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "tm4c123gh6pm.h" 

//define clock registers
#define SYSCTL_RCC_R            (*((volatile unsigned long *)0x400FE060))
#define SYSCTL_RCC2_R           (*((volatile unsigned long *)0x400FE070)) 
#define SYSCTL_RCGC0_R          (*((volatile unsigned long *)0x400FE100))		// ADC clock
#define SYSCTL_RIS_R            (*((volatile unsigned long *)0x400FE050))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))	
#define SYSCTL_RCGCADC_R        (*((volatile unsigned long *)0x400FE638))
#define SYSCTL_RCGCGPIO_R       (*((volatile unsigned long *)0x400FE608))
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08))
#define SYSCTL_PRADC_R          (*((volatile unsigned long *)0x400FEA38))
#define ADC0_PC_R               (*((volatile unsigned long *)0x40038FC4))
#define ADC0_IM_R               (*((volatile unsigned long *)0x40038008))

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

//define port b registers
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_LOCK_R       (*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))

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
#define GPIO_PORTF_DATA_R		  	(*((volatile unsigned long *)0x400253FC)) // define the address of data register
#define GPIO_PORTF_DIR_R 				(*((volatile unsigned long *)0x40025400)) // define the address of direction register
#define GPIO_PORTF_DEN_R				(*((volatile unsigned long *)0x4002551C)) // define the address of digital enable register
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420)) // define the address of analog function register
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510)) // define the address of pull-up resistor register
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520)) // define the address of lock register
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524)) // define the address of control register
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528)) // define the address of analog mode selection register
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C)) // define the address of pctl register
	
//define ADC0 registers
#define ADC0_SSPRI_R          (*((volatile unsigned long *)0x40038020))		// Priority register for sample sequencer (SS)
#define ADC0_ACTSS_R          (*((volatile unsigned long *)0x40038000))		// Activate register for SS
#define ADC0_EMUX_R           (*((volatile unsigned long *)0x40038014))		// Event trigger select register for SS
#define ADC0_SSMUX3_R         (*((volatile unsigned long *)0x400380A0))		// Input source select register for SS
#define ADC0_SSCTL3_R         (*((volatile unsigned long *)0x400380A4))		// Control bits register for SS
#define ADC0_PSSI_R           (*((volatile unsigned long *)0x40038028))		// SS initiate register
#define ADC0_RIS_R            (*((volatile unsigned long *)0x40038004))		// Raw interrupt status for SS
#define ADC0_ISC_R            (*((volatile unsigned long *)0x4003800C))		// Clear SS interrupt condition
#define ADC0_SSFIFO3_R        (*((volatile unsigned long *)0x400380A8))		// ADC result data register for SS
	
// DEFINE SYSTICK REGISTERS
#define NVIC_ST_CTRL_R      		(*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R    		(*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R   		(*((volatile unsigned long *)0xE000E018))
	
// DEFINE INPUTS
#define NEXT 			(GPIO_PORTB_DATA_R & 0x10)
#define SETLOCK		(GPIO_PORTF_DATA_R & 0x10)
#define OPENLOCK 	(GPIO_PORTF_DATA_R & 0x01)



float ui32ADC0Value;		// store ADC result in ui32ADC0Value
float resistance;

void PortA_Init(void);
void PortB_Init(void);
void PortE_Init(void);	
void PortF_Init(void); // define the port f init function
void ADC0_Init(void);
void PLL_Init(void);
unsigned long adcread(void);
void Systick_Init();
void Systick_Wait(unsigned long delay);
void Systick_Wait10ms(unsigned long delay);
void Init(void);

uint32_t potvalue;
uint32_t L[3];
uint32_t S[3];
int state=0;



int main(void){
	
	Init();
	
	
	//CLEAR DATA REGISTERS
	GPIO_PORTA_DATA_R = 0x00;
	while(1){
		
		switch(state){
			
		////////// init phase
			case 0:
				
					Systick_Wait10ms(2);
					if(SETLOCK == 0x00){
						state=1;
					}
				break;
					
			////////// 1st phase	
			case 1:
				
				// lock1
				GPIO_PORTA_DATA_R = 0x80;
				Systick_Wait10ms(100);
				GPIO_PORTA_DATA_R = 0x00;
				Systick_Wait10ms(100);
				if(NEXT == 0x00){
						
					L[0] = adcread()*4095/(2270);
					L[0] /= 100;
					state=2;
				}
				break;
				
				//lock2
			case 2:
				
					GPIO_PORTA_DATA_R = 0xC0;
					Systick_Wait10ms(100);
					GPIO_PORTA_DATA_R = 0x80;
					Systick_Wait10ms(100);
					if(NEXT == 0x00){
						
						L[1] = adcread()*4095/(2270);
						L[1] /= 100;
						state = 3;
					}
					break;
					
				//lock3
			case 3:
				
					GPIO_PORTA_DATA_R = 0xE0;
					Systick_Wait10ms(100);
					GPIO_PORTA_DATA_R = 0xC0;
					Systick_Wait10ms(100);
					if(NEXT == 0x00){
						
						L[2] = adcread()*4095/(2270);
						L[2] /= 100;
						state = 4;
					}
					break;

				//wait for the other phase
			case 4:
				
				Systick_Wait10ms(2);
				GPIO_PORTA_DATA_R = 0xE0;
				if(OPENLOCK==0x00){
					
					state=5;
				}
				break;
				
			///////////2nd phase	
			case 5:
			
				// unlock1
		
				GPIO_PORTA_DATA_R = 0xE0;
				Systick_Wait10ms(100);
				GPIO_PORTA_DATA_R = 0x60;
				Systick_Wait10ms(100);				
				if(NEXT == 0x00){

					S[0] = adcread()*4095/(2270);
					S[0] /= 100;
					if(S[0]==L[0])
						
						state = 6;
				}					
				break;
				
				//unlock2
			case 6:
					
				GPIO_PORTA_DATA_R = 0x60;
				Systick_Wait10ms(100);
				GPIO_PORTA_DATA_R = 0x20;
				Systick_Wait10ms(100);				
				if(NEXT == 0x00){

					S[1] = adcread()*4095/(2270);
					S[1] /= 100;
					if(S[1]==L[1])
						
						state = 7;
				}	
				break;
				
				//unlock3
			case 7:
				
				GPIO_PORTA_DATA_R = 0x20;
				Systick_Wait10ms(100);
				GPIO_PORTA_DATA_R = 0x00;
				Systick_Wait10ms(100);				
				if(NEXT == 0x00){

					S[2] = adcread()*4095/(2270);
					S[2] /= 100;
					if(S[2]==L[2]){
						
						GPIO_PORTA_DATA_R = 0x00;
						while(1){};
					}				
				}
				

				break;
				
		}
	}
	
	
	
	
}

void PortA_Init(void){				       //initialize PortA
	
	volatile unsigned long delay;      // define unsigned long named with delay
  SYSCTL_RCGC2_R |= 0x00000001;      // 1) set the clock for port A
  delay = SYSCTL_RCGC2_R;            // reading register adds a delay   
  GPIO_PORTA_LOCK_R = 0x4C4F434B;    // 2) unlock PortA  
  GPIO_PORTA_CR_R = 0xE0;            // 3) allow changes to PA5 PA6 PA7      
  GPIO_PORTA_AMSEL_R = 0x00;         // 4) disable analog function
  GPIO_PORTA_PCTL_R = 0x00000000;    // 5) GPIO clear bit PCTL  
  GPIO_PORTA_DIR_R = 0xE0;           // 6) PA5 PA6 PA7 OUTPUT   
  GPIO_PORTA_AFSEL_R = 0x00;         // 7) no alternate function
  GPIO_PORTA_PUR_R = 0x00;           // 8) DON'T NEED TO PUR
  GPIO_PORTA_DEN_R = 0xE0;           // 9) DIGITAL ENABLE FOR PA5 PA6 PA7 

}

void PortB_Init(void){				       //initialize PortB
	
	volatile unsigned long delay;      // define unsigned long named with delay
  SYSCTL_RCGC2_R |= 0x00000002;      // 1) set the clock for port B
  delay = SYSCTL_RCGC2_R;            // reading register adds a delay   
  GPIO_PORTB_LOCK_R = 0x4C4F434B;    // 2) unlock PortB
  GPIO_PORTB_CR_R = 0xFF;            // 3) allow changes to PB4    
  GPIO_PORTB_AMSEL_R = 0x00;         // 4) disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;    // 5) GPIO clear bit PCTL  
  GPIO_PORTB_DIR_R = 0x00;           // 6) PORT B INPUT  
  GPIO_PORTB_AFSEL_R = 0x00;         // 7) no alternate function
  GPIO_PORTB_PUR_R = 0xFF;           // 8) USE PUR FOR PORT B
  GPIO_PORTB_DEN_R = 0xFF;           // 9) DIGITAL ENABLE FOR PB4

}

void PortE_Init(void){				       //initialize PortE
	
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000010;	 
	delay = SYSCTL_RCGC2_R;         
  GPIO_PORTE_DIR_R &= 0x00;      
  GPIO_PORTE_AFSEL_R |= 0x08;     
  GPIO_PORTE_DEN_R &= 0x02;      
  GPIO_PORTE_AMSEL_R |= 0x08;
}

void PortF_Init(void){				//initialize PortF
	
	volatile unsigned long delay; // define unsigned long named with delay
  SYSCTL_RCGC2_R |= 0x00000020;      // 1) set the clock for port f
  delay = SYSCTL_RCGC2_R;            // reading register adds a delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x11;           // 3) allow changes to PF0 PF4       
  GPIO_PORTF_AMSEL_R = 0x00;        // 4) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 5) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x00;          // 6) PORT F INPUT
  GPIO_PORTF_AFSEL_R = 0x00;        // 7) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // 8) USE PUR FOR PF0 AND PF4      
  GPIO_PORTF_DEN_R = 0x11;          // 9) enable digital pins PF0 PF4        
}

void ADC0_Init(void){
	
	volatile unsigned long delay;		// define unsigned long named with delay
	SYSCTL_RCGC0_R |= 0x00010000;   // Activate ADC0
	delay = SYSCTL_RCGC2_R;
	SYSCTL_RCGC0_R &= ~0x00000300;  // Set sampling rate to 125K (8th & 9th represents the ADC0 )
	ADC0_SSPRI_R = 0x0123;          // Set sequencer 3 13th and 12th bits correspond to SS3
	ADC0_ACTSS_R &= ~0x0008;   		  // Disable sample sequencer 3 before changes 																					
	ADC0_EMUX_R &= ~0xF000;     		// trigger for SS
	ADC0_SSMUX3_R &= ~0x000F; 			// Choose Ain0 (PE3) as input source
	ADC0_SSMUX3_R += 9;
	ADC0_SSCTL3_R = 0x0006;         // Choose control bits (AIN0)										
	ADC0_ACTSS_R |= 0x0008;   		  // Enable SS3
}


void PLL_Init(){

  SYSCTL_RCC2_R |=  0x80000000;
  SYSCTL_RCC2_R |=  0x00000800; 
  SYSCTL_RCC_R = (SYSCTL_RCC_R &~0x000007C0) + 0x00000540; 
  SYSCTL_RCC2_R &= ~0x00000070;
  SYSCTL_RCC2_R &= ~0x00002000;
  SYSCTL_RCC2_R |= 0x40000000;   
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~ 0x1FC00000) + (9<<22);
  while((SYSCTL_RIS_R&0x00000040)==0){};  
	SYSCTL_RCC2_R &= ~0x00000800;		
}


unsigned long adcread(void){
	
	unsigned long value;
	ADC0_PSSI_R = 0x0008;            
  while((ADC0_RIS_R&0x08)==0){};   
  value = ADC0_SSFIFO3_R&0x0FFF;   
  ADC0_ISC_R = 0x0008;      
	Systick_Wait10ms(2);		
  return value;
}

void Systick_Init(){
  
	NVIC_ST_CTRL_R = 0;          
	NVIC_ST_RELOAD_R = 0x00FFFFFF;
	NVIC_ST_CURRENT_R = 0;
  NVIC_ST_CTRL_R = 0x00000005; 	
}

void Systick_Wait(unsigned long delay){

	NVIC_ST_RELOAD_R = delay-1;  
  NVIC_ST_CURRENT_R = 0;       
  while((NVIC_ST_CTRL_R&0x00010000)==0){};

}

void Systick_Wait10ms(unsigned long delay){
	
	unsigned long i;
  for(i=0; i<delay; i++){
    Systick_Wait(400000); 
  }

}

void Init(void){
	
	PLL_Init();
	PortA_Init();
	PortB_Init();
	PortE_Init();
	PortF_Init();
	Systick_Init();
	ADC0_Init();
}

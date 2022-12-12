#include <stdint.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h" 

//define clock registers
#define SYSCTL_RCC_R            (*((volatile unsigned long *)0x400FE060))
#define SYSCTL_RCC2_R           (*((volatile unsigned long *)0x400FE070)) 
#define SYSCTL_RCGC0_R          (*((volatile unsigned long *)0x400FE100))		// ADC clock
#define SYSCTL_RIS_R            (*((volatile unsigned long *)0x400FE050))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))	

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
	
//definition for ADC0
#define ADC0_SSPRI_R          (*((volatile unsigned long *)0x40038020))		// Priority register for sample sequencer (SS)
#define ADC0_ACTSS_R          (*((volatile unsigned long *)0x40038000))		// Activate register for SS
#define ADC0_EMUX_R           (*((volatile unsigned long *)0x40038014))		// Event trigger select register for SS
#define ADC0_SSMUX3_R         (*((volatile unsigned long *)0x400380A0))		// Input source select register for SS
#define ADC0_SSCTL3_R         (*((volatile unsigned long *)0x400380A4))		// Control bits register for SS
#define ADC0_PSSI_R           (*((volatile unsigned long *)0x40038028))		// SS initiate register
#define ADC0_RIS_R            (*((volatile unsigned long *)0x40038004))		// Raw interrupt status for SS
#define ADC0_ISC_R            (*((volatile unsigned long *)0x4003800C))		// Clear SS interrupt condition
#define ADC0_SSFIFO3_R        (*((volatile unsigned long *)0x400380A8))		// ADC result data register for SS

float ui32ADC0Value;		// store ADC result in ui32ADC0Value
float resistance;

void PortE_Init(void);	
void ADC0_Init(void);
void SetSystemClock_40MHz(void);

int main(void){

ADC0_Init();
SetSystemClock_40MHz();

while(1){
    
ADC0_PSSI_R = 0x0008;            	    //Initiate SS3 seq.

while((ADC0_RIS_R&0x08)==0){};   	    //Wait for finishing conversion
																			
ui32ADC0Value = ADC0_SSFIFO3_R&0xFFF;	//Get ADC result-First 12 bits contain data
			
ADC0_ISC_R = 0x0008;   //Clear interrupt status for SS3 to be able to start sampling again,3rd bit corresponds to Interrupt(IN)3	
resistance = (float)(ui32ADC0Value)*(float)(10000.0/4095.0);        // Convert ADC value to resistance by quantizing	
for(int i=0;i<40000;i++){}; 			   // delay for 1ms
  }
}
void PortE_Init(void){				       //initialize PortE
	
	volatile unsigned long delay;      // define unsigned long named with delay
  SYSCTL_RCC2_R |= 0x00000010;       // 1) set the clock for port E
  delay = SYSCTL_RCC2_R;           	 // reading register adds a delay   
  GPIO_PORTE_LOCK_R = 0x4C4F434B;    // 2) unlock PortE 
  GPIO_PORTE_CR_R = 0x08;            // 3) allow changes to PE3      
  GPIO_PORTE_AMSEL_R = 0x08;         // 4) enable analog mode for pe3
  GPIO_PORTE_PCTL_R = 0x00000000;    // 5) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R 		= 0x00;        // Make PE input (0)   
  GPIO_PORTE_AFSEL_R 	= 0x08;        // Enable analog function for PE3 (1)
  GPIO_PORTE_PUR_R = 0x00;           // 8) disable pull-up resistors on PE     
  GPIO_PORTE_DEN_R 		= 0x00;        // Disable "digital enable" for PE3 (0)     

}

void ADC0_Init(void){
SYSCTL_RCGC0_R = 0x00010000;   // Activate ADC0
SYSCTL_RCGC0_R &= ~0x00000300;  // Set sampling rate to 125K (8th & 9th represents the ADC0 )
ADC0_SSPRI_R = 0x0123;          // Set sequencer 3 13th and 12th bits correspond to SS3
ADC0_ACTSS_R &= 0xFFFFFFF7;     // Disable sample sequencer 3 before changes 																					
ADC0_EMUX_R &= 0xFFFF0FFF;      // trigger for SS
ADC0_SSMUX3_R &= 0xFFFFFFF0; 		// Choose Ain0 (PE3) as input source
ADC0_SSCTL3_R = 0x0006;         // Choose control bits (AIN0)										
ADC0_ACTSS_R |= 0x00000008;     // Enable SS3
}



void SetSystemClock_40MHz(void){  //pll for 16 mhz to 40 mhz 
	// 1. Enable  USERCGC2 in RCGC2
	SYSCTL_RCC2_R |= 0x80000000; 
	// 2. Bypass PLL while initializing
	SYSCTL_RCC2_R |= 0x00000800;
	// 3. Set external crystal value and oscillator source for PLL 
	SYSCTL_RCC_R = (SYSCTL_RCC_R&~0x000007C0)+(0x15<<6);
	// 4. Configure main oscillator source
	SYSCTL_RCC2_R &= ~0x00000070;
	// 5. Clear PWRDN to activate PLL
	SYSCTL_RCC2_R &= ~0x00002000;
	// 6. Set system clock
	SYSCTL_RCC2_R |= 0x20000000;
	// 7. Set system clock divider RCC[28:22]
	SYSCTL_RCC2_R=(SYSCTL_RCC2_R & ~0x1FC00000)+(0x04<<22);
	// 8. Wait for PLL to lock by polling PLLRIS
	while((SYSCTL_RIS_R&0x00000040)==0){};
	// 9. Set BYPASS to 0, select PLL as the source off system clk
	SYSCTL_RCC2_R &= ~0x00000800;
}

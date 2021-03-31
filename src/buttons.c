#include "MKL05Z4.h"                    /* Device header */
#include "buttons.h"
#include "game.h"

typedef enum {false, true} bool; //definicja bool

typedef enum{ BUT1 = 10, BUT2 = 11, BUT3 = 12,  R4 = 5} ButtonType; //przyciski na porcie B

static const IRQn_Type myPORT_IRQn = PORTA_IRQn; 

void buttonsInitialize(void){
	SIM->SCGC5 |=  SIM_SCGC5_PORTA_MASK; 				//wlaczenie zegaru dla portu B
	PORTA->PCR[BUT1] |= PORT_PCR_MUX(1);      	//piny jako gpio
	PORTA->PCR[BUT2] |= PORT_PCR_MUX(1);      	
	PORTA->PCR[BUT3] |= PORT_PCR_MUX(1);      	
	PORTA->PCR[R4] |= PORT_PCR_MUX(1);      	
	
	PORTA->PCR[BUT1] |=  PORT_PCR_PE_MASK |		//wlacz pull up rezystor
											 PORT_PCR_PS_MASK;	    //wybierz pull up rezystor
	PORTA->PCR[BUT1] |= 	PORT_PCR_IRQC(0xA);		//zbocze opadajace dla przerwan portu B
	
	PORTA->PCR[BUT2] |=  PORT_PCR_PE_MASK |		
											 PORT_PCR_PS_MASK;			
	PORTA->PCR[BUT2] |=  PORT_PCR_IRQC(0xA);	 
	
	PORTA->PCR[BUT3] |=  PORT_PCR_PE_MASK |		
											 PORT_PCR_PS_MASK;			
	PORTA->PCR[BUT3] |=  PORT_PCR_IRQC(0xA);	
	
	PTA->PDDR |= (1<<R4); //wyjscie
	PTA->PDOR &= ~(1<<R4); //0
		
	/* ARM's Nested Vector Interrupt Controller configuration */
	NVIC_ClearPendingIRQ(myPORT_IRQn);				//wyczysc NVIC i przychodzace przerwania portu B
	NVIC_EnableIRQ(myPORT_IRQn);							//wlacz przerwania nvic dla portu B/
	NVIC_SetPriority (myPORT_IRQn, 3);			//priorytet przerwan

}


//przerwanie przyciskow
void PORTA_IRQHandler(void){

	if( PORTA->ISFR & (1 << BUT1) ){        //Check in ISFR register if button BUT1 is pressed
		S2();
    while((FPTA->PDIR&(1<<BUT1))==0);				//Enable wait in the interrupt for SW1 button release
		 PORTA->PCR[BUT1] |= PORT_PCR_ISF_MASK; //Make sure that interrupt service flag (ISF) in Port Control Register is cleared during ISR execution
  }
	
	if( PORTA->ISFR & (1 << BUT2) ){          /* Check in ISFR register if button BUT2 is pressed */
		S3();
    while((FPTA->PDIR&(1<<BUT2))==0);				
		 PORTA->PCR[BUT2] |= PORT_PCR_ISF_MASK; 
	}
	
	if( PORTA->ISFR & (1 << BUT3) ){          /* Check in ISFR register if button BUT3 is pressed */
		S4();
    while((FPTA->PDIR&(1<<BUT3))==0);				
		 PORTA->PCR[BUT3] |= PORT_PCR_ISF_MASK; 
  }
}

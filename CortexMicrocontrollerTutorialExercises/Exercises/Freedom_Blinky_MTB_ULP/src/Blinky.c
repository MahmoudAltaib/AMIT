
#include <MKL25Z4.H>

#define RED_PIN			(1 << 18)
#define GREEN_PIN		(1 << 19)
#define BLUE_PIN		(1 << 1)

#define RED_OFF			(PTB->PSOR = RED_PIN)
#define RED_ON			(PTB->PCOR = RED_PIN)
#define RED_TOGGLE		(PTB->PTOR = RED_PIN)

#define GREEN_OFF		(PTB->PSOR = GREEN_PIN)
#define GREEN_ON		(PTB->PCOR = GREEN_PIN)
#define GREEN_TOGGLE	(PTB->PTOR = GREEN_PIN)

#define BLUE_OFF		(PTD->PSOR = BLUE_PIN)
#define BLUE_ON			(PTD->PCOR = BLUE_PIN)
#define BLUE_TOGGLE		(PTD->PTOR = BLUE_PIN)

//global tick count
static uint32_t TICK;

void SysTick_Handler()
{
	TICK++;
}

//initializes PortB pins 18 and 19 and PortD pin 1
//for the Red, Green, and Blue leds respectivly 
void init_leds()
{
	//turn on clock to portB
	SIM->SCGC5 |= (SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK);
	
	//set multiplexer to GPIO
	PORTB->PCR[18] |= (PORT_PCR_MUX_MASK & 0x100);
	PORTB->PCR[19] |= (PORT_PCR_MUX_MASK & 0x100);
	PORTD->PCR[1]  |= (PORT_PCR_MUX_MASK & 0x100);
	
	//set output to high
	PTB->PSOR |= (RED_PIN | GREEN_PIN);
	PTD->PSOR |= (BLUE_PIN);
	
	//set pins to output
	PTB->PDDR |= (RED_PIN | GREEN_PIN);
	PTD->PDDR |= (BLUE_PIN);
}

//rotates through different led combos
void change_leds()
{
static uint32_t state = 2;
	
	switch (state++)	
	{
		case 2:
			RED_ON;
			break;
		case 3:
			RED_OFF;
			GREEN_ON;
			break;
	  case 5:
			GREEN_OFF;
			BLUE_ON;
			break;
		case 7:
			RED_ON;
			break;
		case 11:
			GREEN_ON;
			BLUE_OFF;
			break;
		case 13:
			RED_OFF;
			BLUE_ON;
			break;
	 case 17:
			RED_ON;
			break;
	 default:
			GREEN_OFF;
			BLUE_OFF;
			state = 2;
			break;
	}

}

int main(void)
{
uint32_t local_tick = 0;
	
	SysTick_Config(SystemCoreClock/100);       // Generate interrupt each 10 ms 
	init_leds();				
	
	while(1)
	{
	//update leds every half second
		if(TICK - local_tick > 5)	
		{
			change_leds();
			local_tick = TICK;
		}
		__WFI();
	}
}	





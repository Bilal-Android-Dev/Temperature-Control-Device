#include <stdint.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "Header.h"

//LCD
void GPIO_PortAB_Init(void){
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000001;
    delay = SYSCTL_RCGC2_R;
    GPIO_PORTA_AMSEL_R &= ~0xE0;
    GPIO_PORTA_PCTL_R &= ~0xFFF00000;
    GPIO_PORTA_DIR_R |= 0xE0;
    GPIO_PORTA_AFSEL_R &= ~0xE0;
    GPIO_PORTA_DEN_R |= 0xE0;
    GPIO_PORTA_DR8R_R |= 0xE0;
    SYSCTL_RCGC2_R |= 0x00000002;
    delay = SYSCTL_RCGC2_R;
    GPIO_PORTB_AMSEL_R &= ~0xFF;
    GPIO_PORTB_PCTL_R &= ~0xFFFFFFFF;
    GPIO_PORTB_DIR_R |= 0xFF;
    GPIO_PORTB_AFSEL_R &= ~0xFF;
    GPIO_PORTB_DEN_R |= 0xFF;
    GPIO_PORTB_DR8R_R |= 0xFF;
}


void PortD_Init(void){
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000008;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTD_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0x0F;
	GPIO_PORTF_AMSEL_R = 0x00;
	GPIO_PORTF_PCTL_R = 0x00000000;
	GPIO_PORTF_DIR_R = 0xFF;
	GPIO_PORTF_AFSEL_R = 0x00;
	GPIO_PORTB_DR8R_R |= 0xFF;
	GPIO_PORTF_DEN_R = 0xFF;
}


void Write_Command(unsigned char LCD_Comment){
 
		GPIO_PORTA_DATA_R &= ~(RS+RW+E);
    GPIO_PORTB_DATA_R = LCD_Comment;
	
		
	
		GPIO_PORTD_DATA_R = (LCD_Comment & 0xCC);
    GPIO_PORTA_DATA_R |= E;
    GPIO_PORTA_DATA_R &= ~(RS+RW);
    SysTick_Delay(19);
    GPIO_PORTA_DATA_R &= ~(RS+RW+E);
    Delay1ms(1);
}

void Write_Data(unsigned char LCD_Data){
		
	
    GPIO_PORTB_DATA_R = LCD_Data;
	

    
		GPIO_PORTA_DATA_R |= RS+E;
    GPIO_PORTA_DATA_R &= ~RW;
    SysTick_Delay(19);
    GPIO_PORTA_DATA_R &= ~(RS+RW+E);
    Delay1ms(1);
}

void LCD_Init(){
    GPIO_PortAB_Init();
		PortD_Init();
    Delay1ms(15);
    Write_Command(0x38);
    Delay1ms(5);
    Write_Command(0x38);
    SysTick_Delay1us(150);
    Write_Command(0x0C);
    Write_Command(0x01);
    Write_Command(0x06);
    Delay1ms(50);
}

void LCD_Clear(){
    Write_Command(0x01);
}

void LCD_DisplayString(unsigned char *str){
    while(*str != 0){
        Write_Data(*str++);
    }
}

void LCD_DisplayChar(unsigned char CHAR){
    Write_Data(CHAR);
}

void LCD_DisplayDec(unsigned int number){
    if(number >=10){
        LCD_DisplayDec(number/10);
        number = number%10;

    }
    LCD_DisplayChar(number+'0');
}

void LCD_DisplayPosition(unsigned char Line,unsigned int digit){
    Write_Command(Line + digit);
}

int fputc(int ch, FILE *f){
    if((ch == 10) || (ch == 13) || (ch == 27)){
        LCD_DisplayChar(13);
        LCD_DisplayChar(10);
        return 1;
    }
    LCD_DisplayChar(ch);
    return 1;
}


//END LCD

//PLL
void PLL_Init(void)
{
	SYSCTL_RCC2_R |= 0x80000000;
	SYSCTL_RCC2_R |= 0x00000800;
	SYSCTL_RCC_R = (SYSCTL_RCC_R &~0x000007C0)
		+ 0x00000540;
	SYSCTL_RCC2_R &= ~0x00000070;
	SYSCTL_RCC2_R &= ~0x00002000;
	SYSCTL_RCC2_R |= 0x40000000;
	SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~0x1FC00000)
		+ (4 << 22);
	while ((SYSCTL_RIS_R & 0x00000040) == 0){};
	SYSCTL_RCC2_R &= ~0x00000800;
}
//END PLL

//SysTick
void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_CTRL_R = 0x00000005;
}


void SysTick_Delay(unsigned long delay){
	NVIC_ST_RELOAD_R = delay - 1;
	NVIC_ST_CURRENT_R = 0;
	while ((NVIC_ST_CTRL_R & 0x00010000) == 0){
	}
}


void SysTick_Delay1us(unsigned long delay){
	unsigned long i;
	for (i = 0; i<delay; i++)    {
		SysTick_Delay(80);                 // wait 1us
	}
}


void Delay1ms(unsigned long delay){
	unsigned long i;
	for (i = 0; i<delay; i++)    {
		SysTick_Delay(80000);              // wait 1ms
	}
}
//END SysTick

//ADC
void ADC_Init(void){
	volatile unsigned int delay;
	// 1) activate clock for Port E
	SYSCTL_RCGCGPIO_R |= 0x10;
	while ((SYSCTL_PRGPIO_R & 0x10) != 0x10){};
	delay = SYSCTL_RCGCGPIO_R;      //   allow time for clock to stabilize
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTE_DIR_R &= ~0x10;      // 2) make PE4 input
	GPIO_PORTE_AFSEL_R |= 0x10;     // 3) enable alternate function on PE4
	GPIO_PORTE_DEN_R &= ~0x10;      // 4) disable digital I/O on PE4
	GPIO_PORTE_AMSEL_R |= 0x10;     // 5) enable analog functionality on PE4
	SYSCTL_RCGCADC_R |= 0x0001;   // 7) activate ADC0
	while ((SYSCTL_PRADC_R & 0x0001) != 0x0001){};    // good code, but not yet implemented in simulator
	delay = SYSCTL_RCGCADC_R;         // extra time for clock to stabilize
	delay = SYSCTL_RCGCADC_R;         // extra time for clock to stabilize
	delay = SYSCTL_RCGCADC_R;         // extra time for clock to stabilize
	delay = SYSCTL_RCGCADC_R;

	ADC0_PC_R &= ~0xF;              // 7) clear max sample rate field
	ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
	ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
	ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
	ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
	ADC0_SSMUX3_R &= ~0x000F;       // 11) clear SS3 field
	ADC0_SSMUX3_R += 9;             //    set channel
	ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
	ADC0_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
	ADC0_ACTSS_R |= 0x0008;         // 14) enable sample sequencer 3
}

unsigned int ADC_Data(void){
	unsigned int result;
	ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
	while ((ADC0_RIS_R & 0x08) == 0){};   // 2) wait for conversion done
	// if you have an A0-A3 revision number, you need to add an 8 usec wait here
	result = ADC0_SSFIFO3_R & 0xFFF;   // 3) read result
	ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
	return result;
}

int ADC_Value(void){
	int result = 0;
	double stat = 0;
	result = ADC_Data();
	stat = (double)result/12.412121;
	if((stat - (int)stat) >= 0.500){
		return (result+1);
	}
	else{
		return result;
	}
}
//ADC END

//Interrupt
void PortF_Init(void){
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000020;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0x1F;
	GPIO_PORTF_AMSEL_R = 0x00;
	GPIO_PORTF_PCTL_R = 0x00000000;
	GPIO_PORTF_DIR_R = 0x0E;
	GPIO_PORTF_AFSEL_R = 0x00;
	GPIO_PORTF_PUR_R = 0x11;
	GPIO_PORTF_DEN_R = 0x1F;
}

void PortF_Interrupt(void){
	SYSCTL_RCGC2_R |= 0x00000020;
	DisableInterrupts();
	GPIO_PORTF_IS_R &= ~0x11;   // Sw1 and Sw2 0x11 = 10001 is edge-sensitive (default setting)
	GPIO_PORTF_IBE_R &= ~0x11;  // Is not both edges (default setting)
	GPIO_PORTF_IEV_R &= ~0x11;;   // Rising edge event
  GPIO_PORTF_ICR_R = 0x11;    // clear flag4
  GPIO_PORTF_IM_R |= 0x11;    // enable interrupt
	NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | 0x00200000;
	NVIC_EN0_R = 0x40000000;
	EnableInterrupts();
}

//End Interrupt

//TEST and DRIVE
int Test(unsigned int user, unsigned int adc){
		if(user <= adc){
		return 1;
		}
		else{
		return 0;
		}
}
void TestS(unsigned int user, unsigned int adc){
	if(user <= adc){
		GPIO_PORTF_DATA_R = 0x02;
		Delay1ms(10);
		LCD_DisplayPosition(LINE1,2);
		printf("Notification");
		Delay1ms(100);
		LCD_DisplayPosition(LINE2,3);
		printf("Fan is ON!");
		Delay1ms(2500);
		LCD_Clear();
		Delay1ms(10);
		}
	if(user > adc){
		GPIO_PORTF_DATA_R = 0x00;
		Delay1ms(10);
		LCD_DisplayPosition(LINE1,2);
		printf("Notification");
		Delay1ms(100);
		LCD_DisplayPosition(LINE2,3);
		printf("Fan is OFF!");
		Delay1ms(2500);
		LCD_Clear();
		Delay1ms(10);
		}
}

void Drive(int Current_State, int Pre_State){
	if(Current_State == 1 && Pre_State == 0){
		GPIO_PORTF_DATA_R = 0x02;
		Delay1ms(10);
		LCD_DisplayPosition(LINE1,2);
		printf("Notification");
		Delay1ms(100);
		LCD_DisplayPosition(LINE2,3);
		printf("Fan is ON!");
		Delay1ms(2500);
		LCD_Clear();
		Delay1ms(10);
	}
	if(Current_State == 0 && Pre_State == 1){
		GPIO_PORTF_DATA_R = 0x00;
		Delay1ms(10);
		LCD_DisplayPosition(LINE1,2);
		printf("Notification");
		Delay1ms(100);
		LCD_DisplayPosition(LINE2,3);
		printf("Fan is OFF!");
		Delay1ms(2500);
		LCD_Clear();
		Delay1ms(10);
	}
}

int GetState(int Adc_Value, int User_Value){
	Adc_Value = ADC_Data();
	if(User_Value <= Adc_Value){
	return 1;
	}
	else{
	return 0;
	}
}

void Init_Intro(void){
	PLL_Init();
	Delay1ms(10);
	SysTick_Init();
	Delay1ms(10);
	PortF_Init();
	PortF_Interrupt();
	Delay1ms(10);
	LCD_Init();
	Delay1ms(10);
	ADC_Init();
	
	Delay1ms(10);
	LCD_DisplayPosition(LINE1,0);
	printf("ELECTRICAL ENGG.");
	Delay1ms(1000);
	LCD_DisplayPosition(LINE2,5);
	printf("EE371");
	Delay1ms(2000);
	LCD_Clear();
	
	Delay1ms(10);
	LCD_DisplayPosition(LINE1,1);
	printf("MICROPROCESSOR");
	Delay1ms(1000);
	LCD_DisplayPosition(LINE2,4);
	printf("PROJECT");
	Delay1ms(2000);
	LCD_Clear();

	Delay1ms(10);
	LCD_DisplayPosition(LINE1,2);
	printf("Temperature");
	Delay1ms(1000);
	LCD_DisplayPosition(LINE2,1);
	printf("Ctrl Fan/Device");
	Delay1ms(2000);
	LCD_Clear();

	Delay1ms(10);
	LCD_DisplayPosition(LINE1,0);
	printf("By: BILAL AHMED");
	Delay1ms(1000);
	LCD_DisplayPosition(LINE2,2);
	printf("2014-EE-366");
	Delay1ms(2000);
	LCD_Clear();

	Delay1ms(10);
	LCD_DisplayPosition(LINE1,2);
	printf("MOIN AKHTAR");
	Delay1ms(1000);
	LCD_DisplayPosition(LINE2,3);
	printf("UMER ALYAN");
	Delay1ms(1000);
	LCD_Clear();

	Delay1ms(10);
	LCD_DisplayPosition(LINE1,0);
	printf("To: Sir ");
	Delay1ms(1000);
	LCD_DisplayPosition(LINE2,2);
	printf("M. FAHAD IJAZ");
	Delay1ms(2000);
	LCD_Clear();
	Delay1ms(10);
}

//END TEST and DRIVE

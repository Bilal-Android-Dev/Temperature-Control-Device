#include <stdint.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "Header.h"

int Adc_Value;
int User_Value = 25;

int main(void){
		PLL_Init();
		SysTick_Init();
		PortF_Init();
		PortF_Interrupt();
		LCD_Init();
		ADC_Init();
		
		SysTick_Delay1ms(10);
		LCD_DisplayPosition(LINE1,1);
		printf("MICROPROCESSOR");
		SysTick_Delay1ms(1000);
		LCD_DisplayPosition(LINE2,4);
		printf("PROJECT");
		SysTick_Delay1ms(2000);
		LCD_Clear();
		
		SysTick_Delay1ms(10);
		LCD_DisplayPosition(LINE1,2);
		printf("Temperature");
		SysTick_Delay1ms(1000);
		LCD_DisplayPosition(LINE2,1);
		printf("Ctrl Fan/Device");
		SysTick_Delay1ms(2000);
		LCD_Clear();
	
		SysTick_Delay1ms(10);
		LCD_DisplayPosition(LINE1,0);
		printf("By: BILAL AHMED");
		SysTick_Delay1ms(1000);
		LCD_DisplayPosition(LINE2,2);
		printf("2014-EE-366");
		SysTick_Delay1ms(2000);
		LCD_Clear();
		
		SysTick_Delay1ms(10);
		LCD_DisplayPosition(LINE1,2);
		printf("MOIN AKHTAR");
		SysTick_Delay1ms(1000);
		LCD_DisplayPosition(LINE2,3);
		printf("UMER ALYAN");
		SysTick_Delay1ms(1000);
		LCD_Clear();
		
		SysTick_Delay1ms(10);
		LCD_DisplayPosition(LINE1,0);
		printf("To: Sir ");
		SysTick_Delay1ms(1000);
		LCD_DisplayPosition(LINE2,4);
		printf("FAHID IJAZ");
		SysTick_Delay1ms(2000);
		LCD_Clear();
		
			
while(1){
			Adc_Value = ADC_Data();
			Adc_Value /= 10;
	
			SysTick_Delay1ms(10);
			LCD_DisplayPosition(LINE1,0); 
			printf("NOW TEMP is %d C",Adc_Value);
			SysTick_Delay1ms(2000);
			LCD_DisplayPosition(LINE2,1); 
			printf("USER SET %d C",User_Value);
			SysTick_Delay1ms(2000);
			LCD_Clear();
			SysTick_Delay1ms(10);
			Test(User_Value, Adc_Value);
    }
}

void GPIOPortF_Handler(void){
	if ((GPIO_PORTF_DATA_R & 0x10) == 0){
		User_Value++;
		LCD_Clear();
		LCD_DisplayPosition(LINE1, 1);
		printf("USER SET ++ %d C", User_Value);
		SysTick_Delay1ms(500);
		GPIO_PORTF_ICR_R = 0x10;
	}
	if ((GPIO_PORTF_DATA_R & 0x01) == 0){
		User_Value--;
		LCD_Clear(); LCD_DisplayPosition(LINE1, 1);
		printf("USER SET ++ %d C", User_Value);
		SysTick_Delay1ms(500);
		GPIO_PORTF_ICR_R = 0x01;
	}
}


#include <stdint.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "Header.h"

int Adc_Value, Pre_State, Current_State = 0, User_Value = 25;

int main(void){
	Init_Intro();
	Adc_Value = ADC_Value();
	Pre_State = GetState(Adc_Value, User_Value);
	TestS(User_Value, Adc_Value);

	while (1){
		Adc_Value = ADC_Value();
		Current_State = Test(User_Value, Adc_Value);
		Repeat_Display();
		Drive(Current_State, Pre_State);
		Pre_State = Current_State;
	}
}

void GPIOPortF_Handler(void){
	if ((GPIO_PORTF_DATA_R & 0x10) == 0){
		User_Value++;
		LCD_Clear();
		LCD_DisplayPosition(LINE1, 1);
		printf("USER SET ++ %d C", User_Value);
		Delay1ms(500);
		GPIO_PORTF_ICR_R = 0x10;
	}
	if ((GPIO_PORTF_DATA_R & 0x01) == 0){
		User_Value--;
		LCD_Clear(); LCD_DisplayPosition(LINE1, 1);
		printf("USER SET -- %d C", User_Value);
		Delay1ms(500);
		GPIO_PORTF_ICR_R = 0x01;
	}
}
void Repeat_Display(void){
	Delay1ms(10);
	LCD_DisplayPosition(LINE1, 0);
	printf("NOW TEMP is %d C", Adc_Value);
	Delay1ms(10);
	LCD_DisplayPosition(LINE2, 0);
	if (Current_State == 1){
		printf("USER %d C FAN-ON", User_Value);
	}
	else{
		printf("USER %dC FAN-OFF", User_Value);
	}
	Delay1ms(3000);
	LCD_Clear();
	Delay1ms(3);
}


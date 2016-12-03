#define LINE1 0x80 //First Line of LCD
#define LINE2 0xC0 //Second Line of LCD
#define RW 0x20  //PA5
#define RS 0x40  //PA6 
#define E  0x80  //PA7

//LCD
void LCD_Init(void);
void LCD_Clear(void);
void LCD_DisplayString(unsigned char *str);
void LCD_DisplayChar(unsigned char CHAR);
void LCD_DisplayDec(unsigned int number);
void LCD_DisplayPosition(unsigned char Line, unsigned int digit);

//PLL
void PLL_Init(void);

//SYSTICK
void SysTick_Init(void);
void SysTick_Delay(unsigned long delay);
void SysTick_Delay1us(unsigned long delay);
void Delay1ms(unsigned long delay);

//BUTTON INTERRUPT
void PortF_Interrupt(void);
void DisableInterrupts(void);
void EnableInterrupts(void);
void PortF_Init(void);

//ADC
unsigned int ADC_Data(void);
void ADC_Init(void);
int ADC_Value(void);

//TEST AND DRIVE
void Init_Intro(void);
int Test(unsigned int user, unsigned int adc);
int GetState(int Adc_Value, int User_Value);
void Drive(int Current_State, int Pre_State);
void TestS(unsigned int user, unsigned int adc);
void Repeat_Display(void);

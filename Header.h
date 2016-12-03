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


void PLL_Init(void);


void SysTick_Init(void);
void SysTick_Delay(unsigned long delay); //12.ns
void SysTick_Delay1us(unsigned long delay); //1us
void SysTick_Delay1ms(unsigned long delay); //1ms


void PortF_Interrupt(void);
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);
void PortF_Init(void);

unsigned int ADC_Data(void);
void ADC_Init(void);

void Test(unsigned int user, unsigned int adc);

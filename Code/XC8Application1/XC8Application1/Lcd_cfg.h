#ifndef INC_Lcd_cfg_H_
#define INC_Lcd_cfg_H_

#include "dio.h"


#define LCD_PIN_RS DIO_PORTD,DIO_PIN2
#define LCD_PIN_RW DIO_PORTD,DIO_PIN1
#define LCD_PIN_EN DIO_PORTD,DIO_PIN0

#define LCD_PIN_D0 DIO_PORTC,DIO_PIN0
#define LCD_PIN_D1 DIO_PORTC,DIO_PIN1
#define LCD_PIN_D2 DIO_PORTC,DIO_PIN2
#define LCD_PIN_D3 DIO_PORTC,DIO_PIN3
#define LCD_PIN_D4 DIO_PORTC,DIO_PIN4
#define LCD_PIN_D5 DIO_PORTC,DIO_PIN5
#define LCD_PIN_D6 DIO_PORTC,DIO_PIN6
#define LCD_PIN_D7 DIO_PORTC,DIO_PIN7

#define LCD_DISPLAY_STATUS LCD_DISPLAY_ON
#define LCD_CURSOR_STATUS LCD_CURSOR_ON_BLINK
#define LCD_LINES_STATUS LCD_TWOLINES
#define LCD_FONT_STATUS LCD_FONT_5x7

#define LCD_Contrast_pin DIO_PORTB,DIO_PIN3

#endif

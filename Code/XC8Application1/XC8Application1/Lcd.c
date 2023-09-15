#include "Lcd.h"
#include "Lcd_cfg.h"
#include "Dio.h"
#include "cpu_freq.h"
#include <util/delay.h>
#include "Macros.h"
#include <stdarg.h>
#include "Utils.h"
#include <avr/io.h>

/*		Pins Initialization		*/
static void LCD_Init_Pins();
static void LCD_SendData(u8 data);
void LCD_Init()
{
	LCD_DisplayControlType command = { LCD_DISPLAY_ON,LCD_CURSOR_ON_BLINK,LCD_TWOLINES,LCD_FONT_5x7 };
	LCD_Init_Pins();
	LCD_DisplayControl(command);
	/*LCD_SendCommand(0b00111000);
	LCD_SendCommand(0b00001111);
	LCD_SendCommand(0b00000001);*/
	LCD_ClearDisplay();

	Dio_SetPinMode(LCD_Contrast_pin, DIO_PIN_OUTPUT);
	LCD_ContrastChange(70);
}


void LCD_SendCommand(u8 command) {
	Dio_SetPinMode(LCD_PIN_RS, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_RW, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_EN, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D0, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D1, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D2, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D3, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D4, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D5, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D6, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D7, DIO_PIN_OUTPUT);

	Dio_SetPinLevel(LCD_PIN_RS, STD_LOW);
	Dio_SetPinLevel(LCD_PIN_RW, STD_LOW);
	Dio_SetPinLevel(LCD_PIN_D0, GET_BIT(command, 0));
	Dio_SetPinLevel(LCD_PIN_D1, GET_BIT(command, 1));
	Dio_SetPinLevel(LCD_PIN_D2, GET_BIT(command, 2));
	Dio_SetPinLevel(LCD_PIN_D3, GET_BIT(command, 3));
	Dio_SetPinLevel(LCD_PIN_D4, GET_BIT(command, 4));
	Dio_SetPinLevel(LCD_PIN_D5, GET_BIT(command, 5));
	Dio_SetPinLevel(LCD_PIN_D6, GET_BIT(command, 6));
	Dio_SetPinLevel(LCD_PIN_D7, GET_BIT(command, 7));
	Dio_SetPinLevel(LCD_PIN_EN, STD_HIGH);
	_delay_ms(2);
	Dio_SetPinLevel(LCD_PIN_EN, STD_LOW);
};

void LCD_SendData(u8 data) {
	LCD_Init_Pins();
	Dio_SetPinLevel(LCD_PIN_RS, STD_HIGH);
	Dio_SetPinLevel(LCD_PIN_RW, STD_LOW);
	Dio_SetPinLevel(LCD_PIN_D0, GET_BIT(data, 0));
	Dio_SetPinLevel(LCD_PIN_D1, GET_BIT(data, 1));
	Dio_SetPinLevel(LCD_PIN_D2, GET_BIT(data, 2));
	Dio_SetPinLevel(LCD_PIN_D3, GET_BIT(data, 3));
	Dio_SetPinLevel(LCD_PIN_D4, GET_BIT(data, 4));
	Dio_SetPinLevel(LCD_PIN_D5, GET_BIT(data, 5));
	Dio_SetPinLevel(LCD_PIN_D6, GET_BIT(data, 6));
	Dio_SetPinLevel(LCD_PIN_D7, GET_BIT(data, 7));
	Dio_SetPinLevel(LCD_PIN_EN, STD_HIGH);
	_delay_ms(2);
	Dio_SetPinLevel(LCD_PIN_EN, STD_LOW);
};


static void LCD_Init_Pins() {
	Dio_SetPinMode(LCD_PIN_RS, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_RW, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_EN, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D0, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D1, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D2, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D3, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D4, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D5, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D6, DIO_PIN_OUTPUT);
	Dio_SetPinMode(LCD_PIN_D7, DIO_PIN_OUTPUT);
};

void LCD_DisplayChar(u8 data)
{
	LCD_SendData(data);
}

void LCD_DisplayString(const u8* data)
{
	while (*data != '\0')
	{
		LCD_SendData(*data);
		data++;
	}
};

void LCD_DisplayNumber(s32 num)
{
	u32 reversed = 0;
	u8 c = 0;
	u32 data;

	if (num < 0)
	{
		LCD_SendData('-');
		data = -num;
	}
	else if (num >= 0)
	{
		data = num;
	}

	do {
		reversed = (reversed * 10) + data % 10;
		data /= 10;
		c++;
	} while (data != 0);

	do {
		LCD_SendData('0' + (reversed % 10));
		reversed /= 10;
		c--;
	} while (c != 0);
};

void LCD_ClearDisplay()
{
	LCD_SendCommand(0b00000001);
}

void LCD_SetCursorPostion(u8 X, u8 Y)
{
	/****************************************************/
	/*__________________________________________________*/
	/*|                                                |0*/
	/*|                                                |1*/
	/*|________________________________________________| */
	/*    0        1        2        3        4         */
	/****************************************************/
	if (X>0 && X<=16)
		{
		    switch(Y)
		    {
			    case 1:
					LCD_SendCommand(X+127);
			    break;
			    case 2:
					LCD_SendCommand(X+191);
			    break;
			    default:
			    break;
		    }
		}
}

void LCD_ShiftDisplay(LCD_ShiftDirectionType Direction, u8 num)
{

	for (int i = 0; i < num; i++)
	{
		switch (Direction)
		{
		case LCD_SHIFT_RIGHT:
			LCD_SendCommand(0b00011100);//this one for the direction
		//	LCD_SendCommand(0b00011100);
			break;
		case LCD_SHIFT_LEFT:
		//	LCD_SendCommand(0b00011000);
			LCD_SendCommand(0b00011000);//this one for the direction
			break;
		default:
			break;
		}
	}
}

void LCD_ReturnHome()
{
	LCD_SendCommand(0b00000010);
}

void LCD_DisplayControl(LCD_DisplayControlType controls)
{
	if (controls.Display_Status == LCD_DISPLAY_ON)
	{
		switch (controls.Cursor_Status) //LCD_SendCommand(0b00001DCB);
		{
		case LCD_CURSOR_OFF:
			LCD_SendCommand(0b00001100);
			break;
		case LCD_CURSOR_ON_BLINK:
			LCD_SendCommand(0b00001111);
			break;
		case LCD_CURSOR_ON_NOT_BLINK:
			LCD_SendCommand(0b00001110);
			break;
		default:
			break;
		}

		if (controls.Lines_Status == LCD_ONELINE)
		{
			if (controls.Font_Status == LCD_FONT_5x7)
			{
				LCD_SendCommand(0b00110000);//LCD_SendCommand(0b0011NF00);
			}
			else if (controls.Font_Status == LCD_FONT_5x10)
			{
				LCD_SendCommand(0b00110100);//LCD_SendCommand(0b0011NF00);
			}
		}
		else if (controls.Lines_Status == LCD_TWOLINES)
		{
			if (controls.Font_Status == LCD_FONT_5x7)
			{
				LCD_SendCommand(0b00111000);//LCD_SendCommand(0b0011NF00);
			}
			else if (controls.Font_Status == LCD_FONT_5x10)
			{
				LCD_SendCommand(0b00111100);//LCD_SendCommand(0b0011NF00);
			}
		}
	}
	else if (controls.Display_Status == LCD_DISPLAY_OFF)
	{
		LCD_SendCommand(0b00001000);
	}

}

void LCD_SaveSpecialCharacter(u8* data, u8 location) {
    u8 counter;
    if (location < 8) {
        u8 address;
        address = location*8;
        SET_BIT(address, 6);
        LCD_SendCommand(address);
        for (counter = 0; counter < 8; counter++) {
            LCD_SendData(data[counter]);
        }
    }
}

void LCD_Print(const u8* str, ...) {
	u32 i = 0;
	va_list valist;
	va_start(valist, str);
	while (str[i] != '\0')
	{
		if (str[i] == '%') {
			u8 arr[32];
			u8 count;
			i++;
			switch (str[i])
			{
			case 'c':
				LCD_SendData((u8)va_arg(valist, int));
				break;
			case 'd':
				LCD_DisplayNumber(va_arg(valist, int));
				break;
			case 'u':
				if (str[i + 1] == 'l') {
					i++;
					LCD_DisplayNumber(va_arg(valist, unsigned long));
				}
				else {
					LCD_DisplayNumber(va_arg(valist, unsigned int));
				}
				break;
			case 'l':
				LCD_DisplayNumber(va_arg(valist, long));
				break;
			case 'x':
				if (str[i + 1] == 'l') {
					i++;
					count = Utils_NumberToHex(va_arg(valist, unsigned long), arr);
				}
				else {
					count = Utils_NumberToHex(va_arg(valist, unsigned int), arr);
				}
				for (u8 j = count; j > 0; j--) {
					LCD_SendData(arr[j]);
				}
				LCD_SendData(arr[0]);
				break;
			case 'b':
				if (str[i + 1] == 'l') {
					i++;
					count = Utils_NumberToBin(va_arg(valist, unsigned long), arr);
				}
				else {
					count = Utils_NumberToBin(va_arg(valist, unsigned int), arr);
				}
				for (u8 j = count; j > 0; j--) {
					LCD_SendData(arr[j]);
				}
				LCD_SendData(arr[0]);
				break;
			default:
				if (str[i] == '\0') {
					i--;
				}
				else {
					LCD_SendData(str[i]);
				}
				break;
			}
		}
		else {
			LCD_SendData(str[i]);
		}
		i++;
	}
	va_end(valist);
}

void LCD_ContrastChange(u8 duty)
{
	int lvl = 0;
	lvl = duty * 256 / 100;
	OCR0 = lvl;//duty
	TCCR0 = 0b01111001;//fast pwm, prescaler 1,set on compare match
}

#define C_DisplayDefaultDigits_U8            0xffu // Will display the exact digits in the number
#define C_MaxDigitsToDisplay_U8              10u   // Max decimal/hexadecimal digits to be displayed
#define C_NumOfBinDigitsToDisplay_U8         16u   // Max bits of a binary number to be displayed
#define C_MaxDigitsToDisplayUsingPrintf_U8   C_DisplayDefaultDigits_U8 /* Max dec/hexadecimal digits to be displayed using printf */
#include "stdutils.h"

#define M_LcdClearBit(x)   util_BitClear((M_LcdControlBus),(x))
#define M_LcdSetBit(x)     util_BitSet((M_LcdControlBus),(x))

void LCD_DisplayDecimalNumber(uint32_t var_DecNumber_u32, uint8_t var_numOfDigitsToDisplay_u8)
{
	uint8_t i = 0, a[10];

	if (var_DecNumber_u32 == 0)
	{
		/* If the number is zero then update the array with the same for displaying */
		for (i = 0; ((i < var_numOfDigitsToDisplay_u8) && (i < C_MaxDigitsToDisplay_U8)); i++)
			a[i] = 0x00;
	}
	else
	{
		for (i = 0; i < var_numOfDigitsToDisplay_u8; i++)
		{
			/* Continue extracting the digits from right side
			   till the Specified var_numOfDigitsToDisplay_u8 */
			if (var_DecNumber_u32 != 0)
			{
				/* Extract the digits from the number till it becomes zero.
				First get the remainder and divide the number by 10 each time.
				If var_num_u32 = 123 then extracted remainder will be 3 and number will be 12.
				The process continues till it becomes zero or max digits reached*/
				a[i] = util_GetMod32(var_DecNumber_u32, 10);
				var_DecNumber_u32 = var_DecNumber_u32 / 10;
			}
			else if ((var_numOfDigitsToDisplay_u8 == C_DisplayDefaultDigits_U8) ||
				(var_numOfDigitsToDisplay_u8 > C_MaxDigitsToDisplay_U8))
			{
				/* Stop the iteration if the Max number of digits are reached or
				 the user expects exact(Default) digits in the number to be displayed */
				break;
			}
			else
			{
				/* In case user expects more digits to be displayed than the actual digits in number,
				then update the remaining digits with zero.
			   Ex: var_num_u32 is 123 and user wants five digits then 00123 has to be displayed */
				a[i] = 0;
			}
		}
	}


	while (i)
	{
		/* Finally get the ascii values of the digits and display*/
		LCD_DisplayChar(util_Dec2Ascii(a[i - 1]));
		i--;
	}
}

void LCD_DisplayFloatNumber(double var_floatNum_f32)
{
	uint32_t var_temp_u32;
	/* Dirty hack to support the floating point by extracting the integer and fractional part.
	  1.Type cast the number to int to get the integer part.
	  2.Display the extracted integer part followed by a decimal point(.).
	  3.Later the integer part is made zero by subtracting with the extracted integer value.
	  4.Finally the fractional part is multiplied by 100000 to support 6-digit precision */

	var_temp_u32 = (uint32_t)var_floatNum_f32;
	LCD_DisplayDecimalNumber(var_temp_u32, C_DisplayDefaultDigits_U8);

	LCD_DisplayChar('.');

	var_floatNum_f32 = var_floatNum_f32 - var_temp_u32;
	var_temp_u32 = var_floatNum_f32 * 1000000;
	LCD_DisplayDecimalNumber(var_temp_u32, C_DisplayDefaultDigits_U8);
}
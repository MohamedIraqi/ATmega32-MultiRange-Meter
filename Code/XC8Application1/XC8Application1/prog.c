/*
 * prog.c
 *
 * Created: 10/31/2022 6:15:45 AM
 *  Author: DarkStar
 */
#include <xc.h>
#include <AVR/io.h>
#include "cpu_freq.h"
#include <util/delay.h>
#include "std_types.h"
#include "Macros.h"
#include "ADC.h"
#include "Lcd.h"
#include "prog.h"
#include "Keypad.h"
#include <math.h>
#include "Dio.h"


#define VOLT_CHANNEL1 ADC_CHANNEL_ADC7
#define VOLT_CHANNEL2 ADC_CHANNEL_ADC1
extern Adc_ConfigType Adc_Cfg;

f64 Volt_Read();
f64 Volt_Diff_Read();
void Volt_DC_Read(void);
void Volt_AC_Read(void);
void Ohm_Read(void);
void Temp_Read(void);

void init()
{
	LCD_Init();
	Keypad_Init();
	Adc_Init(&Adc_Cfg);
	Adc_ON();
}

void WelcomeScreen()
{
	LCD_SetCursorPostion(1, 1);
	LCD_DisplayString("Welcome");
	LCD_SetCursorPostion(1, 2);
	LCD_DisplayString("Voltmeter");
	for (s8 i = 4 - 1; i >= 0; i--)
	{
		_delay_ms(1000);
		LCD_SetCursorPostion(12, 2);
		LCD_DisplayNumber(i);
		LCD_DisplayString("..");
	}
	LCD_ClearDisplay();
	LCD_SetCursorPostion(1, 1);
	LCD_DisplayString("By Iraqi ");
	_delay_ms(2000);
}

void Progmanager()
{
	volatile Keypad_ButtonType button = KEYPAD_INVALID;

	LCD_ClearDisplay();
	LCD_SetCursorPostion(1, 1);
	LCD_DisplayString("0-DC  2-Ohm");
	LCD_SetCursorPostion(1, 2);
	LCD_DisplayString("1-AC(RMS)(50hz)");
	while (Keypad_GetPressedButton() == KEYPAD_INVALID)
	{
		button = Keypad_GetPressedButton();
	}
	while (Keypad_GetPressedButton() != KEYPAD_INVALID) {};
	LCD_SetCursorPostion(1, 1);
	switch (button)
	{
	case KEYPAD_B00:
		Volt_DC_Read();
		break;
	case KEYPAD_B01:
		Volt_AC_Read();
		break;
	case KEYPAD_B02:
		Ohm_Read();
		break;
	case KEYPAD_B03:
		Temp_Read();
		break;
	default:
		break;
	}
	button = KEYPAD_INVALID;
}
inline void Volt_DC_Read(void)
{
	volatile s64 Sumvolt = 0;
	u16 i = 0;
	s64 AVGVolt = 0;
	s64 max=0;
	u8 Stop = 0;
	while (!Stop)
	{
		for (i = 0; i < 6000; i++)
		{
			Sumvolt += 2*Volt_Read();
		}
		AVGVolt = Sumvolt / 6000;
		LCD_ClearDisplay();
		LCD_SetCursorPostion(1, 1);
		LCD_DisplayString("V(DC)=");
		LCD_DisplayNumber(AVGVolt);
		LCD_DisplayString(" mV");
		LCD_SetCursorPostion(1, 2);
		LCD_DisplayString("Max=");
		LCD_DisplayNumber(max);
		LCD_DisplayString(" mV");
		(max < AVGVolt) ? (max = AVGVolt):(max = max);
		(Keypad_GetPressedButton() == KEYPAD_B01) ? (max = 0) : (Stop = 0);
		Sumvolt = 0;
		(Keypad_GetPressedButton() == KEYPAD_B15) ? (Stop = 1) : (Stop = 0);
	}
}

void Volt_AC_Read(void)
{
	volatile u16 i = 0;
	volatile u64 Sumvolt = 0;
	u64 AVGVolt = 0;
	u8 Stop = 0;
	u64 max=0;
	while (!Stop)
	{
		for (i = 0; i < 6000; i++)
		{
			Sumvolt += Volt_Read();
		}
		AVGVolt = Sumvolt / 6000;
		LCD_ClearDisplay();
		LCD_DisplayString("V(RMS)=");
		LCD_DisplayNumber(AVGVolt*230/5);
		LCD_DisplayString(" mV");
		LCD_SetCursorPostion(1, 2);
		LCD_DisplayString("Max=");
		LCD_DisplayNumber(max*230/5);
		LCD_DisplayString(" mV");
		(max < AVGVolt) ? (max = (AVGVolt)) : (max = max);
		(Keypad_GetPressedButton() == KEYPAD_B01) ? (max = 0) : (Stop = 0);
		Sumvolt = 0;
		(Keypad_GetPressedButton() == KEYPAD_B15) ? (Stop = 1) : (Stop = 0);
	}
}

f64 Volt_Read()
{
	volatile f64 Volt;
	Adc_StartConversionSync(VOLT_CHANNEL2);
	Volt = Adc_GetResultSync();
	Volt = ((Volt) * 2560) / 1023;
	return Volt;
}

f64 Volt_Diff_Read()
{
	static u8 Voltogle = 0;
	volatile f64 Volt;
	if (Voltogle == 0)
	{
		Adc_StartConversionSync(VOLT_CHANNEL1);
		Voltogle = 1;
		Volt = Adc_GetResultSync();
	}
	else
	{
		Adc_StartConversionSync(VOLT_CHANNEL2);
		Voltogle = 0;
		Volt = Adc_GetResultSync();
		Volt = -Volt;
	}
	Volt = ((Volt) * 2560) / 1023;
	return Volt;
}

void Ohm_Read(void)
{
	volatile u16 i = 0;
	volatile u64 Sumvolt = 0;
	f64 AVGVolt = 0;
	volatile u64 Rmid = 5000;
	volatile u64 R1 = 10000;
	volatile u64 E = 5;
	u64 Rx = 0;
	u8 Stop = 0;
	u64 max = 0;
	Dio_SetPinMode(DIO_PORTA, DIO_PIN4, DIO_PIN_INPUT_FLOATING);
	Dio_SetPinMode(DIO_PORTA, DIO_PIN5, DIO_PIN_INPUT_FLOATING);
	while (!Stop)
	{
		if (Dio_ReadPinLevel(DIO_PORTA, DIO_PIN4) == 1)
		{
			Rmid = 5000;//4945;
			R1 = 10000;//9910;
		}
		else if (Dio_ReadPinLevel(DIO_PORTA, DIO_PIN5) == 1)
		{
			Rmid = 10000;//9924;
			R1 = 20000;//19700;
		}
		for (i = 0; i < 6000; i++)
		{
			Sumvolt += Volt_Read();
		}
		if (Dio_ReadPinLevel(DIO_PORTA, DIO_PIN5) == Dio_ReadPinLevel(DIO_PORTA, DIO_PIN4))
		{
			LCD_ClearDisplay();
			LCD_DisplayString("Error in");
			LCD_SetCursorPostion(1, 2);
			LCD_DisplayString("connection");
		}
		else
		{
			AVGVolt = Sumvolt / 6000;
			AVGVolt /= 1000;
			Rx = (Rmid * AVGVolt) / ((f64)(((f64)E * (f64)((f64)Rmid / (f64)R1)) - AVGVolt));
			LCD_ClearDisplay();
			LCD_SetCursorPostion(1, 1);
			if (Rx > (8 * Rmid))
			{
				LCD_DisplayString("OutOfRange");
			}
			else
			{
				LCD_DisplayString("Rx=");
				LCD_DisplayNumber(Rx);
				LCD_DisplayString("ohm");
			}
			LCD_SetCursorPostion(1, 2);
			LCD_DisplayString("Rmid=");
			LCD_DisplayNumber(Rmid);
			LCD_DisplayString("ohm");
			(max < AVGVolt) ? (max = (AVGVolt)) : (max = max);
			Sumvolt = 0;
			(Keypad_GetPressedButton() == KEYPAD_B01) ? (max = 0) : (Stop = 0);
			(Keypad_GetPressedButton() == KEYPAD_B15) ? (Stop = 1) : (Stop = 0);
		}
	}
}
void Temp_Read(void)
{
	volatile s16 i = 0;
	volatile f64 Sumvolt = 0;
	f64 AVGVolt = 0;
	s8 Stop = 0;
	//s64 max = 0;
	f64 temp = 0;
	f64 avgvolp = 0;
	while (!Stop)
	{
		for (i = 0; i < 8000; i++)
		{
			Sumvolt += Volt_Read();
		}
		AVGVolt = Sumvolt / 8000;
		avgvolp = AVGVolt/1000;
		LCD_ClearDisplay();
		temp = -25.46 * (log(((20 * avgvolp - 90) / (-2 * avgvolp - 9)) * 1000) / log(M_E)) + 260.08;//260.08
		LCD_DisplayString("Temp=");
		LCD_DisplayFloatNumber(temp);
		LCD_DisplayString("\337C");
		LCD_SetCursorPostion(1, 2);
		LCD_DisplayNumber(AVGVolt);
		//(max < AVGVolt) ? (max = (AVGVolt)) : (max = max);
		//(Keypad_GetPressedButton() == KEYPAD_B01) ? (max = 0) : (Stop = 0);
		Sumvolt = 0;
		//(Keypad_GetPressedButton() == KEYPAD_B15) ? (Stop = 1) : (Stop = 0);
	}

}

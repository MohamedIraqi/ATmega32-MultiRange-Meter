
/*
 * ADC.c
 *
 *  Created on: Sep 16, 2022
 *      Author: User
 */
#include "std_types.h"
#include "ADC.h"
#include "Macros.h"
#include "Dio.h"
#include "Registers.h"

 /******************************************************************
  * ADC Bits
  *****************************************************************/
#define ADMUX_REFS1     7
#define ADMUX_REFS0     6
#define ADMUX_ADLAR     5
#define ADMUX_MUX4      4
#define ADMUX_MUX3      3
#define ADMUX_MUX2      2
#define ADMUX_MUX1      1
#define ADMUX_MUX0      0
#define ADCSRA_ADEN     7
#define ADCSRA_ADSC     6
#define ADCSRA_ADATE    5
#define ADCSRA_ADIF     4
#define ADCSRA_ADIE     3
#define ADCSRA_ADPS2    2
#define ADCSRA_ADPS1    1
#define ADCSRA_ADPS0    0
#define SFIOR_ADTS2     7
#define SFIOR_ADTS1     6
#define SFIOR_ADTS0     5

  /******************************************************************
   * ADC ISR
   *****************************************************************/
//void (*Adc_CallbackISR)(u16 res);
//ISR(VECTOR_ADC)
//{
	//Adc_CallbackISR(ADC_DATA & 0x03FF);
//}

/******************************************************************
 * Functions Implementation
 *****************************************************************/

void Adc_Init(const Adc_ConfigType* config)
{
    /*select the voltage reference for the ADC*/
    ADMUX &= 0b00111111;
    ADMUX |= ((config->refernce) << 6);
     

    CLR_BIT(ADMUX, ADMUX_ADLAR);
    
    if (config->trigger == ADC_TRIGGER_OFF)
    {
        CLR_BIT(ADCSRA, ADCSRA_ADATE);
    }
    else
    {
        /* Auto Trigger Enable */
        SET_BIT(ADCSRA, ADCSRA_ADATE);
        /* Auto Tigger Source Selection */
        SFIOR &= 0b00011111;
        SFIOR |= ((config->trigger) << 5);
    }
    /* Determine the division factor between the XTAL frequency and the input clock to the ADC*/
    ADCSRA &= 0b11111000;
    ADCSRA |= (config->prescaler);
    //switch (config->prescaler)
    //{
    //case ADC_PRESCALER_2:
    //    // TODO
    //    break;
    //case ADC_PRESCALER_4:
    //    // TODO
    //    break;
    //case ADC_PRESCALER_8:
    //    // TODO
    //    break;
    //case ADC_PRESCALER_16:
    //    // TODO
    //    break;
    //case ADC_PRESCALER_32:
    //    // TODO
    //    break;
    //case ADC_PRESCALER_64:
    //    // TODO
    //    break;
    //case ADC_PRESCALER_128:
    //    SET_BIT(ADCSRA, ADCSRA_ADPS0);
    //    SET_BIT(ADCSRA, ADCSRA_ADPS1);
    //    SET_BIT(ADCSRA, ADCSRA_ADPS2);
    //    break;
    //default:
    //    break;
    //}
}

void Adc_StartConversionSync(Adc_ChannelType channel)
{
    /* Wait for current conversion to finish */
    while (GET_BIT(ADCSRA, ADCSRA_ADSC) == 1) {	};

    /* Channel Select */
    ADMUX &= 0b11100000;
    ADMUX |= channel;
    /* Start Conversion */
    SET_BIT(ADCSRA, ADCSRA_ADSC);
}

Adc_StateType Adc_StartConversionAsync(Adc_ChannelType channel)
{
    Adc_StateType state = ADC_STATE_BUSY;
    if (GET_BIT(ADCSRA, ADCSRA_ADSC) == 0)
    {
        /* Channel Select */
        ADMUX &= 0b11100000;
        ADMUX |= channel;
        /* Start Conversion */
        SET_BIT(ADCSRA, ADCSRA_ADSC);
        state = ADC_STATE_IDLE;
    }
    return state;
}

Adc_ConversionStateType Adc_GetResultAsync(u16* result)
{
    Adc_ConversionStateType flag = ADC_CONVERSION_NOT_COMPLETE;
    if (GET_BIT(ADCSRA, ADCSRA_ADIF) == 1)
    {
        SET_BIT(ADCSRA, ADCSRA_ADIF);
        *result = (ADC_DATA & 0x03FF);
        flag = ADC_CONVERSION_COMPLETE;
    }
    return flag;
}

u16 Adc_GetResultSync(void)
{
    u16 wtd = 1;
    /* Wait for conversion complete */
    while (GET_BIT(ADCSRA, ADCSRA_ADIF) == 0) { if (wtd == 0) { break; } else { wtd++; } };
    return (ADC_DATA & 0x03FF);
}

void Adc_EnableNotification(void)// (*callback)(u16 res)
{
   // Adc_CallbackISR = callback;
    SET_BIT(ADCSRA, ADCSRA_ADIE);
}

void Adc_DisableNotification(void)
{
    CLR_BIT(ADCSRA, ADCSRA_ADIE);
}

void Adc_OFF(void)
{
    CLR_BIT(ADCSRA, ADCSRA_ADEN);
}
void Adc_ON(void)
{
    SET_BIT(ADCSRA, ADCSRA_ADEN);
}
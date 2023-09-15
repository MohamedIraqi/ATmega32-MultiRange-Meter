/*
 * ADC.h
 *
 *  Created on: Sep 16, 2022
 *      Author: User
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "std_types.h"

 /******************************************************************
  * Types Definition
  *****************************************************************/
typedef enum
{
    ADC_STATE_BUSY,
    ADC_STATE_IDLE
} Adc_StateType;

typedef enum
{
    ADC_CONVERSION_NOT_COMPLETE,
    ADC_CONVERSION_COMPLETE
} Adc_ConversionStateType;

typedef enum
{
    ADC_CHANNEL_ADC0,
    ADC_CHANNEL_ADC1,
    ADC_CHANNEL_ADC2,
    ADC_CHANNEL_ADC3,
    ADC_CHANNEL_ADC4,
    ADC_CHANNEL_ADC5,
    ADC_CHANNEL_ADC6,
    ADC_CHANNEL_ADC7,
    ADC_CHANNEL_ADC0_ADC0_10X,
    ADC_CHANNEL_ADC1_ADC0_10X,
    ADC_CHANNEL_ADC0_ADC0_200X,
    ADC_CHANNEL_ADC1_ADC0_200X,
    ADC_CHANNEL_ADC2_ADC2_10X,
    ADC_CHANNEL_ADC3_ADC2_10X,
    ADC_CHANNEL_ADC2_ADC2_200X,
    ADC_CHANNEL_ADC3_ADC2_200X,
    ADC_CHANNEL_ADC0_ADC1_1X,
    ADC_CHANNEL_ADC1_ADC1_1X,
    ADC_CHANNEL_ADC2_ADC1_1X,
    ADC_CHANNEL_ADC3_ADC1_1X,
    ADC_CHANNEL_ADC4_ADC1_1X,
    ADC_CHANNEL_ADC5_ADC1_1X,
    ADC_CHANNEL_ADC6_ADC1_1X,
    ADC_CHANNEL_ADC7_ADC1_1X,
    ADC_CHANNEL_ADC0_ADC2_1X,
    ADC_CHANNEL_ADC1_ADC2_1X,
    ADC_CHANNEL_ADC2_ADC2_1X,
    ADC_CHANNEL_ADC3_ADC2_1X,
    ADC_CHANNEL_ADC4_ADC2_1X,
    ADC_CHANNEL_ADC5_ADC2_1X,
    ADC_CHANNEL_1_22_V,
    ADC_CHANNEL_GND
} Adc_ChannelType;

typedef enum
{
    ADC_REF_AREF, // AREF, Internal Vref turned off
    ADC_REF_AVCC, // AVCC with external capacitor at AREF pin
    ADC_REF_RESERVED,
    ADC_REF_INT // Internal 2.56V Voltage Reference with external capacitor at AREF pin
} Adc_ReferenceType;

typedef enum
{
    ADC_TRIGGER_FREE_RUNNING,// Free Running mode
    ADC_TRIGGER_AC,// Analog Comparator
    ADC_TRIGGER_INT0,// External Interrupt Request 0
    ADC_TRIGGER_TIM0_CM,// Timer/Counter0 Compare Match
    ADC_TRIGGER_TIM0_OF,// Timer/Counter0 Overflow
    ADC_TRIGGER_TIMB_CM,// Timer/Counter Compare Match B
    ADC_TRIGGER_TIM1_OF,// Timer/Counter1 Overflow
    ADC_TRIGGER_TIM1_CE,//  Timer/Counter1 Capture Even
    ADC_TRIGGER_OFF
} Adc_TriggerType;

typedef enum
{
    ADC_PRESCALER_2, //2
    ADC_PRESCALER_2A, //2 just 2 repeated
    ADC_PRESCALER_4,  //4
    ADC_PRESCALER_8,  //8
    ADC_PRESCALER_16, //16
    ADC_PRESCALER_32, //32
    ADC_PRESCALER_64, //64
    ADC_PRESCALER_128 //128
} Adc_PrescalerType;//Division Factor

typedef struct
{
    Adc_ReferenceType   refernce;
    Adc_TriggerType     trigger;
    Adc_PrescalerType   prescaler;
} Adc_ConfigType;

/******************************************************************
 * Configuration Struct
 *****************************************************************/
extern Adc_ConfigType Adc_Cfg;

/******************************************************************
 * Functions Prototype
 *****************************************************************/

void Adc_Init(const Adc_ConfigType* config);
void Adc_EnableNotification(void);//(*callback)(u16 res)
void Adc_DisableNotification(void);
void Adc_StartConversionSync(Adc_ChannelType channel);
Adc_StateType Adc_StartConversionAsync(Adc_ChannelType channel);
Adc_ConversionStateType Adc_GetResultAsync(u16* result);
u16 Adc_GetResultSync(void);
void Adc_OFF(void);
void Adc_ON(void);

#endif /* INC_ADC_H_ */

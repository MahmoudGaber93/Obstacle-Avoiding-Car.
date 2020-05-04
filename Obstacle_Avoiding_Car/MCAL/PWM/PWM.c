/***************************************************/
/* Layer: MCAL                                     */
/* Module: PWM                                     */
/* File: PWM.C                                     */
/* Author: GABER                                   */
/* Description: PWM header Source                  */
/***************************************************/

/************************************************************************/
/*		                     INCLUDES	                                */
/************************************************************************/

#include "PWM.h"

 /************************************************************************/
 /*				                   DEFINES	            		         */
 /************************************************************************/

#define     NO_PRESCALER_V1              (7)
#define     NO_PRESCALER_V2              (7)
#define     CHANNEL_NO                   (4)

#define     MAX_NUM_COUNT_T1_2          (255)

#define     DIVESION_BY_100             (100)
#define     DIVESION_BY_2                (2)

#define     PWM_INITIATED                (1)
#define     PWM_NOT_INITIATED            (0)

#define     PWM_CH0_STOP_MASK             (0xf8)
#define     PWM_CH1A_STOP_MASK            (0x7f)
#define     PWM_CH1B_STOP_MASK            (0xdf)
#define     PWM_CH3_STOP_MASK             (0xf8)

#define     CH0_INIT                      (0x60)

#define     CH1A_INIT1                  (0b10000000)
#define     CH1A_INIT2                     (0x10)

#define     CH1B_INIT1                   (0b00100000)
#define     CH1B_INIT2                      (0x10)

#define     CH2_INIT                        (0x60)

/************************************************************************/
/*		                     GLOBAL VARIABLES	                        */
/************************************************************************/


/********** Look up table  of the PWM prescaler of CH0, CH1A, CH1B*********/
static uint8 PwmPrescal_Start_Lut_V1[NO_PRESCALER_V1] = {0x01,0x02,0,0x03,0,0x04,0x05};

/********** Look up table  of the PWM prescaler of CH2*******************/
static uint8 PwmPrescal_Start_Lut_V2[NO_PRESCALER_V2] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07};

/********** Look up table  of the PWM prescaler real value****************/
static uint16 PwmPrescal__ActualValue_Lut[NO_PRESCALER_V2] = {1,8,32,64,128,256,1024};

/*************Array to save the state of CH (Started or Stopped)*********/
static uint8 PwmPrescal_SaveStart_Ch[CHANNEL_NO] = {0};

/*************Array to save the real value of prescaler*****************/
static uint16 PwmPrescal_SaveActual_Ch[CHANNEL_NO] = {0};

/*************Array to save the state of CH (Initiated  or not)*********/
static uint8 Init_Pwm[CHANNEL_NO] = {0};



/************************************************************************/
/*                       FUNCTIONS IMPLEMENTATION                       */
/************************************************************************/

/*********************************************************************************/
/* Function: Error_State Pwm_Init(Pwm_Cfg_s_t *Pwm_Cfg);                         */
/* Type: public                                                                  */
/* Input parameters: Pwm_Cfg Structure (channel number, Prescaler)               */
/* Return type : void                                                            */
/*                                                                               */
/* Description: initialize the PWM configuration                                 */
/*********************************************************************************/
extern ERROR_STATUS Pwm_Init(Pwm_Cfg_s_t *Pwm_Cfg)
{
	DIO_Cfg_s Pwm_PinConfg_s;

	switch(Pwm_Cfg -> Channel)
	{
	case PWM_CH0 :

/*TCCR0
* foc0 >>0
* WGM00 WGM01 : 1  0   >>> PWM, Phase Correct
* COM00 COM01 : 0  1   >>> non_inverting mode
*/
		/**********************checking the boundry conditions****************************/
		if(!((Pwm_Cfg->Prescaler==PWM_PRESCALER_32)||((Pwm_Cfg->Prescaler==PWM_PRESCALER_128)||(Pwm_Cfg -> Prescaler >PWM_PRESCALER_1024))))
		{

		PwmPrescal_SaveStart_Ch[PWM_CH0]  = PwmPrescal_Start_Lut_V1[Pwm_Cfg -> Prescaler];
		PwmPrescal_SaveActual_Ch[PWM_CH0] = PwmPrescal__ActualValue_Lut[Pwm_Cfg -> Prescaler];
     /***************************choose the phase correct mode*****************************/
		TCCR0 =CH0_INIT;

		/***********************Set PWM Pins DIRECTION******************************/

		Pwm_PinConfg_s . GPIO = GPIOB;
		Pwm_PinConfg_s . pins = PIN3;
		Pwm_PinConfg_s . dir  = OUTPUT;
		}
		else
		{
			return E_NOK;
		}

		break;
	case PWM_CH1A :
/*
* TCCR1A  COM1A1 COMA10 COM1B1 COM1B0 FOC1A FOC1B WGM11 WGM10
* TCCR1B  ICNC1  ICES1     -    WGM13 WGM12 CS12 CS11  CS10
*/

		if(!((Pwm_Cfg->Prescaler==PWM_PRESCALER_32)||((Pwm_Cfg->Prescaler==PWM_PRESCALER_128)||(Pwm_Cfg -> Prescaler >PWM_PRESCALER_1024))))
		{
			TCCR1A |= CH1A_INIT1;
			TCCR1B |= CH1A_INIT2;
			PwmPrescal_SaveStart_Ch[PWM_CH1A]  = PwmPrescal_Start_Lut_V1[Pwm_Cfg -> Prescaler];
			PwmPrescal_SaveActual_Ch[PWM_CH1A] = PwmPrescal__ActualValue_Lut[Pwm_Cfg -> Prescaler];

			/***********************Set PWM Pins DIRECTION******************************/

			Pwm_PinConfg_s . GPIO = GPIOD;
			Pwm_PinConfg_s . pins = PIN5;
			Pwm_PinConfg_s . dir  = OUTPUT;
		}
		else
		{
			return E_NOK;
		}

		break;
	case PWM_CH1B :

/*
* TCCR1A  COM1A1 COMA10 COM1B1 COM1B0 FOC1A FOC1B WGM11 WGM10
* TCCR1B  ICNC1  ICES1     -    WGM13 WGM12 CS12 CS11  CS10
*/
		if(!((Pwm_Cfg->Prescaler==PWM_PRESCALER_32)||((Pwm_Cfg->Prescaler==PWM_PRESCALER_128)||(Pwm_Cfg -> Prescaler >PWM_PRESCALER_1024))))
		{
			TCCR1A |= CH1B_INIT1;
			TCCR1B |= CH1B_INIT2;
			PwmPrescal_SaveStart_Ch[PWM_CH1B] = PwmPrescal_Start_Lut_V1[Pwm_Cfg -> Prescaler];
			PwmPrescal_SaveActual_Ch[PWM_CH1B] = PwmPrescal__ActualValue_Lut[Pwm_Cfg -> Prescaler];

			Pwm_PinConfg_s . GPIO = GPIOD;
			Pwm_PinConfg_s . pins = PIN4;
			Pwm_PinConfg_s . dir  = OUTPUT;
		}
		else
		{
			return E_NOK;
		}
		break;
	case PWM_CH2:
/*TCCR2
* foc2 >>
* WGM20 WGM21 : 1  0   >>> PWM, Phase Correct
* COM20 COM21 : 0  1   >>> non_inverting mode
*/
		if(!(Pwm_Cfg -> Prescaler >PWM_PRESCALER_1024))
		{
			TCCR2 = CH2_INIT;
			PwmPrescal_SaveStart_Ch[PWM_CH2] = PwmPrescal_Start_Lut_V2[Pwm_Cfg -> Prescaler];
			PwmPrescal_SaveActual_Ch[PWM_CH2] = PwmPrescal__ActualValue_Lut[Pwm_Cfg -> Prescaler];

			/***********************Set PWM Pins DIRECTION******************************/

			Pwm_PinConfg_s . GPIO = GPIOD;
			Pwm_PinConfg_s . pins = PIN7;
			Pwm_PinConfg_s . dir  = OUTPUT;
		}
		else
		{
			return E_NOK;
		}
		break;
	default:
		return E_NOK;
		break;
	}
/***********************Set the initialization check****************************************/
	Init_Pwm[Pwm_Cfg -> Channel] = PWM_INITIATED;
	DIO_init(&Pwm_PinConfg_s);
	return E_OK;

}

/*********************************************************************************/
/* Function: Error_State Pwm_Start(uint8_t channel,uint8_t Duty);                */
/* Type: public                                                                  */
/* Input parameters: Channel ID, Duty cycle(0:100)                               */
/* Return type : Error State                                                     */
/*                                                                               */
/* Description: starts the PWM on the dedicated channel with the required duty   */
/*				cycle and frequency                                              */
/*********************************************************************************/
extern ERROR_STATUS Pwm_Start(uint8 Channel,uint8 Duty,uint32 Frequncy)
{

	if(Init_Pwm[Channel] == PWM_INITIATED)
	{
		switch(Channel)
		{
		case PWM_CH0 :
			OCR0 = ((Duty * MAX_NUM_COUNT_T1_2)/DIVESION_BY_100);

			/*********************start the PWM************************/
			TCCR0 |= PwmPrescal_SaveStart_Ch[Channel];
			break;

		case PWM_CH1A :
			/******************Calculate the ICR1 and OCR1A **********************/
			ICR1   = (uint32) ((F_CPU) /(PwmPrescal_SaveActual_Ch[Channel]*DIVESION_BY_2 *Frequncy));
			OCR1A  = ( ICR1 * Duty);
			OCR1A /= DIVESION_BY_100;

			/*********************start the PWM************************/
			TCCR1B |= PwmPrescal_SaveStart_Ch[Channel];
			break;

		case PWM_CH1B :
			/******************Calculate the ICR1 and OCR1B **********************/
			ICR1   = (uint32) ((F_CPU) /(PwmPrescal_SaveActual_Ch[Channel] * DIVESION_BY_2 * Frequncy));
			OCR1B  = ( ICR1 * Duty);
			OCR1B /= DIVESION_BY_100;

			/*********************start the PWM************************/
			TCCR1B |= PwmPrescal_SaveStart_Ch[Channel];

			break;
		case PWM_CH2 :
			OCR2 = ((Duty * MAX_NUM_COUNT_T1_2)/DIVESION_BY_100);

			/*********************start the PWM************************/
			TCCR2 |= PwmPrescal_SaveStart_Ch[Channel];
			break;
		default:
			return E_NOK;
			break;

		}
	}

	else
	{
		return E_NOK ;
	}
	return E_OK;
}

/*********************************************************************************/
/* Function: Error_State Pwm_Start(uint8_t channel,uint8_t Duty);                */
/* Type: public                                                                  */
/* Input parameters: Channel ID, Duty cycle(0:100)                               */
/* Return type : Error State                                                     */
/*                                                                               */
/* Description: updates the duty cycle and frequency of the dedicated channel    */
/*********************************************************************************/
extern ERROR_STATUS Pwm_Update(uint8 Channel,uint8 Duty,uint32 Frequncy)
{
	if(PwmPrescal_SaveActual_Ch[Channel] != PWM_NOT_INITIATED)
		{
			switch(Channel)
			{
			case PWM_CH0 :
				OCR0   = ((Duty * MAX_NUM_COUNT_T1_2)/DIVESION_BY_100);
				break;

			case PWM_CH1A :
				ICR1   = (uint16) ((F_CPU) /(PwmPrescal_SaveActual_Ch[Channel]*2*Frequncy));
				OCR1A  = (ICR1/DIVESION_BY_100);
				OCR1A *= Duty;
				break;

			case PWM_CH1B :
				ICR1   = (uint32) ((F_CPU) /(PwmPrescal_SaveActual_Ch[Channel]*2*Frequncy));
				OCR1B  = (ICR1/DIVESION_BY_100);
				OCR1B *= Duty;
				//OCR1B = (Duty *ICR1)/100 ;/*didnt work for pres_8 */
				break;
			case PWM_CH2 :
				OCR2   = ((Duty * MAX_NUM_COUNT_T1_2)/DIVESION_BY_100);
				TCCR2 |= PwmPrescal_SaveStart_Ch[Channel];
				break;
			default:
				return E_NOK;
				break;
			}
		}
		else
		{
			return E_NOK ;
		}
		return E_OK;

}

/*********************************************************************************/
/* Function: Error_State Pwm_Stop(uint8_t channel);                              */
/* Type: public                                                                  */
/* Input parameters: channel ID                                                  */
/* Return type : Error state                                                     */
/*                                                                               */
/* Description: responsible of Stopping the PWM by clearing the prescaler		 */
/*				of the corresponding channel                                     */
/*********************************************************************************/
extern ERROR_STATUS Pwm_Stop(uint8 Channel)
{
	switch(Channel)
	{
	case PWM_CH0 :
		TCCR0 &= PWM_CH0_STOP_MASK ;
		break;

	case PWM_CH1A :
		TCCR1A &= PWM_CH1A_STOP_MASK ;
		break;

	case PWM_CH1B :
		TCCR1A &= PWM_CH1B_STOP_MASK;

		break;
	case PWM_CH2 :

		TCCR2 &= PWM_CH3_STOP_MASK;
		break;
	default:
		return E_NOK;
		break;

	}
	PwmPrescal_SaveActual_Ch[Channel] = PWM_NOT_INITIATED;

		return E_OK;
}

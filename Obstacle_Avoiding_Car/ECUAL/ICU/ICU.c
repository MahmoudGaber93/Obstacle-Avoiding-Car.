/*
 *  File Name :ICU.c
 *  Author    : GABER
 *  Version   : 1
 */

/************************************************************************/
/*				 		      INCLUDES                                  */
/************************************************************************/

#include "ICU.h"
#include "DIO.h"

/************************************************************************/
/*				 		       Defines                                  */
/************************************************************************/

#define ICU_NO                (3)
#define ONE                   (1)
#define ZERO                  (0)

#define MAX_NUM_COUNT_T0_2    (255)
#define USED_PRESCALER        (64)
#define F_CPU_US              (16)
/************************************************************************/
/*		                     GLOBAL VARIABLES	                        */
/************************************************************************/

uint32 gu32_Ov_num               = ZERO;
uint8  gu8_flag                  = ONE;
uint32 gu32_Timer_value          = ZERO;
uint32 gu32_Raising_value        = ZERO;
uint32 gu32_Falling_value        = ZERO;
uint16 gu16_Timer_Register_value = ZERO;
uint32 gu32_Pulse_value          = ZERO;
uint8 gu8_Read                   = ONE;

/************************************************************************/
/*				          Functions' Prototypes                         */
/************************************************************************/


void ISR_Function(void)
{
	Timer_GetValue(ICU_TIMER_CH0,&gu16_Timer_Register_value);
	gu32_Timer_value = gu16_Timer_Register_value+(gu32_Ov_num*MAX_NUM_COUNT_T0_2);
	if(gu8_flag == ONE)
	{
		gu32_Raising_value = gu32_Timer_value;
		ExternInt_SetEvent(EXTRN_INT_2,FALLING_EDGE);
		gu8_flag = ZERO;

	}
	else
	{
	gu32_Falling_value = gu32_Timer_value;
	gu32_Pulse_value = gu32_Falling_value - gu32_Raising_value;
	ExternInt_SetEvent(EXTRN_INT_2,RISING_EDGE);
	gu8_flag = ONE;
	/**************flag to the read the value of the ultrasonic pulse***********/
	gu8_Read    =  ZERO;
	gu32_Ov_num =  ONE;
Timer_Stop(TIMER_CH0);

	}

}
void Timer_Function(void)
{

	gu32_Ov_num++;
}






/**************************************************************************
 * Function 	: Icu_Init                                                *
 * Input 		: Icu_cfg : pointer to structure of type Icu_cfg_s        *
 * Return 		: value of type ERROR_STATUS							  *
 * 				  which is one of the following values:					  *
 * 				  - E_OK  : initialized successfully					  *
 *				  - E_NOK : not initialized successfully				  *
 * Description  : Initializes the ICU by initializing the timer			  *
 * 				  and enabling the global interrupt						  *
 **************************************************************************/
ERROR_STATUS Icu_Init(Icu_cfg_s * Icu_Cfg)
{

	ExternInt_Cfg_s EXTERNAL_INTERRUPT_ST;
	EXTERNAL_INTERRUPT_ST.ExternInt_No = Icu_Cfg->ICU_Ch_No;
	EXTERNAL_INTERRUPT_ST.ExternInt_Event =RISING_EDGE;
	EXTERNAL_INTERRUPT_ST.ExternInt_CBF_Ptr = ISR_Function;
	ExternInt_Init(&EXTERNAL_INTERRUPT_ST);
	ExternInt_Enable(EXTRN_INT_2);

	Timer_cfg_s Timer_ST;
	Timer_ST.Timer_CH_NO = Icu_Cfg -> ICU_Ch_Timer;
	Timer_ST.Timer_Prescaler = TIMER_PRESCALER_64;
	Timer_ST.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;
	Timer_ST.Timer_Mode = TIMER_MODE;
	Timer_ST.Timer_Cbk_ptr = Timer_Function;
	Timer_Init(&Timer_ST);

	DIO_Cfg_s a;
	a.GPIO = GPIOC;
	a.dir  = OUTPUT;
	a.pins = FULL_PORT;
	DIO_init(&a);
   return E_OK;

}

/***************************************************************************
 * Function		: Icu_RiseToFall										   *
 * Output 		: Icu_Time : pointer to uint32 variable to give the time   *
 * 				  from rising edge to falling edge						   *
 * Return 		: value of type ERROR_STATUS							   *
 * 				  which is one of the following values:					   *
 * 				  - E_OK  : successful									   *
 *				  - E_NOK : not successful								   *
 * Description	: calculates the time between the rising and the falling   *
 * 				  edges												 	   *
 ***************************************************************************/
ERROR_STATUS Icu_RiseToFall(uint32 * Icu_Time) /*call it after trigger pulse*/
{

	if(NULL!= Icu_Time)
	{
	while(0!= gu8_Read);
	*Icu_Time = gu32_Pulse_value;
	/************************prescaler_64***************/
	*Icu_Time *= (USED_PRESCALER/F_CPU_US);
	gu8_Read =ONE;

	return E_OK;
	}
	else
	{
		return E_NOK;
	}

}

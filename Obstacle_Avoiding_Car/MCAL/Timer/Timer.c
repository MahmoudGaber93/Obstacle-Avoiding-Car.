/*
 *     file name : Timer
 *      Author   : GABER
 *      Version  : 1
 */


/************************************************************************/
/*		                     INCLUDES	                                */
/************************************************************************/

#include "Timer.h"



 /************************************************************************/
 /*				                   DEFINES	            		         */
 /************************************************************************/
#define     TIMER_NO                 (3)

/*****************Prescaler of timer0 and timer1*************************/

#define		T0_1_NO_CLOCK	   			(0x00)
#define		T0_1_PRESCALER_NO		    (0x01)
#define		T0_1_PRESCALER_8			(0x02)
#define		T0_1_PRESCALER_64			(0x03)
#define		T0_1_PRESCALER_256			(0x04)
#define		T0_1_PRESCALER_1024         (0x05)
#define		T0_1_COUNTER_FALLING        (0x06)
#define		T0_1_COUNTER_RAISING        (0x07)


/*****************Prescaler of timer2*************************/

#define		T2_NO_CLOCK	   			    (0x00)
#define		T2_PRESCALER_NO		        (0x01)
#define		T2_PRESCALER_8			    (0x02)
#define		T2_PRESCALER_32             (0x03)
#define		T2_PRESCALER_64	            (0x04)
#define		T2_PRESCALER_128            (0x05)
#define		T2_PRESCALER_256			(0x06)
#define		T2_PRESCALER_1024           (0x07)

/*****************INTERRUPT *************************/
#define     ENABLE_TIMER0_INTERRUPT     (0x01)
#define     ENABLE_TIMER1_INTERRUPT     (0x04)
#define     ENABLE_TIMER2_INTERRUPT     (0x40)

#define     TIMER0_POLLING              (0xf7)
#define     TIMER1_POLLING              (0xfb)
#define     TIMER2_POLLING              (0xbf)

/*****************State of the timer(initiated or not)*************************/
#define     TIMER_INITIATED                (1)
#define     TIMER_NON_INITIATED            (0)

/*****************State of the timer(started or stopped)*************************/
#define     TIMER_STARTED                  (1)
#define     TIMER_STOPED                   (0)

/*****************Max counts of Timers*************************/
#define MAX_NUM_COUNT_T0_T2                (255)
#define MAX_NUM_COUNT_T1                  (0xffff)


#define TIMER_START_MASK                  (0xF8)
#define TIMER_STOP_MASK                   (0xF8)
/************************************************************************/
/*		                     GLOBAL VARIABLES	                        */
/************************************************************************/

static uint8 gu8_prescaler =0;

/************Array to store the state of timer initiated or not*******************/
static uint8 gau8_InitStatus[3]={0};

/************Array to store the state of timer started or not**********************/
static uint8 gau8_StatrtStatus[3]={0};

/*****Array of pointer to function to  store addresses of call back function ******/
 void (*Timer_CBF_ptr_fun[TIMER_NO]) (void)= {NULL,NULL,NULL};



/************************************************************************/
/*		             TIMER FUNCTIONS' IMPLIMENTATION		            */
/************************************************************************/


/**
 * Input: Pointer to a structure contains the information needed to initialize the timer.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module.
 *
 */
ERROR_STATUS Timer_Init(Timer_cfg_s* Timer_cfg)
{
	switch(Timer_cfg -> Timer_CH_NO)
	{
	case TIMER_CH0:
		switch(Timer_cfg -> Timer_Mode)
		{
		case TIMER_MODE:
			switch(Timer_cfg -> Timer_Prescaler)
			{
			case TIMER_NO_CLOCK:
				gu8_prescaler =T0_1_NO_CLOCK;
				break;
			case TIMER_PRESCALER_NO:
				gu8_prescaler = T0_1_PRESCALER_NO;
				break;
			case TIMER_PRESCALER_8:
				gu8_prescaler=T0_1_PRESCALER_8;
				break;
			case TIMER_PRESCALER_64:
				gu8_prescaler= T0_1_PRESCALER_64;
				break;
			case TIMER_PRESCALER_256:
				gu8_prescaler= T0_1_PRESCALER_256;
				break;
			case TIMER_PRESCALER_1024:
				gu8_prescaler= T0_1_PRESCALER_1024;
				break;
			default:
			return	E_NOK;
				break;
    		 }
			break;

		case COUNTER_RISING_MODE:
				gu8_prescaler= T0_1_COUNTER_RAISING;
				break;
		case COUNTER_FALLING_MODE:
				gu8_prescaler= T0_1_COUNTER_FALLING;
				break;
			default :
				return E_NOK;
				break;
		 }
		switch(Timer_cfg -> Timer_Polling_Or_Interrupt)
		{
		case TIMER_POLLING_MODE:
			TIMSK &=TIMER0_POLLING;
			break;
		case TIMER_INTERRUPT_MODE:
			if(NULL != Timer_cfg ->Timer_Cbk_ptr)
			{
				Timer_CBF_ptr_fun[TIMER_CH0] = Timer_cfg ->Timer_Cbk_ptr;
			}
			else
			{
				return E_NOK;
			}
			TIMSK |= ENABLE_TIMER0_INTERRUPT;
			break;
		default :
			return E_NOK;
			break;
		}
		gau8_InitStatus[Timer_cfg -> Timer_CH_NO] = TIMER_INITIATED;
		break;

	case TIMER_CH1:
		switch(Timer_cfg -> Timer_Mode)
		{
		case TIMER_MODE:
			switch(Timer_cfg -> Timer_Prescaler)
			{
			case TIMER_NO_CLOCK:
				gu8_prescaler= T0_1_NO_CLOCK;
				break;
			case TIMER_PRESCALER_NO:
				gu8_prescaler= T0_1_PRESCALER_NO;
				break;
			case TIMER_PRESCALER_8:
				gu8_prescaler= T0_1_PRESCALER_8;
				break;
			case TIMER_PRESCALER_64:
				gu8_prescaler= T0_1_PRESCALER_64;
				break;
			case TIMER_PRESCALER_256:
				gu8_prescaler= T0_1_PRESCALER_256;
				break;
			case TIMER_PRESCALER_1024:
				gu8_prescaler= T0_1_PRESCALER_1024;
				break;
			default :
				return E_NOK;
				break;
			}
			break;

		case COUNTER_RISING_MODE:
			gu8_prescaler=T0_1_COUNTER_RAISING;
			break;
		case COUNTER_FALLING_MODE:
			gu8_prescaler= T0_1_COUNTER_FALLING;
			break;
		default:
			return E_NOK;
			break;
		}
		switch(Timer_cfg -> Timer_Polling_Or_Interrupt)
		{
		case TIMER_POLLING_MODE:
			TIMSK &=TIMER1_POLLING;
			break;
		case TIMER_INTERRUPT_MODE:
			if(NULL != Timer_cfg ->Timer_Cbk_ptr)
			{
				Timer_CBF_ptr_fun[TIMER_CH1] = Timer_cfg ->Timer_Cbk_ptr;
			}
			else
			{
				return E_NOK;
			}
			TIMSK |= ENABLE_TIMER1_INTERRUPT;
			break;
		default :
			return E_NOK;
			break;
		}
		gau8_InitStatus[Timer_cfg -> Timer_CH_NO] = 1;
		break;

	case TIMER_CH2:
		switch(Timer_cfg -> Timer_Mode)
		{
		case TIMER_MODE:
			switch(Timer_cfg -> Timer_Prescaler)
			{
			case TIMER_NO_CLOCK:
				gu8_prescaler= T2_NO_CLOCK;
				break;
			case TIMER_PRESCALER_NO:
				gu8_prescaler= T2_PRESCALER_NO;
				break;
			case TIMER_PRESCALER_8:
				gu8_prescaler= T2_PRESCALER_8;
				break;
			case TIMER_PRESCALER_32:
				gu8_prescaler= T2_PRESCALER_32;
				break;
			case TIMER_PRESCALER_64:
				gu8_prescaler= T2_PRESCALER_64;
				break;
			case TIMER_PRESCALER_128:
				gu8_prescaler= T2_PRESCALER_128;
				break;
			case TIMER_PRESCALER_256:
				gu8_prescaler= T2_PRESCALER_256;
				break;
			case TIMER_PRESCALER_1024:
				gu8_prescaler= T2_PRESCALER_1024;
				break;
			default:
				return E_NOK;
			}
			break;
		default :
			return E_NOK ;
			break;
		}
		switch(Timer_cfg -> Timer_Polling_Or_Interrupt)
		{
		case TIMER_POLLING_MODE:
			TIMSK &=TIMER2_POLLING;
			break;
		case TIMER_INTERRUPT_MODE:
			if(NULL != Timer_cfg ->Timer_Cbk_ptr)
			{
				Timer_CBF_ptr_fun[TIMER_CH2] = Timer_cfg ->Timer_Cbk_ptr;
			}
			else
			{
				return E_NOK;
			}
			TIMSK |= ENABLE_TIMER2_INTERRUPT;
			break;
		default :
			return E_NOK;
			break;
		}
		gau8_InitStatus[Timer_cfg -> Timer_CH_NO] = TIMER_INITIATED;
		break;
	default :
		return E_NOK ;
		break;
	}

	return E_OK;
}

/**
 * Input:
 * 	Timer_CH_NO: The channel number of the timer needed to be started.
 *	Timer_Count: The start value of the timer.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function strats the needed timer.
 *
 */
ERROR_STATUS Timer_Start(uint8 Timer_CH_NO, uint16 Timer_Count)
{
	if(gau8_InitStatus[Timer_CH_NO] == TIMER_INITIATED)
	{
		switch( Timer_CH_NO)
		{
		case TIMER_CH0:
			TCNT0 =MAX_NUM_COUNT_T0_T2- Timer_Count;
			TCCR0 &= TIMER_START_MASK;
			TCCR0 |= gu8_prescaler;
			break;
		case TIMER_CH1:
			TCNT1 = MAX_NUM_COUNT_T1-Timer_Count;
			TCCR1 &= TIMER_START_MASK;
			TCCR1 |= gu8_prescaler;
			break;
		case TIMER_CH2:
			TCNT2 = MAX_NUM_COUNT_T0_T2-Timer_Count;
			TCCR2 &= TIMER_START_MASK;
			TCCR2 |= gu8_prescaler;
			break;
		default :
			return E_NOK;
		}
		gau8_StatrtStatus[Timer_CH_NO] = TIMER_STARTED;
		return E_OK ;
	}
	else
	{
		return E_NOK;
	}
}

/**
 * Input:
 * 	Timer_CH_NO: The channel number of the timer needed to be stopped.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function stops the needed timer.
 *
 */
ERROR_STATUS Timer_Stop(uint8 Timer_CH_NO)
{
	if(	gau8_StatrtStatus[Timer_CH_NO] == TIMER_STARTED )
	{
	switch( Timer_CH_NO)
	{
	case TIMER_CH0:
		TCCR0 &=  TIMER_STOP_MASK  ;
		break;
	case TIMER_CH1:
		TCCR1 &=  TIMER_STOP_MASK  ;
		break;
	case TIMER_CH2:
		TCCR2 &=  TIMER_STOP_MASK  ;
		break;
	default :
		return E_NOK;
	}
	gau8_StatrtStatus[Timer_CH_NO] = TIMER_STOPED;
	return E_OK ;
	}
	else
	{
		return E_NOK;
	}
}

/**
 * Input:
 * 	Timer_CH_NO: The channel number of the timer needed to get its status.
 * Output:
 * 	Data: A variable of type bool returns if the flag of the timer is raised or not.
 * In/Out:
 * Return: The error status of the function.
 * Description: This function is used to return if the flag of the timer is raised or not.
 *
 */
ERROR_STATUS Timer_GetStatus(uint8 Timer_CH_NO, bool* Data)
{
	if(gau8_InitStatus[Timer_CH_NO] == TIMER_INITIATED)
	{
		switch( Timer_CH_NO)
		{
		case TIMER_CH0:
			*Data = GET_BIT(TIFR,BIT0);
			break;
		case TIMER_CH1:
			*Data = GET_BIT(TIFR,BIT2);
			break;
		case TIMER_CH2:
			*Data = GET_BIT(TIFR,BIT6);
			break;
		default :
			return E_NOK;
		}
		return E_OK ;
	}
	else
	{
		return E_NOK;

	}
}

/**
 * Input:
 * 	Timer_CH_NO: The channel number of the timer needed to get its value.
 * Output:
 * 	Data: This is the output variable of the function which holds the value of the timer.
 * In/Out:
 * Return: The error status of the function.
 * Description: This function is used to return the value of the timer.
 *
 */
ERROR_STATUS Timer_GetValue(uint8 Timer_CH_NO, uint16* Data)
{
	if(gau8_InitStatus[Timer_CH_NO] == TIMER_INITIATED)
	{
		switch( Timer_CH_NO)
		{
		case TIMER_CH0:
			*Data = TCNT0;
			break;
		case TIMER_CH1:
			*Data = TCNT1;
			break;
		case TIMER_CH2:
			*Data = TCNT2;
			break;
		default :
			return E_NOK;
		}
		return E_OK ;
	}
	else
	{
		return E_NOK;
	}
}



/************************************************************************/
/*		                 IMPLEMNTATION OF THE OVER FLOW ISR 	        */
/************************************************************************/

ISR(TIMER0_OVF_vect)
{
/**************************Callback function of Timer0*******************/
 Timer_CBF_ptr_fun[TIMER_CH0]();
}



ISR(TIMER1_OVF_vect)
{
	/**************************Callback function of Timer1*******************/
	Timer_CBF_ptr_fun[TIMER_CH1]();
}


ISR(TIMER2_OVF_vect)
{
	/**************************Callback function of Timer2*******************/
	Timer_CBF_ptr_fun[TIMER_CH2]();
}


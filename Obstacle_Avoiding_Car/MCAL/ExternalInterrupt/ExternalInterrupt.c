
/* Layer: MCAL                                     */
/* Module: PWM                                     */
/* File: ExternalInterrupt.c                       */
/* Author: GABER                                   */
/* Description: ExternalInterrupt Source           */
/***************************************************/



/************************************************************************/
/*		                     INCLUDES	                                */
/************************************************************************/
#include "ExternalInterrupt.h"

 /************************************************************************/
 /*				                   DEFINES	            		         */
 /************************************************************************/

#define     INTERRUPT_NO                    (3)
#define     EVENT_NO                        (4)
#define     SET_RAISING_EDGE               (0x40)
#define     SET_FALLING_EDGE               (0x00)

#define     EXT_INTERRUPT_INITIATED          (1)
#define     EXT_INTERRUPT_NOT_INITIATED      (0)

#define     EXT0_MASK                       (0xfc)
#define     EXT1_MASK                        (0xf3)

/************************************************************************/
/*		                     GLOBAL VARIABLES	                        */
/************************************************************************/

static uint8 Interrupt_Lut[INTERRUPT_NO] = {0x40,0x80,0x20};

static uint8 Event_Lut[EVENT_NO] = {0x00,0x01,0x02,0x03};

static uint8 Init_Staut[EVENT_NO] = {0};

 void (*Interrupt_CBF_ptr[INTERRUPT_NO]) (void)= {NULL,NULL,NULL};

/************************************************************************/
/*				 		Functions Implementation                        */
/************************************************************************/

/*
 * Function: ExternInt_Init
 * Inputs:	ExternIntCfg -> pointer of type ExternInt_Cfg_s which points to structure that contain the initialized data.
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: Initiate the required external interrupt configuration as it specify
 *	event of interrupt.
 *	Enable the required external interrupt from GICR but to allow the interrupt global interrupt must be set
 */
ERROR_STATUS ExternInt_Init(ExternInt_Cfg_s *ExternIntCfg)
{
	switch(ExternIntCfg->ExternInt_No)
	{
	case EXTRN_INT_0 :
		/*******set the call back fun ***********/
		if(NULL != (ExternIntCfg -> ExternInt_CBF_Ptr))
		{
			Interrupt_CBF_ptr[ExternIntCfg->ExternInt_No] = ExternIntCfg -> ExternInt_CBF_Ptr;
		}
		else
		{
			return E_NOK;
		}
		/*******set the bits of the event ***********/
		MCUCR |= Event_Lut[ExternIntCfg -> ExternInt_Event];
		break;
	case EXTRN_INT_1 :
		/*******set the call back fun ***********/
		if(NULL != (ExternIntCfg -> ExternInt_CBF_Ptr))
		{
			Interrupt_CBF_ptr[ExternIntCfg->ExternInt_No] = ExternIntCfg -> ExternInt_CBF_Ptr;
		}
		else
		{
			return E_NOK;
		}
		MCUCR |= Event_Lut[ExternIntCfg -> ExternInt_Event];
		break;
	case EXTRN_INT_2 :

		/*******set the call back fun ***********/
		if(NULL != ExternIntCfg -> ExternInt_CBF_Ptr)
		{
			Interrupt_CBF_ptr[ExternIntCfg->ExternInt_No] = ExternIntCfg -> ExternInt_CBF_Ptr;
		}
		else
		{
			return E_NOK;
		}

		switch(ExternIntCfg -> ExternInt_Event  )
		{
		case RISING_EDGE:
			MCUCSR |= SET_RAISING_EDGE;
			break;
		case FALLING_EDGE:
			MCUCSR &= SET_FALLING_EDGE;
			break;
		default:
			return E_NOK;
			break;
		}
		break;
	default :
		return E_NOK;
		break;
	}
	Init_Staut[ExternIntCfg -> ExternInt_No] = EXT_INTERRUPT_INITIATED;
	return E_OK;
}



/*
 * Function: ExternInt_Enable
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2).
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: Enable the required external interrupt from GICR but to allow the interrupt global interrupt must be set.
 */
ERROR_STATUS ExternInt_Enable(uint8 ExternInt_No)
{
	if(Init_Staut[ExternInt_No] == EXT_INTERRUPT_INITIATED)
	{
		switch(ExternInt_No)
		{
		case EXTRN_INT_0 :
			/*******set the bit of the interrupt ***********/
			GICR |= Interrupt_Lut[EXTRN_INT_0];
			break;
		case EXTRN_INT_1 :
			GICR |= Interrupt_Lut[EXTRN_INT_1];
			break;
		case EXTRN_INT_2 :
			GICR |= Interrupt_Lut[EXTRN_INT_2];
			break;
		default :
			return E_NOK;
			break;
		}
		return E_OK;
	}
	else
	{
		return E_NOK;
	}
}



/*
 * Function: ExternInt_SetEvent
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2).
 * 	       InterruptEvent -> indicate required event for INT0 and INT1 there are 4 events to check (RISING_EDGE,FALLING_EDGE,LOW_LEVEL,LOGICAL_CHANGE).
							  But for Interrupt 2 there are only Two cases (Rising,Falling)
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: function is used to set event of required external interrupt.
 * 				note: usually used to poll on the flag.
 */
ERROR_STATUS ExternInt_SetEvent(uint8 ExternInt_No,uint8 InterruptEvent)
{
	switch(ExternInt_No)
		{
		case EXTRN_INT_0 :
			/*******set the bits of the event ***********/
			MCUCR &= EXT0_MASK;
			MCUCR |= Event_Lut[InterruptEvent];
			break;
		case EXTRN_INT_1 :
			MCUCR &= EXT1_MASK;
			MCUCR |= Event_Lut[InterruptEvent];
			break;
		case EXTRN_INT_2 :
			switch(InterruptEvent)
			{
			case RISING_EDGE:
				MCUCSR |= SET_RAISING_EDGE;
				break;
			case FALLING_EDGE:
				MCUCSR &= SET_FALLING_EDGE;
				break;
			default:
				return E_NOK;
				break;
			}
			break;
		default :
			return E_NOK;
			break;
		}
	return E_OK;

}


/*
 * Function: ExternInt_GetStatus
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2).
 * Outputs: *status -> points to the value of selected interrupt flag.
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: function is used to get flag of required interrupt status.
 * 				note: usually used to poll on the flag.
 */
ERROR_STATUS ExternInt_GetStatus(uint8 ExternInt_No,uint8 *Status)
{
	if(ExternInt_No <= EXTRN_INT_2)
	{
		*Status =  (GIFR &Interrupt_Lut[ExternInt_No]) /Interrupt_Lut[ExternInt_No];
		return E_OK;
	}
	else
	{
		return E_NOK;
	}
}


/*
 * Function: ExternInt_Disable
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2)
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: Disable the required external interrupt.
 */
ERROR_STATUS ExternInt_Disable(uint8 ExternInt_No)
{
	if(ExternInt_No <= EXTRN_INT_2)
	{
		GICR &= ~(Interrupt_Lut[EXTRN_INT_0]);
		Init_Staut[ExternInt_No] = EXT_INTERRUPT_NOT_INITIATED;
		return E_OK;
	}
	else
	{
		return E_NOK;
	}

}


ISR(INT0_vect)
{
	Interrupt_CBF_ptr[EXTRN_INT_0]();
}


ISR(INT1_vect)
{
	Interrupt_CBF_ptr[EXTRN_INT_1]();
}


ISR(INT2_vect)
{
	Interrupt_CBF_ptr[EXTRN_INT_2]();
}

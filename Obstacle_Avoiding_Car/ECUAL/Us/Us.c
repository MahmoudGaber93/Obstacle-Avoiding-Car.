/*
 *      file name  : "Us.c"
 *      Author:  GABER
 *      version : 1
 *
 */

#ifndef US_H_
#define US_H_

/*********************************************
 *				Includes  					 *
 ********************************************/
#include "Us1.h"

/*********************************************
 *				Defines  					 *
 ********************************************/
#define      _ZERO                            (0)
#define      MAX_COUNTS                      (255)
#define      INITIALIZED_TRIGGER_COUNTER      (2)
#define      AIR_SPEED                        (343)
#define      DIVISION_FACTOR                  (20000)

/************************************************************************/
/*		                     GLOBAL VARIABLES	                        */
/************************************************************************/
uint32  gu32_Us_Time = _ZERO;



/************************************************************************/
/*				   functions' prototypes	                            */
/************************************************************************/

/****************************************************************************************************
 * Function Name   : Us_Init.                                                                       *
 * Input Parameters : None.                                                                         *                                                                   *                                                                                                *
 * Output Parameters: None.                                                                         *
 * In_Out Parameters: None.                                                                         *
 * Return           : ERROR_STATUS.                                                                 *
 * Description      : This function initiate the UltraSonic by set the value of the Dio_Dirction of *
 *                    the pin trigger as an output then initiate the ICU to use it to calculate the *
 *                    On time of the ECHO signal.                                                   *
 *                                                                                                  *
 *                                                                                                  *
 ***************************************************************************************************/
ERROR_STATUS Us_Init(void)
{
	/*trigger pin cfg*/
	DIO_Cfg_s Us_Pin_Cfg_St = {_ZERO};
	Us_Pin_Cfg_St.GPIO      = GPIOB;
	Us_Pin_Cfg_St.pins      = BIT0;
	Us_Pin_Cfg_St.dir       = OUTPUT;
	DIO_init(&Us_Pin_Cfg_St);

	/*Icu cfg*/
	Icu_cfg_s Icu_Cfg_St;
	Icu_Cfg_St.ICU_Ch_No    = EXTRN_INT_2;
	Icu_Cfg_St.ICU_Ch_Timer = ICU_TIMER_CH0;
	Icu_Init(&Icu_Cfg_St);

	return E_OK;
}


/**************************************************************************************************
 * Function Name    : Us_Trigger.                                                                 *
 * Input Parameters : None.                                                                       *                                                                              *                                                                                                *
 * Output Parameters: None.                                                                       *
 * In_Out Parameters: None.                                                                       *
 * Return           : ERROR_STATUS                                                                *
 * Description      : This function send the  pulse, to trigger pin of the UltraSonic to enhance  *
 *                    the  ECUO signal                                                            *
 *************************************************************************************************/
ERROR_STATUS Us_Trigger(void)
{
	   Timer_Start(ICU_TIMER_CH0,MAX_COUNTS);

	uint8  au8_counter= INITIALIZED_TRIGGER_COUNTER;
	DIO_Write(GPIOB,BIT0,HIGH);
	while(au8_counter !=_ZERO)
	{
		au8_counter--;
	}
	DIO_Write(GPIOB,BIT0,LOW);

return E_NOK;

}





/**************************************************************************************************
 * Function Name    : Us_GetDistance.                                                             *
 * Input Parameters : pointer to uint16 that will return the distance on it.                      *                                                                                              *                                                                                                *
 * Output Parameters: Distance.                                                                   *
 * In_Out Parameters: None.                                                                       *
 * Return           : ERROR_STATUS.                                                               *
 * Description      : This function get the time of high signal of ECUO pulse, then               *
 *                    use it to calculate the distance, then return the value in the              *
 *                    Distance parameter.                                                         *
 *************************************************************************************************/


ERROR_STATUS Us_GetDistance(uint16 *Distance)
{
	Icu_RiseToFall(&gu32_Us_Time);
	*Distance =  ((gu32_Us_Time * AIR_SPEED) /DIVISION_FACTOR);
	return E_OK;
}


#endif /* US_H_ */

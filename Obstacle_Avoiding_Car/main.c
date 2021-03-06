/*
 *   File Name : main
 *   Author    : GABER
 *   Version   : 1
 */

/************************************************************************/
/*				                INCLUDES		            	        */
/************************************************************************/

#include "interrupt.h"
#include"car_sm.h"


/************************************************************************/
/*				           MAIN IMPLEMENTATION		                    */
/************************************************************************/

void main(void)
{
/********************ENABLE GLOBAL INTERRRUPT ****************************/
	sei();

/*********************Initial the state machine ***************************/
	Car_SM_Init();

	while(1)
	{
/***************************update the state of the car*********************/
		Car_SM_Update();
	}

}

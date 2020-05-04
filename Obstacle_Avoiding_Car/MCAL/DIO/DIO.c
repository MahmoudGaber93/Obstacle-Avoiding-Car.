/*
 *     file name :DIO.c
 *      Author   : GABER
 *      Version  : 1
 */



/************************************************************************/
/*				                INCLUDES		            	        */
/************************************************************************/
#include "DIO.h"


/************************************************************************/
/*				           FUNCTION IMPLEMENTATION	                    */
/************************************************************************/

/*
*Input: DIO_Cfg_s -> to get PORT name, pins to be initiated and the required direction
*Output: No output
*In/Out:
*Description: This function can set the direction of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_init (DIO_Cfg_s *DIO_info)
{

/***********Check the boundary of the number of pins*/
	if((DIO_info -> pins )<= FULL_PORT)
	{
		switch(DIO_info ->GPIO)
		{
		case GPIOA:
			switch(DIO_info -> dir)
			{
			case OUTPUT:
				PORTA_DIR |=(DIO_info -> pins);
				break;
			case INPUT:
				PORTA_DIR &=~(DIO_info -> pins);
				break;
			default:
				return E_NOK;
				break;
			}
			break;
		case GPIOB:
			switch(DIO_info -> dir)
			{
			case OUTPUT:
				PORTB_DIR |=(DIO_info -> pins);
				break;
			case INPUT:
				PORTB_DIR &=~(DIO_info -> pins);
				break;
			default:
				return E_NOK;
				break;
			}
			break;
		case GPIOC:
			switch(DIO_info -> dir)
			{
			case OUTPUT:
				PORTC_DIR |=(DIO_info -> pins);
				break;
			case INPUT:
				PORTC_DIR &=~(DIO_info -> pins);
				break;
			default:
				return E_NOK;
				break;
			}
			break;
		case GPIOD:
			switch(DIO_info -> dir)
			{
			case OUTPUT:
				PORTD_DIR |=(DIO_info -> pins);
				break;
			case INPUT:
				PORTD_DIR &=~(DIO_info -> pins);
				break;
			default:
				return E_NOK;
				break;
			}
			break;
			default:
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
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*		value 	-> The desired value
*Output: No output
*In/Out: No In/Out
*Description: This function can set the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Write (uint8 GPIO, uint8 pins, uint8 value)
{
    if( pins <= FULL_PORT)
    {
    	switch(GPIO)
    	{
    	case GPIOA:
    		switch(value)
    		{
    		case HIGH:
    			PORTA_DATA |=pins;
    			break;
    		case LOW:
    			PORTA_DATA &=~ pins;
    			break;
    		default:
    			return E_NOK;
    			break;
    		}
        	 break;
		case GPIOB:
			switch(value)
			{
			case HIGH:
				PORTB_DATA |=pins;
				break;
			case LOW:
				PORTB_DATA &=~ pins;
				break;
			default:
				return E_NOK;
				break;
			}
			break;
		case GPIOC:
			switch(value)
			{
			case HIGH:
				PORTC_DATA |= pins;
				break;
			case LOW:
				PORTC_DATA &=~ pins;
				break;
			default:
				return E_NOK;
				break;
			}
			break;
		case GPIOD:
			switch(value)
			{
			case OUTPUT:
				PORTD_DATA |=pins;
				break;
			case INPUT:
				PORTD_DATA &=~ pins;
				break;
			default:
				return E_NOK;
				break;
			}
			break;
			default:
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
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*Output: data -> the acquired data wether it was PORT data or pins data
*In/Out:
*Description: This function gets the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Read (uint8 GPIO,uint8 pins, uint8 *data)
{
	if( pins <= PIN7)
	{
		switch(GPIO)
		{
		case GPIOA:

			*data = (PORTA_PIN &pins)/(pins);
			break;
		case GPIOB:
			*data = (PORTB_PIN &pins)/(pins);
			break;
		case GPIOC:
			*data = (PORTC_PIN &pins)/(pins);
			break;
		case GPIOD:
			*data = (PORTD_PIN &pins)/(pins);
			break;
		default:
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
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*Output: data -> No output
*In/Out:
*Description: This function toggles the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Toggle (uint8 GPIO, uint8 pins)
{

	if( pins <= FULL_PORT)
	{
		switch(GPIO)
		{
		case GPIOA:
			PORTA_DATA ^=pins;
			break;
		case GPIOB:
			PORTB_DATA ^=pins;
			break;
		case GPIOC:
			PORTC_DATA ^=pins;
			break;
		case GPIOD:
			PORTD_DATA ^=pins;
			break;
		default:
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

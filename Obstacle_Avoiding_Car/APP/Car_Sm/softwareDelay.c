/*
 * softwareDelay.c
 *

 *      Author: Gaber
 */

/* INCLUDES -----------------------------------------------------------*/
#include "std_types.h"
#include "softwareDelay.h"

/*APIs IMPLIMENTATION ----------------------------------------------------*/
void SwDelay_ms(uint32 n)
{
	uint16  au16_scale = 545;
	uint32 i=0;
	for(;i<(n * au16_scale);)
	{
		i++;
	}

}

/*
 * common_macros.h
 *
 *  Created on: Dec 22, 2019
 *      Author: GABER
 */

#ifndef SPRINTS_ZAYED_AVR_STATICDESIGN_SRC_MCAL_COMMON_MACROS_H_
#define SPRINTS_ZAYED_AVR_STATICDESIGN_SRC_MCAL_COMMON_MACROS_H_


/************************************************************************/
/*			       			BIT_MATH defines       	                    */
/************************************************************************/

/* Set VARiable's Certin BIT High */

#define SET_BIT(VAR,BIT_NB) 		(VAR)|=(BIT_NB)

/* Set VARiable's Certin BIT Low */

#define CLR_BIT(VAR,BIT_NB) 		(VAR)&=~(BIT_NB)

/* Toggle VARiable's Certin BIT */

#define TOGGLE_BIT(VAR,BIT_NB) 		(VAR)^=(BIT_NB)

/* Assign VARiable's Certin BIT by a Certin VALue */

#define ASSIGN_BIT(VAR,BIT_NB,VAL)  (VAR)=((VAR&(~(BIT_NB)))|(VAL&BIT_NB))

/* Get VARiable's Certin BIT Value */

#define GET_BIT(VAR,BIT_NB) 		(((VAR)&(BIT_NB))/(BIT_NB))



#define ERROR_STATUS uint8

#define E_NOK 1
#define E_OK  0

#endif /* SPRINTS_ZAYED_AVR_STATICDESIGN_SRC_MCAL_COMMON_MACROS_H_ */

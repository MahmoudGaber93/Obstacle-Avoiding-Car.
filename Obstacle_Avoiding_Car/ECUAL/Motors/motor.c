
/*
 * File		: Motor.h
 * Author	: Gaber
 * Version	: 1.1
 */

/*********************************************
 *				Includes  					 *
 ********************************************/
#include "motor.h"

/*********************************************
 *				Defines  					 *
 ********************************************/
#define      MOTOR_INIT          (1)
#define      MOTOR_NOT_INIT      (0)
#define      ZERO_               (0)
#define      ONE_                (1)
#define      FREQUANCY         (1000)

/************************************************************************/
/*		                     GLOBAL VARIABLES	                        */
/************************************************************************/

uint8 gu8_Motor1_status =ZERO_;
uint8 gu8_Motor2_status =ZERO_;



/*********************************************
 *	  	   Function prototypes				 *
 ********************************************/

/**
 * Func			: Motor_Init
 * Input		: motorNumber
 *	  	  		  #def
 *	  	  		  number of motor (MOTOR_1, MOTOR_2)
 * output		: none
 * In out		: none
 * Return		: ERROR_STATUS
 * Description	: Initialize (Enable, IN1, IN2) pins as digital outputs.
 */
ERROR_STATUS Motor_Init(uint8 Motor_Number)
{

	DIO_Cfg_s Motor_Pin_Cfg;
	Motor_Pin_Cfg.GPIO = GPIOD;
	Motor_Pin_Cfg.dir= OUTPUT;

	Pwm_Cfg_s_t Motor_Pwm_Cfg_St;

	switch(Motor_Number)
	{
	case MOTOR_1:
		Motor_Pin_Cfg.pins =  BIT2 | BIT3;
		Motor_Pwm_Cfg_St.Channel = PWM_CH1A;
		Motor_Pwm_Cfg_St.Prescaler = PWM_PRESCALER_64;
		gu8_Motor1_status = MOTOR_INIT;
		break;
	case MOTOR_2:
		Motor_Pin_Cfg.pins =  BIT7 | BIT6;
		Motor_Pwm_Cfg_St.Channel = PWM_CH1B;
		Motor_Pwm_Cfg_St.Prescaler = PWM_PRESCALER_64;
		gu8_Motor2_status = MOTOR_INIT;
		break;
	default :
		return E_NOK;
		break;
	}
	DIO_init(&Motor_Pin_Cfg);
	Pwm_Init(&Motor_Pwm_Cfg_St);
	return E_OK;

}

/**
 * Fun		 : Motor_Direction
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2

			   @param Motor_Direction
			   #define
			   Stop, Forward, Backword
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: Controls the motor direction from getting the motor number and the direction.
 */
ERROR_STATUS Motor_Direction(uint8 Motor_Number, uint8 Motor_Direction)
{
	switch(Motor_Number)
	{
	case MOTOR_1 :
		switch(Motor_Direction)
		{
		case MOTOR_STOP:
			DIO_Write(GPIOD,BIT2,LOW);
			DIO_Write(GPIOD,BIT3,LOW);
			break;
		case MOTOR_FORWARD:
			DIO_Write(GPIOD,BIT2,HIGH);
			DIO_Write(GPIOD,BIT3,LOW);
			break;
		case MOTOR_BACKWORD:
			DIO_Write(GPIOD,BIT2,LOW);
			DIO_Write(GPIOD,BIT3,HIGH);
			break;
		default:
			return E_NOK;
			break;
		}
		break;
		case MOTOR_2 :
			switch(Motor_Direction)
			{
			case MOTOR_STOP:
				DIO_Write(GPIOD,BIT6,LOW);
				DIO_Write(GPIOD,BIT7,LOW);
				break;
			case MOTOR_FORWARD:
				DIO_Write(GPIOD,BIT6,HIGH);
				DIO_Write(GPIOD,BIT7,LOW);
				break;
			case MOTOR_BACKWORD:
				DIO_Write(GPIOD,BIT6,LOW);
				DIO_Write(GPIOD,BIT7,HIGH);
				break;
			default:
				return E_NOK;
				break;
			}
			break;
			default:
				return E_NOK;

	}
	return E_OK;
}

/**
 * Fun		 : Motor_Start
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2
			   @param Mot_Speed
			   set the motor speed
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: Start the motor.
 */
ERROR_STATUS Motor_Start(uint8 Motor_Number, uint8 Mot_Speed)
{
	switch(Motor_Number)
	{
	case MOTOR_1:
		Pwm_Start(PWM_CH1A,Mot_Speed,1000);

		break;
	case MOTOR_2:
		Pwm_Start(PWM_CH1B,Mot_Speed,1000);
		break;
	default :
		return E_NOK;
		break;
	}
	return E_OK;
}

/**
 * Fun		 : Motor_SpeedUpdate
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2
			   @param Mot_Speed
			   set the motor speed
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: Controls the motor speed from getting the motor number and the speed.
 */
ERROR_STATUS Motor_SpeedUpdate(uint8 Motor_Number, uint8 Speed)
{
	switch(Motor_Number)
	{
	case MOTOR_1:
		Pwm_Update(PWM_CH1A,Speed,FREQUANCY);

		break;
	case MOTOR_2:
		Pwm_Update(PWM_CH1B,Speed,FREQUANCY);
		break;
	default :
		return E_NOK;
		break;
	}
	return E_OK;
}

/**
 * Fun		 : Motor_Stop
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: stop the motor.
 */
ERROR_STATUS Motor_Stop(uint8 Motor_Number)
{
	switch(Motor_Number)
	{
	case MOTOR_1:
		DIO_Write(GPIOD,BIT2,LOW);
		DIO_Write(GPIOD,BIT3,LOW);
		gu8_Motor1_status = MOTOR_NOT_INIT;


		break;
	case MOTOR_2:
		DIO_Write(GPIOD,BIT6,LOW);
		DIO_Write(GPIOD,BIT7,LOW);
		gu8_Motor2_status = MOTOR_NOT_INIT;


		break;
	default :
		return E_NOK;
		break;
	}
	return E_OK;
}

/**
 * Func			: Motor_GetStatus
 * Input		: Motor_Number
 *	  	  		 #def
 *	  	  		 number of motor (MOTOR_1, MOTOR_2)
 * output		: none
 * In out		: none
 * Return		: Initialization_STATUS
 * Description	: Returns status of the motor whether intialized or not
 */
uint8 Motor_GetStatus(uint8 Motor_Number)
{
	uint8 status=ZERO_;
	switch(Motor_Number)
	{
	case MOTOR_1:
		status = gu8_Motor1_status ;
		break;
	case MOTOR_2:
		status = gu8_Motor2_status ;
		break;
	default :
		break;
	}
	return status ;

}


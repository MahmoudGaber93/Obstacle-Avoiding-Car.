/*
 * File		: car_sm.h
 * Created	: 12/21/2019 5:56:37 PM
 * Author	: GABER
 * Version	: v1.0
 */

/*************************************************************/
/*                     INCLUDES                              */
/*************************************************************/
#include "car_sm.h"

/*************************************************************/
/*                     MACROS                             */
/*************************************************************/

/**************SPEEDS******************/
#define SPEED_80             (80)
#define SPEED_30             (50)
#define SPEED_ZERO           (0)
#define ROTATE_SPEED         (60)

/***************DISTANCE******************/
#define DISTANCE_50          (50)
#define DISTANCE_30          (30)


/***************CAR STATUS******************/
#define STOP_CAR             (0)
#define CAR_FROWARD_80       (1)
#define CAR_FROWARD_30       (2)
#define BACKWARD_CAR         (3)
#define CAR_ROTAT_RIGHT      (4)

static uint8  gu8_Car_Stauts = CAR_FROWARD_80;
static uint16 gu16_Distance = 0;
char buffer[16] ={0};

#if 0
void swap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}

// function to reverse buffer[i..j]
char* reverse(char *buffer, uint32 i, uint32 j)
{
	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}

// Iterative function to implement itoa() function in C
char* itoa_(uint32 value, char* buffer, uint32 base)
{
	// invalid input
	if (base < 2 || base > 32)
		return buffer;

	// consider absolute value of number
	int n = abs(value);

	int i = 0;
	while (n)
	{
		int r = n % base;

		if (r >= 10)
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;

		n = n / base;
	}

	// if number is 0
	if (i == 0)
		buffer[i++] = '0';

	// If base is 10 and value is negative, the resulting string
	// is preceded with a minus sign (-)
	// With any other base, value is always considered unsigned
	if (value < 0 && base == 10)
		buffer[i++] = '-';

	buffer[i] = '\0'; // null terminate string

	// reverse the string and return it
	return reverse(buffer, 0, i - 1);
}
#endif
/*************************************************************/
/*                     FUNCTIONS IMPLEMENTATION              */
/*************************************************************/

/*
 * Fun----------: ERROR_STATUS Car_SM_Init(void);
 * Input--------: Nothing
 * Output-------: Nothing
 * Return-------: ERROR_STATUES
 * -------------- #E_Ok	->function operation is okay, no error
 * -------------- #E_Nok->function operation faild, error exists
 * Description--: Initiate the car state machine with state"forward at speed 80%",
 * -------------- And initiate steering and ultrasonic functions
 */

ERROR_STATUS Car_SM_Init(void)
{

	Steering_Init();
	Us_Init();
	//LCD_init();


	return E_OK;

}

/*
 * Fun----------: ERROR_STATUS Car_SM_Update(void);
 * Input--------: Nothing
 * Output-------: Nothing
 * Return-------: ERROR_STATUES
 * -------------- #E_Ok	->function operation is okay, no error
 * -------------- #E_Nok->function operation faild, error exists
 * Description--: changes car state according to the ultrasonic input
 * -------------- And represents the motors output for each state
 */
ERROR_STATUS Car_SM_Update(void)
{
	Us_Trigger();
	Us_GetDistance(&gu16_Distance);

	switch(gu8_Car_Stauts)
	{
	case STOP_CAR:
		Steering_SteerCar(CAR_STOP,SPEED_ZERO);
		/*delay*/
		SwDelay_ms(5);
		gu8_Car_Stauts = CAR_ROTAT_RIGHT;
		break;

	case CAR_FROWARD_80:
		Steering_SteerCar(CAR_FORWARD,SPEED_80);
		gu8_Car_Stauts = CAR_FROWARD_30;
		break;

	case CAR_FROWARD_30:
		Steering_SteerCar(CAR_FORWARD,SPEED_30);
		if(gu16_Distance == DISTANCE_30)
		{
			gu8_Car_Stauts = STOP_CAR;

		}
		else if(gu16_Distance >DISTANCE_50)
		{
			gu8_Car_Stauts = CAR_FROWARD_80;
		}
		else if(DISTANCE_30 < gu16_Distance  && gu16_Distance < DISTANCE_50)
		{
			gu8_Car_Stauts = CAR_FROWARD_30;
		}
		else if( DISTANCE_30 >gu16_Distance)
		{
			gu8_Car_Stauts = BACKWARD_CAR;
		}

		break;
	case BACKWARD_CAR:
		Steering_SteerCar(CAR_BACKWARD,SPEED_30);
		if(gu16_Distance == DISTANCE_30)
		{
			gu8_Car_Stauts = STOP_CAR;
		}
		else if(gu16_Distance >DISTANCE_50)
		{
			gu8_Car_Stauts = CAR_FROWARD_80;
		}
		else if(DISTANCE_30 < gu16_Distance  && gu16_Distance < DISTANCE_50)
		{
			gu8_Car_Stauts = CAR_FROWARD_30;
		}
		else if( DISTANCE_30 >gu16_Distance)
		{
			gu8_Car_Stauts = BACKWARD_CAR;
		}

		break;
	case CAR_ROTAT_RIGHT:
		Steering_SteerCar(CAR_RIGHT,ROTATE_SPEED);
		//delay(according to 90 degree)
		SwDelay_ms(220);
		Steering_SteerCar(CAR_STOP,SPEED_ZERO);
		SwDelay_ms(5);

		if(gu16_Distance == DISTANCE_30)
		{
			gu8_Car_Stauts = STOP_CAR;
		}
		else if(gu16_Distance >DISTANCE_50)
		{
			gu8_Car_Stauts = CAR_FROWARD_80;
		}
		else if(DISTANCE_30 < gu16_Distance  && gu16_Distance < DISTANCE_50)
		{
			gu8_Car_Stauts = CAR_FROWARD_30;
		}
		else if( DISTANCE_30 >gu16_Distance)
		{
			gu8_Car_Stauts = BACKWARD_CAR;
		}

		break;
	default:
		return E_NOK;
		break;
	}

	/*LCD_goto_xy(0,0);

	itoa_(gu16_Distance,buffer,10);
	LCD_send_string(buffer);
	LCD_send_string(" ");*/

	return E_OK;
}


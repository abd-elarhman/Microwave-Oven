/**
 * @file main.c "Microwave Oven Control System"
 * @author Abdelrahman Ahmed Moussa (abdelrahman.ahmed0599@gmail.com , https://www.linkedin.com/in/-abdelrahman-ahmed)
 * @brief  This is the main code file of a microwave oven control system implemented on a avr atmega32 microcontroller.
 *		   It includes the main application logic for managing the microwave oven's operations.
 *		   The system is designed to handle various states and events such as starting the microwave,
 *		   pausing, adjusting the timer, and responding to (door open/close) and (food exist/not_exist) events.
 *		   
 *		   The code is structured around a state machine that transitions
 *		   between states based on user inputs and sensor readings. It includes/
 *		   functionality for reading keypad inputs, updating the display,
 *		   and controlling the microwave's heating elements, motor, and lamp.
 *		   
 *		   Key components of the system include:
 *			- State Machine: Manages the different states of the microwave operation and transitions between them.
 *			- Display: Shows the current status, timer, and prompts to the user.
 *			- Keypad: Allows the user to interact with the system by setting the timer and responding to prompts.
 *			- System Control: Handles the activation and deactivation of system components such as the heater, motor, and lamp.
 *		   
 *		   The main function initializes the system and contains the super loop where the state machine logic is executed.
 *
 * @version 0.1
 * @date 2024-03-10
 * @copyright Copyright (c) 2024
 * 
 */

/************************************************************************/
/*                            Inclusion                                 */
/************************************************************************/
/*
* LIB
*/
#include "MemMap.h"

#include "Std_Types.h"
#include "Utils_BitMath.h"
#include "Utils_interrupt.h"
 

//u16 CC1;

/*
* Include MCAL layer files
*/
#include "MCU_config.h"
#include "DIO_Interface.h"
#include "TIMERS_Interfacing.h"


/*
* Include HAL layer files

*/
#include "LCD_Interface.h"
#include "Keypad_interface.h"

/* ===================================================================== */
/*                           LCD                                         */
/* ===================================================================== */
enum {
	LCD_ID1
	};


/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                              State Machine                                   */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*--------------------------------*/
/*                                */
/*            Macros              */
/*                                */
/*--------------------------------*/
#define    HEATER  PC0
#define    LAMP    PC1
#define    MOTOR   PC2

#define    START_PB       PA0
#define    STOP_PB        PA1
#define    DOOR_SENSOR    PA2
#define    WEIGHT_SENSOR  PA3

/*TESTING*/
/*
#define	   PLUS_PB        PA4
#define    MINUS_PB		  PA5
#define    DOOR_LAMP      PA6
#define    FOOD_LAMP      PA7
*/

/*--------------------------------*/
/*                                */
/*           Typedef              */
/*                                */
/*--------------------------------*/
typedef enum 
{
	IDLE_OFF_STATE,
	ON_STATE,
	PAUSE_STATE,
	DOOR_OPENED_STATE
	}STATE_t;
	
typedef enum 
{
	START_EVENT,
	CANCEL_EVENT,
	DOOR_OPENED_EVENT,
	DOOR_CLOSED_EVENT,
	FOOD_WEIGHT_VALID_EVENT,
	FOOD_WEIGHT_INVALID_EVENT,
	TIME_ADJUSTED_EVENT,
	TIME_NOT_ADJUSTED_EVENT,
	NO_TRANSATION
	}TRANSATION_t;

/*--------------------------------*/
/*                                */
/*          Global Var            */
/*                                */
/*--------------------------------*/
STATE_t currentState=IDLE_OFF_STATE;
STATE_t historyState=IDLE_OFF_STATE;

TRANSATION_t transation;


volatile u16 opTime=0; /*< global becuase it used in (getTimeTransation(),getTimeUpdate(),resetTimer(),ISR) & volatile becuase it used in ISR */

Std_Bool_t isDisplayUpdateNeeded=STD_TRUE;
Std_Bool_t volatile isTimerUpdateNeeded=STD_FALSE;

u8 volatile secondUnits='0';
u8 volatile secondTens='0';
u8 volatile minuteUnits='0';
u8 volatile minuteTens='0';


/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                              Displaying                                      */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*--------------------------------*/
/*                                */
/*           Typedef              */
/*                                */
/*--------------------------------*/
typedef enum
{
	DISPLAY_DOOR_OPENED,
	DISPLAY_PUT_FOOD,
	DISPLAY_RESET,
	DISPLAY_DONE,
	DISPLAY_TIMER_NOT_ADJUSTED,
	DISPLAY_SET_TIMER_PLEASE,
	DISPLAY_RUNNING,
	DISPLAY_PAUSED,
	DISPLAY_NO_TRANSACTION
}DISPLAY_t;

/*--------------------------------*/
/*                                */
/*           Macros               */
/*                                */
/*--------------------------------*/
#define DISPLAY_STRING_ROW     4
#define DISPLAY_STRING_COL     1
#define DISPLAY_TIMER_ROW      3
#define DISPLAY_TIMER_COL      8

#define DISPLAY_DELAY          500
#define DISPLAY_BLINK_DELAY	   750
#define DISPLAY_EMPTY_LINE     "                    "

/*--------------------------------*/
/*                                */
/*          GLOBAL VAR            */
/*                                */
/*--------------------------------*/
const u8 *displayString[]={
							">>>>Door Opened<<<<<" ,
							">>>>>>Put Food<<<<<<" ,
							">>>>>>>RESET<<<<<<<<" ,
							">>>>>>>>DONE<<<<<<<<" ,
							">Timer Not Adjusted<" ,
							">>Set Timer Please<<" ,
							">>>>>>RUNNING<<<<<<<" ,
							">>>>>>>PAUSED<<<<<<<" 
							/*
							"    Door Opened     " ,
							"     Put Food       " ,
							"       RESET        " ,
							"        DONE        " ,
							" Timer Not Adjusted " ,
							"   Set Timer Please " ,
							"      RUNNING       " ,
							"      PAUSED        " ,*/
						  };


/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                            fUNCTION PROTOTYPES                               */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/*--------------------------------*/
/*                                */
/*      DISPLAY PROTOTYPES         */
/*                                */
/*--------------------------------*/
void displayStatus(DISPLAY_t display);
void display_update(DISPLAY_t display);
void timeDisplay(void);


/*--------------------------------*/
/*                                */
/*       KEYPAD PROTOTYPES        */
/*                                */
/*--------------------------------*/
u8 keypad_read_var;
void keyPad_read(void);

/*--------------------------------*/
/*                                */
/*   STATE MACHINE PROTOTYPES     */
/*                                */
/*--------------------------------*/
TRANSATION_t getTransation(void);
TRANSATION_t  getFoodTransation(void);
TRANSATION_t  getTimeTransation(void);
Std_Status_t  getTimeUpdate(void);
void resetTimer(void);

/*--------------------------------*/
/*                                */
/*       SYSTEM PROTOTYPES        */
/*                                */
/*--------------------------------*/
void system_timerOvf(void);
void system_OnOff(Std_OnOff_t status);
void system_init(void);

int main(void)
{
	/************************************************************************/
	/*                           Initialization                             */
	/************************************************************************/
	system_init();

	while (1) /*< Super_Loop */
	{
		transation=getTransation();
			
		switch (currentState)
		{
			/***************************************************/
			/*                IDLE_OFF_STATE                   */
			/***************************************************/
			case IDLE_OFF_STATE:
				keyPad_read();
				display_update(DISPLAY_SET_TIMER_PLEASE);
				
				switch (transation)
				{
					case START_EVENT:
						if (Dio_ReadPin(DOOR_SENSOR)==DIO_VOLT_HIGH) /*< check if door is opened*/
						{
							display_update(DISPLAY_DOOR_OPENED);	
						}
						else if (getFoodTransation()==FOOD_WEIGHT_INVALID_EVENT) /*< check if no food exist */
						{
							display_update(DISPLAY_PUT_FOOD);
						} 
						else if (getTimeTransation()==TIME_NOT_ADJUSTED_EVENT) /*< check if Timer not adjusted */
						{
							display_update(DISPLAY_TIMER_NOT_ADJUSTED);
						} 
						else /*< the system ready to run */
						{
							currentState=ON_STATE; /*< switch the current state to the state should be executed */
							isDisplayUpdateNeeded=STD_TRUE; /*< changing this flag to true indicate that the display_update() function must update the LCD*/
							historyState=IDLE_OFF_STATE;   /*< store the history state is needed for system_OnOff() function*/
						}
					break;
			
					case CANCEL_EVENT:
						display_update(DISPLAY_RESET);
						resetTimer();
						currentState=IDLE_OFF_STATE; /*< switch the current state to the state should be executed */
					break;	
					
					default:
						//do nothing
					break;	
				}
			break;
			
			/***************************************************/
			/*                   ON_STATE                      */
			/***************************************************/
			case ON_STATE:
			   
			   /* moved in else to avoid print running and run for a while before check and result of check is goto paused state
				display_update(DISPLAY_RUNNING);
				system_OnOff(STD_ON);
			    */
				if (Dio_ReadPin(DOOR_SENSOR)==DIO_VOLT_HIGH)//opened
				{
					system_OnOff(STD_OFF); /*< switch OFF the system to because next state is paused state*/
					display_update(DISPLAY_DOOR_OPENED);
					currentState=PAUSE_STATE; /*< switch the current state to the state should be executed */
					isDisplayUpdateNeeded=STD_TRUE; /*< changing this flag to true indicate that the display_update() function must update the LCD*/
				}
				else if (getFoodTransation()==FOOD_WEIGHT_INVALID_EVENT)
				{
					system_OnOff(STD_OFF); /*< switch OFF the system to because next state is paused state*/
					display_update(DISPLAY_PUT_FOOD); 
					currentState=PAUSE_STATE; /*< switch the current state to the state should be executed */
					isDisplayUpdateNeeded=STD_TRUE; /*< changing this flag to true indicate that the display_update() function must update the LCD*/
				}
				else if (getTimeUpdate()==STD_DONE) /*< timer is counting down to zero */
				{
					system_OnOff(STD_OFF); /*< switch OFF the system to because next state is paused state*/
					display_update(DISPLAY_DONE);
					currentState=IDLE_OFF_STATE; /*< switch the current state to the state should be executed */
					resetTimer();
				}
				else
				{
					// new commit 
					display_update(DISPLAY_RUNNING);
					system_OnOff(STD_ON);
				}
				
				
				switch (transation)
				{
					case START_EVENT:
						currentState=ON_STATE;
					break;
					
					case CANCEL_EVENT:
						system_OnOff(STD_OFF); /*< switch OFF the system to because next state is paused state*/
						currentState=PAUSE_STATE;/*< switch the current state to the state should be executed */
						isDisplayUpdateNeeded=STD_TRUE; /*< changing this flag to true indicate that the display_update() function must update the LCD*/
					break;
					
					default:
						//do nothing
					break;
				}				
			break;
			
			case PAUSE_STATE:	
				display_update(DISPLAY_PAUSED);
				switch (transation)
				{
					case START_EVENT: 
						currentState=ON_STATE;
						historyState=PAUSE_STATE;
						isDisplayUpdateNeeded=STD_TRUE; /*< changing this flag to true indicate that the display_update() function must update the LCD*/
					break;
					
					case CANCEL_EVENT:
						display_update(DISPLAY_RESET);
						resetTimer();
						currentState=IDLE_OFF_STATE; /*< switch the current state to the state should be executed */
					break;
					
					default:
						//do nothing
					break;
				}
			break;
			
			default:
				//do nothing
			break;
		}//switch (currentState)
	}//while(1) Super_Loop
}//main



/*--------------------------------*/
/*                                */
/*      DISPLAY FUNCTIONS         */
/*                                */
/*--------------------------------*/
void displayStatus(DISPLAY_t display)
{
	u8 i;
	if ( (display>=DISPLAY_DOOR_OPENED)&&(display<=DISPLAY_TIMER_NOT_ADJUSTED))
	{
		//"    Door Opened     " , "     Put Food       " , "       RESET        " , "        DONE        " , " Timer Not Adjusted "
		for (i=0 ; i<2 ;i++ )
		{
			//clear
			LCD_SetCursor(LCD_ID1,DISPLAY_STRING_ROW,DISPLAY_STRING_COL);
			LCD_WriteStr(LCD_ID1,(u8*)DISPLAY_EMPTY_LINE);
			
			_delay_ms(DISPLAY_BLINK_DELAY/2);
			//display
			LCD_SetCursor(LCD_ID1,DISPLAY_STRING_ROW,DISPLAY_STRING_COL);
			LCD_WriteStr(LCD_ID1,(u8*)displayString[display]);
			
			_delay_ms(DISPLAY_BLINK_DELAY);
		}
	}
	else if ((display>=DISPLAY_SET_TIMER_PLEASE)&&(display<=DISPLAY_PAUSED))
	{
		// "   Set Timer Please ", "      RUNNING       ",  "      PAUSED        "
		LCD_SetCursor(LCD_ID1,DISPLAY_STRING_ROW,DISPLAY_STRING_COL);
		LCD_WriteStr(LCD_ID1,(u8*)DISPLAY_EMPTY_LINE);
		LCD_SetCursor(LCD_ID1,DISPLAY_STRING_ROW,DISPLAY_STRING_COL);
		LCD_WriteStr(LCD_ID1,(u8*)displayString[display]);
		
		_delay_ms(DISPLAY_DELAY);
	}
	else if (display==DISPLAY_NO_TRANSACTION)
	{
		//do nothing
	}
	
}
void display_update(DISPLAY_t display)
{
	if ( (display>=DISPLAY_DOOR_OPENED)&&(display<=DISPLAY_TIMER_NOT_ADJUSTED))
	{
		//"    Door Opened     " , "     Put Food       " , "       RESET        " , "        DONE        " , " Timer Not Adjusted "
		displayStatus(display);
		isDisplayUpdateNeeded=STD_TRUE;
	}
	else if ((display>=DISPLAY_SET_TIMER_PLEASE)&&(display<=DISPLAY_PAUSED))
	{
		// "   Set Timer Please ", "      RUNNING       ",  "      PAUSED        "
		switch (currentState)
		{
			case IDLE_OFF_STATE:
			if (isDisplayUpdateNeeded==STD_TRUE)
			{
				isDisplayUpdateNeeded=STD_FALSE;
				displayStatus(display);//DISPLAY_SET_TIMER_PLEASE
				timeDisplay();
			}
			break;
			
			case ON_STATE:
			if (isDisplayUpdateNeeded==STD_TRUE)
			{
				isDisplayUpdateNeeded=STD_FALSE;
				displayStatus(display); // DISPLAY_RUNNING
			}
			if (isTimerUpdateNeeded==STD_TRUE)
			{
				isTimerUpdateNeeded=STD_FALSE;
				
				// calculate timer digits
				secondUnits= ( ( (u16)opTime%60) % 10)+ '0';
				secondTens=  ( ( (u16)opTime%60) / 10)+ '0';
				minuteUnits= ( ( (u16)opTime/60) % 10)+ '0';
				minuteTens=  ( ( (u16)opTime/60) / 10)+ '0';
				
				timeDisplay();
			}
			break;
			
			case PAUSE_STATE:
			if (isDisplayUpdateNeeded==STD_TRUE)
			{
				isDisplayUpdateNeeded=STD_FALSE;
				displayStatus(DISPLAY_PAUSED);
			}
			break;
			
			default:
				//do nothing
			break;
		}
		
	}
	
}
void timeDisplay(void)
{
	// clear
	LCD_SetCursor(LCD_ID1,DISPLAY_TIMER_ROW,DISPLAY_TIMER_COL);
	LCD_WriteStr(LCD_ID1,(u8*)DISPLAY_EMPTY_LINE);
	
	//diplay
	LCD_SetCursor(LCD_ID1,DISPLAY_TIMER_ROW,DISPLAY_TIMER_COL);
	LCD_WriteCh(LCD_ID1,minuteTens);
	LCD_WriteCh(LCD_ID1,minuteUnits);
	LCD_WriteCh(LCD_ID1,':');
	LCD_WriteCh(LCD_ID1,secondTens);
	LCD_WriteCh(LCD_ID1,secondUnits);
}


/*--------------------------------*/
/*                                */
/*       SYSTEM PROTOTYPES        */
/*                                */
/*--------------------------------*/
void system_OnOff(Std_OnOff_t status)
{
	if (status==STD_ON)
	{
		if ( (historyState==PAUSE_STATE) || (historyState==IDLE_OFF_STATE) )
		{
			historyState=ON_STATE;
			
			Dio_WritePin(HEATER,DIO_VOLT_HIGH);
			Dio_WritePin(LAMP,DIO_VOLT_HIGH);
			Dio_WritePin(MOTOR,DIO_VOLT_HIGH);
			
			
			TIMER_ONOFF(TIMER1,STD_ON);
			TIMER1_OVF_INT(STD_ENABLED);
		}
	}
	else if (status==STD_OFF)
	{
		Dio_WritePin(HEATER,DIO_VOLT_LOW);
		Dio_WritePin(LAMP,DIO_VOLT_LOW);
		Dio_WritePin(MOTOR,DIO_VOLT_LOW);
		
		
		TIMER_ONOFF(TIMER1,STD_OFF);
		TIMER1_OVF_INT(STD_DISABLED);
	}
	
}
void system_timerOvf(void)
{
	TIMER1_SetCounterValue((u16)65536-31250);
	if (opTime>0)
	{
		opTime--;
		isTimerUpdateNeeded=STD_TRUE;
	}
}
void system_init(void)
{
	Dio_Init();
	LCD_init(LCD_ID1);
	
	TIMER1_OVF_SetCallBack(system_timerOvf);
	TIMER1_Init(TIMER1_NORMAL,TIMER_OCx_MODE_DICONNECTED,TIMER_OCx_MODE_DICONNECTED,TIMER_Pre_CLK_256);
	TIMER_ONOFF(TIMER1,STD_OFF);
	TIMER1_SetCounterValue((u16)65536-31250);
	
	Global_Interrupt_Enable__asm();
	TIMER1_OVF_INT(STD_DISABLED);
	
	currentState=IDLE_OFF_STATE;
	historyState=IDLE_OFF_STATE;
	isDisplayUpdateNeeded=STD_TRUE;
	
	Dio_WritePin(HEATER,DIO_VOLT_LOW);
	Dio_WritePin(LAMP,DIO_VOLT_LOW);
	Dio_WritePin(MOTOR,DIO_VOLT_LOW);

	LCD_voidSetOption(LCD_ID1,LCD_ClearDisplay);
	LCD_SetCursor(LCD_ID1,1,1);
	LCD_WriteStr(LCD_ID1,(u8*)"     Microwave      ");
	LCD_SetCursor(LCD_ID1,2,1);
	LCD_WriteStr(LCD_ID1,(u8*)"  Abdelrahman Ahmed ");
}


/*--------------------------------*/
/*                                */
/*   STATE MACHINE PROTOTYPES     */
/*                                */
/*--------------------------------*/
TRANSATION_t getTransation(void)
{
	TRANSATION_t transation=NO_TRANSATION;
	 if (Dio_ReadPin(STOP_PB)==DIO_VOLT_LOW)
	{
		while(Dio_ReadPin(STOP_PB)==DIO_VOLT_LOW);
		transation=CANCEL_EVENT;
	}
	else if (Dio_ReadPin(START_PB)==DIO_VOLT_LOW)
	{
		while(Dio_ReadPin(START_PB)==DIO_VOLT_LOW);
		transation=START_EVENT;
	}
	
	return transation;
}
TRANSATION_t  getFoodTransation(void)
{
	TRANSATION_t foodTransation=FOOD_WEIGHT_INVALID_EVENT;
	
	//check if is food exist
	if (Dio_ReadPin(WEIGHT_SENSOR)==DIO_VOLT_LOW) 
	{
		foodTransation=FOOD_WEIGHT_VALID_EVENT;
	}
	else
	{
		foodTransation=FOOD_WEIGHT_INVALID_EVENT;
	}
	return foodTransation;
}
TRANSATION_t  getTimeTransation(void)
{
	TRANSATION_t timeTransation=TIME_ADJUSTED_EVENT;
	
	u16 min=0,sec=0;
	
	if (secondTens>='6')
	{
		secondTens='6';
		secondUnits='0';
	} 
	else
	{
		//nothing
	}
	
	/*
	----------------------------------------------------------------
	|	minuteTens	|	minuteUnits	:	secondTens	|	secondUnits	|
	-----------------------------------------------------------------
	EX:
	(minuteTens=1 ,minuteUnits=0, secondTens=4, secondUnits=0)  ===>  (10:40) ten minutes and fourty seconds
	
	*/
	min=((minuteTens-'0')*10)+(minuteUnits-'0');
	sec=((secondTens-'0')*10)+(secondUnits-'0');
	opTime=((u16)min*60)+sec;
	
	if (opTime>0)
	{
		timeTransation=TIME_ADJUSTED_EVENT;
	} 
	else
	{
		timeTransation=TIME_NOT_ADJUSTED_EVENT;
	}
	return timeTransation;
}
Std_Status_t  getTimeUpdate(void)
{
	Std_Status_t status=STD_PENDING;	
	if (opTime>0)
	{
		status=STD_PENDING;
	}
	else
	{
		status=STD_DONE;
	}
	return status;
}

void resetTimer(void)
{
	secondUnits='0';
	secondTens='0';
	minuteUnits='0';
	minuteTens='0';
	opTime=0;
}
void keyPad_read(void)
{
	if(KEYPAD_GetKey(&keypad_read_var)==STD_OK)
	{
		isDisplayUpdateNeeded=STD_TRUE;
		
		minuteTens=minuteUnits;
		minuteUnits=secondTens;
		secondTens=secondUnits;
		secondUnits=keypad_read_var;
	}
	else
	{
		//nothing
	}
}



/**
void system_OnOff_OLD(Std_OnOff_t status)
{
	if (status==STD_ON)
	{
		if ( (historyState==PAUSE_STATE) || (historyState==IDLE_OFF_STATE) )
		{
			historyState=ON_STATE;
			
			Dio_WritePin(HEATER,DIO_VOLT_HIGH);
			Dio_WritePin(LAMP,DIO_VOLT_HIGH);
			Dio_WritePin(MOTOR,DIO_VOLT_HIGH);
			
			
			write_bit(TIMSK, TIMSK_TOIE1, STD_ENABLED);
			TIMER_setClk(TIMER1, TIMER_Pre_CLK_8);
		}
	}
	else if (status==STD_OFF)
	{
		if (historyState==ON_STATE)
		{
			historyState=currentState; 
			
			Dio_WritePin(HEATER,DIO_VOLT_LOW);
			Dio_WritePin(LAMP,DIO_VOLT_LOW);
			Dio_WritePin(MOTOR,DIO_VOLT_LOW);
			
			
			write_bit(TIMSK, TIMSK_TOIE1, STD_DISABLED);
			TIMER_setClk(TIMER1, TIMER_Pre_CLK_0);
		}
	}
	
}
*/
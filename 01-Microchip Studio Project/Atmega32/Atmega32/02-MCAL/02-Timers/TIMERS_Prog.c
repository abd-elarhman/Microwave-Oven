// lib
#include "Std_Types.h"
#include "MemMap.h"
#include "Utils_BitMath.h"
#include "Utils_interrupt.h"

// own module files
#include "TIMERS_Interfacing.h"
#include "TIMERS_Private.h"
#include "TIMERS_Lcfg.h"


/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                        Intialization Functions                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
/**
 * @brief Initialize TIMER0 with the specified parameters.
 *
 * This function initializes TIMER0 with the specified timer mode, compare mode, and clock select.
 *
 * @param timerMode The timer mode to be set for TIMER0.
 * @param compareMode The compare mode to be set for OC0.
 * @param CLk_select The clock select to be set for TIMER0.
 *
 * @return None.
 *
 */
void TIMER0_Init(TIMER0_Modes_t timerMode, TIMER_OC_MODE_t compareMode, TIMERS_Pre_CLK_t CLk_select)
{
	// [1] select timer mode
	TIMER_setTimer_Mode(TIMER0, timerMode);

	// [2] select oc mode
	TIMER_setOc_Mode(OC0, compareMode);

	// [3] set CLK
	Timer0_Clk = CLk_select;
	TIMER_setClk(TIMER0, Timer0_Clk);
}

/**
 * @brief Initialize TIMER1 with the specified parameters.
 *
 * This function initializes TIMER1 with the specified timer mode, compare mode, and clock select.
 *
 * @param timerMode The timer mode to be set for TIMER1.
 * @param OC1A_compareMode The compare mode to be set for OC1A.
 * @param OC1B_compareMode The compare mode to be set for OC1B.
 * @param CLk_select The clock select to be set for TIMER1.
 *
 * @return None.
 *
 */
void TIMER1_Init(TIMER1_Modes_t timerMode, TIMER_OC_MODE_t OC1A_compareMode, TIMER_OC_MODE_t OC1B_compareMode, TIMERS_Pre_CLK_t CLk_select)
{
	// [1] select timer mode
	TIMER_setTimer_Mode(TIMER1, timerMode);

	// [2] select oc mode
	TIMER_setOc_Mode(OC1A, OC1A_compareMode);
	TIMER_setOc_Mode(OC1B, OC1B_compareMode);

	// [3] set CLK
	Timer1_Clk = CLk_select;
	TIMER_setClk(TIMER1, Timer1_Clk);
}


/**
 * @brief Initialize TIMER2 with the specified parameters.
 *
 * This function initializes TIMER2 with the specified timer mode, compare mode, and clock select.
 *
 * @param timerMode The timer mode to be set for TIMER2.
 * @param compareMode The compare mode to be set for OC2.
 * @param CLk_select The clock select to be set for TIMER2.
 *
 * @return None.
 *
 */
void TIMER2_Init(TIMER2_Modes_t timerMode, TIMER_OC_MODE_t compareMode, TIMERS_Pre_CLK_t CLk_select)
{
	// [1] select timer mode
	TIMER_setTimer_Mode(TIMER2, timerMode);

	// [2] select oc mode
	TIMER_setOc_Mode(OC2, compareMode);

	// [3] set CLK
	Timer2_Clk = CLk_select;
	TIMER_setClk(TIMER2, Timer2_Clk);
}

/**
 * @brief Set the value of the timer.
 *
 * This function sets the value of the timer.
 *
 * @param timer The timer to set its value.
 * @param value The value to set the timer to.
 *
 * @return None.
 *
 */
void TIMER_ONOFF(TIMERn_t timer, Std_OnOff_t status)
{
	switch (timer)
	{
	case TIMER0:
		if (status == STD_ON)
		{
			write_masked_value(TCCR0, 0b00000111, Timer0_Clk);
		}
		else if (status == STD_OFF)
		{
			write_masked_value(TCCR0, 0b00000111, TIMER_Pre_CLK_0);
		}
		break;

	case TIMER1:
		if (status == STD_ON)
		{
			write_masked_value(TCCR1B, 0b00000111, Timer1_Clk);
		}
		else if (status == STD_OFF)
		{
			write_masked_value(TCCR1B, 0b00000111, TIMER_Pre_CLK_0);
		}
		break;

	case TIMER2:
		if (status == STD_ON)
		{
			write_masked_value(TCCR2, 0b00000111, Timer2_Clk);
		}
		else if (status == STD_OFF)
		{
			write_masked_value(TCCR2, 0b00000111, TIMER_Pre_CLK_0);
		}
		break;
	}
}


/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                         Interrupt Functions                                 */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

/**
 * @brief Enables or disables the Timer0 overflow interrupt.
 *
 * This function allows you to enable or disable the Timer0 overflow interrupt.
 *
 * @param status The status of the interrupt. Use `ENABLE` to enable the interrupt or `DISABLE` to disable it.
 */
void TIMER0_OVF_INT(Std_EnableDisable_t status)
{
	write_bit(TIMSK, TIMSK_TOIE0, status);
}

/**
 * @brief Enables or disables the Timer/Counter0 Compare Match interrupt.
 *
 * This function is used to enable or disable the Timer/Counter0 Compare Match interrupt.
 *
 * @param status The status of the interrupt. Use `ENABLE` to enable the interrupt or `DISABLE` to disable it.
 */
void TIMER0_COMP_INT(Std_EnableDisable_t status)
{
	write_bit(TIMSK, TIMSK_OCIE0, status);
}

/**
 * @brief Enables or disables the Timer1 overflow interrupt.
 *
 * This function allows you to enable or disable the Timer1 overflow interrupt.
 *
 * @param status The status of the interrupt. Use `ENABLE` to enable the interrupt or `DISABLE` to disable it.
 */
void TIMER1_OVF_INT(Std_EnableDisable_t status)
{
	write_bit(TIMSK, TIMSK_TOIE1, status);
}

/**
 * @brief Enables or disables the Timer1 Compare Match A interrupt.
 *
 * This function allows you to enable or disable the Timer1 Compare Match A interrupt.
 *
 * @param status The status of the interrupt. Use `ENABLE` to enable the interrupt or `DISABLE` to disable it.
 */
void TIMER1_COMPA_INT(Std_EnableDisable_t status)
{
	write_bit(TIMSK, TIMSK_OCIE1A, status);
}

/**
 * @brief Enables or disables the Timer1 Compare Match B interrupt.
 *
 * This function allows you to enable or disable the Timer1 Compare Match B interrupt.
 *
 * @param status The status of the interrupt. Use `ENABLE` to enable the interrupt or `DISABLE` to disable it.
 */
void TIMER1_COMPB_INT(Std_EnableDisable_t status)
{
	write_bit(TIMSK, TIMSK_OCIE1B, status);
}

/**
 * @brief Enables or disables the Timer1 Input Capture interrupt.
 *
 * This function allows you to enable or disable the Timer1 Input Capture interrupt.
 *
 * @param status The status of the interrupt. Use `ENABLE` to enable the interrupt or `DISABLE` to disable it.
 */
void TIMER1_CAPT_INT(Std_EnableDisable_t status)
{
	write_bit(TIMSK, TIMSK_TICIE1, status);
}

/*-------------*/
/*   Timer2    */
/*-------------*/

/**
 * @brief Enables or disables the Timer2 overflow interrupt.
 *
 * This function allows you to enable or disable the Timer2 overflow interrupt.
 *
 * @param status The status of the interrupt. Use `ENABLE` to enable the interrupt or `DISABLE` to disable it.
 */
void TIMER2_OVF_INT(Std_EnableDisable_t status)
{
	write_bit(TIMSK, TIMSK_TOIE2, status);
}

/**
 * @brief Enables or disables the Timer2 Compare Match interrupt.
 *
 * This function allows you to enable or disable the Timer2 Compare Match interrupt.
 *
 * @param status The status of the interrupt. Use `ENABLE` to enable the interrupt or `DISABLE` to disable it.
 */
void TIMER2_COMP_INT(Std_EnableDisable_t status)
{
	write_bit(TIMSK, TIMSK_OCIE2, status);
}

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                         Call Back Functions                                 */
/*                                                                             */
/*-----------------------------------------------------------------------------*/


/**
 * @brief Sets the callback function for TIMER0 overflow interrupt.
 * 
 * This function allows the user to set a callback function that will be executed when TIMER0 overflows.
 * 
 * @param LocalPtr Pointer to the callback function.
 */
void TIMER0_OVF_SetCallBack(void (*LocalPtr)(void))
{
	TIMER0_OVF_Fptr = LocalPtr;
}

/**
 * @brief Sets the callback function for TIMER0 Compare Match interrupt.
 * 
 * This function allows the user to set a callback function that will be executed when TIMER0 Compare Match occurs.
 * 
 * @param LocalPtr Pointer to the callback function.
 */
void TIMER0_COMP_SetCallBack(void (*LocalPtr)(void))
{
	TIMER0_COMP_Fptr = LocalPtr;
}

/**
 * @brief Sets the callback function for TIMER1 overflow interrupt.
 * 
 * This function allows the user to set a callback function that will be executed when TIMER1 overflows.
 * 
 * @param LocalPtr Pointer to the callback function.
 */
void TIMER1_OVF_SetCallBack(void (*LocalPtr)(void))
{
	TIMER1_OVF_Fptr = LocalPtr;
}

/**
 * @brief Sets the callback function for TIMER1 Compare Match A interrupt.
 * 
 * This function allows the user to set a callback function that will be executed when TIMER1 Compare Match A occurs.
 * 
 * @param LocalPtr Pointer to the callback function.
 */
void TIMER1_COMPA_SetCallBack(void (*LocalPtr)(void))
{
	TIMER1_COMPA_Fptr = LocalPtr;
}

/**
 * @brief Sets the callback function for TIMER1 Compare Match B interrupt.
 * 
 * This function allows the user to set a callback function that will be executed when TIMER1 Compare Match B occurs.
 * 
 * @param LocalPtr Pointer to the callback function.
 */
void TIMER1_COMPB_SetCallBack(void (*LocalPtr)(void))
{
	TIMER1_COMPB_Fptr = LocalPtr;
}

/**
 * @brief Sets the callback function for TIMER1 Input Capture interrupt.
 * 
 * This function allows the user to set a callback function that will be executed when TIMER1 Input Capture occurs.
 * 
 * @param LocalPtr Pointer to the callback function.
 */
void TIMER1_CAPT_SetCallBack(void (*LocalPtr)(void))
{
	TIMER1_CAPT_Fptr = LocalPtr;
}

/**
 * @brief Sets the callback function for TIMER2 overflow interrupt.
 * 
 * This function allows the user to set a callback function that will be executed when TIMER2 overflows.
 * 
 * @param LocalPtr Pointer to the callback function.
 */
void TIMER2_OVF_SetCallBack(void (*LocalPtr)(void))
{
	TIMER2_OVF_Fptr = LocalPtr;
}

/**
 * @brief Sets the callback function for TIMER2 Compare Match interrupt.
 * 
 * This function allows the user to set a callback function that will be executed when TIMER2 Compare Match occurs.
 * 
 * @param LocalPtr Pointer to the callback function.
 */
void TIMER2_COMP_SetCallBack(void (*LocalPtr)(void))
{
	TIMER2_COMP_Fptr = LocalPtr;
}

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                              PRIVATE Functions                              */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

/**
 * @brief Sets the timer mode for the specified timer.
 *
 * This function sets the timer mode for the specified timer. The timer mode determines
 * the behavior and operation of the timer.
 *
 * @param timer The timer to set the mode for (TIMER0, TIMER1, or TIMER2).
 * @param timerMode The mode to set for the timer (TIMER_NORMAL, TIMER_PWM_PHASE_CORRECT_8_BIT, TIMER_PWM_PHASE_CORRECT_9_BIT, TIMER_PWM_PHASE_CORRECT_10_BIT, TIMER_CTC_OCRN_TOP, TIMER_FAST_PWM_8_BIT, TIMER_FAST_PWM_9_BIT, TIMER_FAST_PWM_10_BIT, TIMER_PWM_PHASE_FREQUENCY_CORRECT_ICR1_TOP, TIMER_PWM_PHASE_FREQUENCY_CORRECT_OCR1A_TOP, TIMER_PWM_PHASE_CORRECT_ICR1_TOP, TIMER_PWM_PHASE_CORRECT_OCR1A_TOP, TIMER_CTC_ICR1_TOP, TIMER_FAST_PWM_ICR1_TOP, or TIMER_FAST_PWM_OCR1A_TOP).
 */
static void TIMER_setTimer_Mode(TIMERn_t timer, TIMER1_Modes_t timerMode)
{
	switch (timer)
	{
	case TIMER0:
		switch (timerMode)
		{
		case TIMER1_NORMAL:
			clear_bit(TCCR0, TCCR0_WGM00);
			clear_bit(TCCR0, TCCR0_WGM01);
			break;

		case TIMER1_PWM_PHASE_CORRECT_8_BIT:
			set_bit(TCCR0, TCCR0_WGM00);
			clear_bit(TCCR0, TCCR0_WGM01);
			break;

		case TIMER1_CTC_OCRN_TOP:
			clear_bit(TCCR0, TCCR0_WGM00);
			set_bit(TCCR0, TCCR0_WGM01);
			break;

		case TIMER1_FAST_PWM_8_BIT:
			set_bit(TCCR0, TCCR0_WGM00);
			set_bit(TCCR0, TCCR0_WGM01);
			break;
		default:
			//do nothing
			break;
		}
		break;
	case TIMER1:
		switch (timerMode)
		{

		case TIMER1_NORMAL:
			clear_bit(TCCR1A, TCCR1A_WGM10);
			clear_bit(TCCR1A, TCCR1A_WGM11);
			clear_bit(TCCR1B, TCCR1B_WGM12);
			clear_bit(TCCR1B, TCCR1B_WGM13);
			break;
		case TIMER1_PWM_PHASE_CORRECT_8_BIT:
			set_bit(TCCR1A, TCCR1A_WGM10);
			clear_bit(TCCR1A, TCCR1A_WGM11);
			clear_bit(TCCR1B, TCCR1B_WGM12);
			clear_bit(TCCR1B, TCCR1B_WGM13);
			break;
		case TIMER1_PWM_PHASE_CORRECT_9_BIT:
			clear_bit(TCCR1A, TCCR1A_WGM10);
			set_bit(TCCR1A, TCCR1A_WGM11);
			clear_bit(TCCR1B, TCCR1B_WGM12);
			clear_bit(TCCR1B, TCCR1B_WGM13);
			break;
		case TIMER1_PWM_PHASE_CORRECT_10_BIT:
			set_bit(TCCR1A, TCCR1A_WGM10);
			set_bit(TCCR1A, TCCR1A_WGM11);
			clear_bit(TCCR1B, TCCR1B_WGM12);
			clear_bit(TCCR1B, TCCR1B_WGM13);
			break;
		case TIMER1_CTC_OCRN_TOP:
			clear_bit(TCCR1A, TCCR1A_WGM10);
			clear_bit(TCCR1A, TCCR1A_WGM11);
			set_bit(TCCR1B, TCCR1B_WGM12);
			clear_bit(TCCR1B, TCCR1B_WGM13);
			break;
		case TIMER1_FAST_PWM_8_BIT:
			set_bit(TCCR1A, TCCR1A_WGM10);
			clear_bit(TCCR1A, TCCR1A_WGM11);
			set_bit(TCCR1B, TCCR1B_WGM12);
			clear_bit(TCCR1B, TCCR1B_WGM13);
			break;
		case TIMER1_FAST_PWM_9_BIT:
			clear_bit(TCCR1A, TCCR1A_WGM10);
			set_bit(TCCR1A, TCCR1A_WGM11);
			set_bit(TCCR1B, TCCR1B_WGM12);
			clear_bit(TCCR1B, TCCR1B_WGM13);
			break;
		case TIMER1_FAST_PWM_10_BIT:
			set_bit(TCCR1A, TCCR1A_WGM10);
			set_bit(TCCR1A, TCCR1A_WGM11);
			set_bit(TCCR1B, TCCR1B_WGM12);
			clear_bit(TCCR1B, TCCR1B_WGM13);
			break;
		case TIMER1_PWM_PHASE_FREQUENCY_CORRECT_ICR1_TOP:
			clear_bit(TCCR1A, TCCR1A_WGM10);
			clear_bit(TCCR1A, TCCR1A_WGM11);
			clear_bit(TCCR1B, TCCR1B_WGM12);
			set_bit(TCCR1B, TCCR1B_WGM13);
			break;
		case TIMER1_PWM_PHASE_FREQUENCY_CORRECT_OCR1A_TOP:
			set_bit(TCCR1A, TCCR1A_WGM10);
			clear_bit(TCCR1A, TCCR1A_WGM11);
			clear_bit(TCCR1B, TCCR1B_WGM12);
			set_bit(TCCR1B, TCCR1B_WGM13);
			break;
		case TIMER1_PWM_PHASE_CORRECT_ICR1_TOP:
			clear_bit(TCCR1A, TCCR1A_WGM10);
			set_bit(TCCR1A, TCCR1A_WGM11);
			clear_bit(TCCR1B, TCCR1B_WGM12);
			set_bit(TCCR1B, TCCR1B_WGM13);
			break;
		case TIMER1_PWM_PHASE_CORRECT_OCR1A_TOP:
			set_bit(TCCR1A, TCCR1A_WGM10);
			set_bit(TCCR1A, TCCR1A_WGM11);
			clear_bit(TCCR1B, TCCR1B_WGM12);
			set_bit(TCCR1B, TCCR1B_WGM13);
			break;
		case TIMER1_CTC_ICR1_TOP:
			clear_bit(TCCR1A, TCCR1A_WGM10);
			clear_bit(TCCR1A, TCCR1A_WGM11);
			set_bit(TCCR1B, TCCR1B_WGM12);
			set_bit(TCCR1B, TCCR1B_WGM13);
			break;
		case TIMER1_FAST_PWM_ICR1_TOP:
			clear_bit(TCCR1A, TCCR1A_WGM10);
			set_bit(TCCR1A, TCCR1A_WGM11);
			set_bit(TCCR1B, TCCR1B_WGM12);
			set_bit(TCCR1B, TCCR1B_WGM13);
			break;
		case TIMER1_FAST_PWM_OCR1A_TOP:
			set_bit(TCCR1A, TCCR1A_WGM10);
			set_bit(TCCR1A, TCCR1A_WGM11);
			set_bit(TCCR1B, TCCR1B_WGM12);
			set_bit(TCCR1B, TCCR1B_WGM13);
			break;
		default:
		//do nothing
			break;
		}
		break;
		
	case TIMER2:
		switch (timerMode)
		{
		case TIMER1_NORMAL:
			clear_bit(TCCR2, TCCR2_WGM20);
			clear_bit(TCCR2, TCCR2_WGM21);
			break;

		case TIMER1_PWM_PHASE_CORRECT_8_BIT:
			set_bit(TCCR2, TCCR2_WGM20);
			clear_bit(TCCR2, TCCR2_WGM21);
			break;

		case TIMER1_CTC_OCRN_TOP:
			clear_bit(TCCR2, TCCR2_WGM20);
			set_bit(TCCR2, TCCR2_WGM21);
			break;

		case TIMER1_FAST_PWM_8_BIT:
			set_bit(TCCR2, TCCR2_WGM20);
			set_bit(TCCR2, TCCR2_WGM21);
			break;
		default:
		//do nothing
			break;
		}
		break;
		default:
			//do nothing
		break;
	}
}

/**
 * @brief Sets the compare mode for the specified compare unit.
 *
 * This function sets the compare mode for the specified compare unit. The compare mode determines
 * the behavior of the compare unit.
 *
 * @param OCx The compare unit to set the mode for (OC0, OC1A, OC1B, or OC2).
 * @param compareMode The mode to set for the compare unit (TIMER_OC_MODE_NO_OC0, TIMER_OC_MODE_TOGGLE_OC0, TIMER_OC_MODE_CLEAR_OC0, or TIMER_OC_MODE_SET_OC0).
 */
static void TIMER_setOc_Mode(TIMER_OCx_t OCx, TIMER_OC_MODE_t compareMode)
{
	u8 COMx0 = 0, COMx1 = 0;
	volatile u8 *arrOf_TCCRx[] = {&TCCR0, &TCCR1A, &TCCR2};
	volatile u8 *TCCRx;
	switch (OCx)
	{
	case OC0:
		COMx0 = TCCR0_COM00;
		COMx1 = TCCR0_COM01;
		TCCRx = arrOf_TCCRx[0];
		break;
	case OC1A:
		COMx0 = TCCR1A_COM1A0;
		COMx1 = TCCR1A_COM1A1;
		TCCRx = arrOf_TCCRx[1];
		break;
	case OC1B:
		COMx0 = TCCR1A_COM1B0;
		COMx1 = TCCR1A_COM1B1;
		TCCRx = arrOf_TCCRx[1];
		break;
	case OC2:
		COMx0 = TCCR2_COM20;
		COMx1 = TCCR2_COM21;
		TCCRx = arrOf_TCCRx[2];
		break;
	}
	switch (compareMode)
	{
	case TIMER_OCx_MODE_DICONNECTED:
		clear_bit((*TCCRx), COMx0);
		clear_bit((*TCCRx), COMx1);
		break;
	case TIMER_OCx_MODE_TOGGLE:
		set_bit((*TCCRx), COMx0);
		clear_bit((*TCCRx), COMx1);
		break;
	case TIMER_OCx_MODE_CLEAR_NON_INVERTING:
		clear_bit((*TCCRx), COMx0);
		set_bit((*TCCRx), COMx1);
		break;
	case TIMER_OCx_MODE_SET_INVERTING:
		set_bit((*TCCRx), COMx0);
		set_bit((*TCCRx), COMx1);
		break;
	}
}

/**
 * @brief Sets the clock select for the specified timer.
 *
 * This function sets the clock select for the specified timer. The clock select determines
 * the clock source for the timer.
 *
 * @param timer The timer to set the clock select for (TIMER0, TIMER1, or TIMER2).
 * @param CLk_select The clock select to set for the timer (TIMER_Pre_CLK_0, TIMER_Pre_CLK_1, TIMER_Pre_CLK_8, TIMER_Pre_CLK_64, TIMER_Pre_CLK_256, TIMER_Pre_CLK_1024, TIMER_Pre_ExtT0_F, or TIMER_Pre_ExtT0_R).
 */
static void TIMER_setClk(TIMERn_t timer, TIMERS_Pre_CLK_t CLk_select)
{
	volatile u8 *arrOf_TCCRx[] = {&TCCR0, &TCCR1B, &TCCR2};
	volatile u8 *TCCRx;
	switch (timer)
	{
	case TIMER0:
		TCCRx = arrOf_TCCRx[0];
		break;
	case TIMER1:
		TCCRx = arrOf_TCCRx[1];
		break;
	case TIMER2:
		TCCRx = arrOf_TCCRx[2];
		break;
	}
	write_masked_value(*TCCRx, 0b00000111, CLk_select);
}



/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                         Interrupt Service Routines                          */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
ISR(TIMER0_OVF_VECT)
{
	if (TIMER0_OVF_Fptr != NULL_PTR)
	{
		TIMER0_OVF_Fptr();
	}
}
ISR(TIMER0_COMP_VECT)
{
	if (TIMER0_COMP_Fptr != NULL_PTR)
	{
		TIMER0_COMP_Fptr();
	}
}
ISR(TIMER1_OVF_VECT)
{
	if (TIMER1_OVF_Fptr != NULL_PTR)
	{
		TIMER1_OVF_Fptr();
	}
}
ISR(TIMER1_COMPA_VECT)
{
	if (TIMER1_COMPA_Fptr != NULL_PTR)
	{
		TIMER1_COMPA_Fptr();
	}
}
ISR(TIMER1_COMPB_VECT)
{
	if (TIMER1_COMPB_Fptr != NULL_PTR)
	{
		TIMER1_COMPB_Fptr();
	}
}
ISR(TIMER1_CAPT_VECT)
{
	if (TIMER1_CAPT_Fptr != NULL_PTR)
	{
		TIMER1_CAPT_Fptr();
	}
}
ISR(TIMER2_OVF_VECT)
{
	if (TIMER2_OVF_Fptr != NULL_PTR)
	{
		TIMER2_OVF_Fptr();
	}
}
ISR(TIMER2_COMP_VECT)
{
	if (TIMER2_COMP_Fptr != NULL_PTR)
	{
		TIMER2_COMP_Fptr();
	}
}

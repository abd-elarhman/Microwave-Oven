/**
 * @file TIMERS_Interfacing.h
 * @author Abdelrahman Ahmed Moussa (abdelrahman.ahmed0599@gmail.com  , https://www.linkedin.com/in/-abdelrahman-ahmed)
 * @brief  This file is a header file that contains the public functions prototypes and the macros used for the Timers driver.
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef TIMERS_INTERFACING_H_
#define TIMERS_INTERFACING_H_

#include "MemMap.h"
/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                          Macro like FUNCTIONS                                */
/*                                                                              */
/*------------------------------------------------------------------------------*/
#define    TIMER0_SetCounterValue(value)       (TCNT0=((u8)value))
#define    TIMER0_GetCounterValue()            ((u8)TCNT0)
#define    TIMER0_SetCompareValue(value)       (OCR0=((u8)(value)))
#define    TIMER0_GetCompareValue()            ((u8)OCR0)

#define    TIMER1_SetCounterValue(value)       (TCNT1=( (u16)(value) ) )
#define    TIMER1_GetCounterValue()            ((u16)TCNT1)

#define    TIMER1_SetCompare_A_Value(value)    (OCR1A=((u16)(value)))
#define    TIMER1_GetCompare_A_Value()         ((u16)OCR1A)
#define    TIMER1_SetCompare_B_Value(value)    (OCR1B=((u16)(value)))
#define    TIMER1_GetCompare_B_Value()         ((u16)OCR1B)
#define	   TIMER1_SetICRValue(value)           (ICR1=((u16)(value)))
#define    TIMER1_GetICRValue()                ((u16)ICR1)

#define    TIMER2_SetCounterValue(value)       (TCNT2=((u8)value))
#define    TIMER2_GetCounterValue()            ((u8)TCNT2)
#define    TIMER2_SetCompareValue(value)       (OCR2=((u8)(value)))
#define    TIMER2_GetCompareValue()            ((u8)OCR2)

/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                                  TYPEDEFS                                    */
/*                                                                              */
/*------------------------------------------------------------------------------*/
typedef enum {
	TIMER0_NORMAL=0,
	TIMER0_PWM_PHASE_CORRECT_8_BIT=1,
	TIMER0_CTC_OCRN_TOP=4,
	TIMER0_FAST_PWM_8_BIT=5
}TIMER0_Modes_t;

typedef enum {
	TIMER1_NORMAL,
	TIMER1_PWM_PHASE_CORRECT_8_BIT,
	TIMER1_PWM_PHASE_CORRECT_9_BIT,
	TIMER1_PWM_PHASE_CORRECT_10_BIT,
	TIMER1_CTC_OCRN_TOP,
	TIMER1_FAST_PWM_8_BIT,
	TIMER1_FAST_PWM_9_BIT,
	TIMER1_FAST_PWM_10_BIT,
	TIMER1_PWM_PHASE_FREQUENCY_CORRECT_ICR1_TOP,
	TIMER1_PWM_PHASE_FREQUENCY_CORRECT_OCR1A_TOP,
	TIMER1_PWM_PHASE_CORRECT_ICR1_TOP,
	TIMER1_PWM_PHASE_CORRECT_OCR1A_TOP,
	TIMER1_CTC_ICR1_TOP,
	TIMER1_FAST_PWM_ICR1_TOP,
	TIMER1_FAST_PWM_OCR1A_TOP
}TIMER1_Modes_t;

typedef enum {
	TIMER2_NORMAL=0,
	TIMER2_PWM_PHASE_CORRECT_8_BIT=1,
	TIMER2_CTC_OCRN_TOP=4,
	TIMER2_FAST_PWM_8_BIT=5
}TIMER2_Modes_t;

typedef enum {
	// CS02 CS01 CS00 Description
	TIMER_Pre_CLK_0   =0,     // 0    0    0    No clock source (Timer/Counter stopped).
	TIMER_Pre_CLK_1     ,     // 0    0    1    clk/1    (No prescaling)
	TIMER_Pre_CLK_8     ,     // 0    1    0    clk/8    (From prescaler)
	TIMER_Pre_CLK_64    ,     // 0    1    1    clk/64   (From prescaler)
	TIMER_Pre_CLK_256   ,     // 1    0    0    clk/256  (From prescaler)
	TIMER_Pre_CLK_1024  ,     // 1    0    1    clk/1024 (From prescaler)
	TIMER_Pre_ExtT0_F   ,     // 1    1    0    External clock source on T0 pin. Clock on falling edge.
	TIMER_Pre_ExtT0_R =7      // 1    1    1    External clock source on T0 pin. Clock on rising edge.
}TIMERS_Pre_CLK_t;

/* ====== Compare Output Mode, non-PWM Mode ====== */
// COM01 COM00 Description
// 0     0     Normal port operation, OC0 disconnected.
// 0     1     Toggle OC0 on compare match
// 1     0     Clear OC0 on compare match
// 1     1     Set OC0 on compare match

/* ====== Compare Output Mode, Fast PWM Mode ====== */
// COM01 COM00 Description
// 0     0     Normal port operation, OC0 disconnected.
// 0     1     Reserved
// 1     0     Clear OC0 on compare match, set OC0 at TOP
// 1     1     Set OC0 on compare match, clear OC0 at TOP

/* ====== Compare Output Mode, Phase Correct PWM Mode ====== */
// COM01 COM00 Description
// 0     0     Normal port operation, OC0 disconnected.
// 0     1     Reserved
// 1     0     Clear OC0 on compare match when up-counting. Set OC0 on compare match when downcounting.
// 1     1     Set OC0 on compare match when up-counting. Clear OC0 on compare match when downcounting.

typedef enum {
	TIMER_OCx_MODE_DICONNECTED,           // DIO
	TIMER_OCx_MODE_TOGGLE,               //  Toggle at Compare Match
	TIMER_OCx_MODE_CLEAR_NON_INVERTING,  //  Clear at Compare Match
	TIMER_OCx_MODE_SET_INVERTING         //  Set   at Compare Match
}TIMER_OC_MODE_t;

typedef enum {
	TIMER0,
	TIMER1,
	TIMER2
}TIMERn_t;

typedef enum {
	OC0,
	OC1A,
	OC1B,
	OC2
}TIMER_OCx_t;


typedef enum{
	RISING,
	FALLING
}ICU_Edge_type;






/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                             API FUNCTIONS                                    */
/*                                                                              */
/*------------------------------------------------------------------------------*/
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
void TIMER0_Init(TIMER0_Modes_t timerMode, TIMER_OC_MODE_t compareMode, TIMERS_Pre_CLK_t CLk_select);

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
void TIMER1_Init(TIMER1_Modes_t timerMode, TIMER_OC_MODE_t OC1A_compareMode, TIMER_OC_MODE_t OC1B_compareMode, TIMERS_Pre_CLK_t CLk_select);

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
void TIMER2_Init(TIMER2_Modes_t timerMode, TIMER_OC_MODE_t compareMode, TIMERS_Pre_CLK_t CLk_select);

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
void TIMER_ONOFF(TIMERn_t timer, Std_OnOff_t status);


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
void TIMER0_OVF_INT(Std_EnableDisable_t status);

/**
 * @brief Enables or disables the Timer/Counter0 Compare Match interrupt.
 *
 * This function is used to enable or disable the Timer/Counter0 Compare Match interrupt.
 *
 * @param status The status of the interrupt. Use `ENABLE` to enable the interrupt or `DISABLE` to disable it.
 */
void TIMER0_COMP_INT(Std_EnableDisable_t status);

/**
 * @brief Enables or disables the Timer1 overflow interrupt.
 *
 * This function allows you to enable or disable the Timer1 overflow interrupt.
 *
 * @param status The status of the interrupt. Use `ENABLE` to enable the interrupt or `DISABLE` to disable it.
 */
void TIMER1_OVF_INT(Std_EnableDisable_t status);

/**
 * @brief Enables or disables the Timer1 Compare Match A interrupt.
 *
 * This function allows you to enable or disable the Timer1 Compare Match A interrupt.
 *
 * @param status The status of the interrupt. Use `ENABLE` to enable the interrupt or `DISABLE` to disable it.
 */
void TIMER1_COMPA_INT(Std_EnableDisable_t status);

/**
 * @brief Enables or disables the Timer1 Compare Match B interrupt.
 *
 * This function allows you to enable or disable the Timer1 Compare Match B interrupt.
 *
 * @param status The status of the interrupt. Use `ENABLE` to enable the interrupt or `DISABLE` to disable it.
 */
void TIMER1_COMPB_INT(Std_EnableDisable_t status);

/**
 * @brief Enables or disables the Timer1 Input Capture interrupt.
 *
 * This function allows you to enable or disable the Timer1 Input Capture interrupt.
 *
 * @param status The status of the interrupt. Use `ENABLE` to enable the interrupt or `DISABLE` to disable it.
 */
void TIMER1_CAPT_INT(Std_EnableDisable_t status);

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
void TIMER2_OVF_INT(Std_EnableDisable_t status);

/**
 * @brief Enables or disables the Timer2 Compare Match interrupt.
 *
 * This function allows you to enable or disable the Timer2 Compare Match interrupt.
 *
 * @param status The status of the interrupt. Use `ENABLE` to enable the interrupt or `DISABLE` to disable it.
 */
void TIMER2_COMP_INT(Std_EnableDisable_t status);

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
void TIMER0_OVF_SetCallBack(void (*LocalPtr)(void));

/**
 * @brief Sets the callback function for TIMER0 Compare Match interrupt.
 * 
 * This function allows the user to set a callback function that will be executed when TIMER0 Compare Match occurs.
 * 
 * @param LocalPtr Pointer to the callback function.
 */
void TIMER0_COMP_SetCallBack(void (*LocalPtr)(void));

/**
 * @brief Sets the callback function for TIMER1 overflow interrupt.
 * 
 * This function allows the user to set a callback function that will be executed when TIMER1 overflows.
 * 
 * @param LocalPtr Pointer to the callback function.
 */
void TIMER1_OVF_SetCallBack(void (*LocalPtr)(void));

/**
 * @brief Sets the callback function for TIMER1 Compare Match A interrupt.
 * 
 * This function allows the user to set a callback function that will be executed when TIMER1 Compare Match A occurs.
 * 
 * @param LocalPtr Pointer to the callback function.
 */
void TIMER1_COMPA_SetCallBack(void (*LocalPtr)(void));

/**
 * @brief Sets the callback function for TIMER1 Compare Match B interrupt.
 * 
 * This function allows the user to set a callback function that will be executed when TIMER1 Compare Match B occurs.
 * 
 * @param LocalPtr Pointer to the callback function.
 */
void TIMER1_COMPB_SetCallBack(void (*LocalPtr)(void));

/**
 * @brief Sets the callback function for TIMER1 Input Capture interrupt.
 * 
 * This function allows the user to set a callback function that will be executed when TIMER1 Input Capture occurs.
 * 
 * @param LocalPtr Pointer to the callback function.
 */
void TIMER1_CAPT_SetCallBack(void (*LocalPtr)(void));

/**
 * @brief Sets the callback function for TIMER2 overflow interrupt.
 * 
 * This function allows the user to set a callback function that will be executed when TIMER2 overflows.
 * 
 * @param LocalPtr Pointer to the callback function.
 */
void TIMER2_OVF_SetCallBack(void (*LocalPtr)(void));

/**
 * @brief Sets the callback function for TIMER2 Compare Match interrupt.
 * 
 * This function allows the user to set a callback function that will be executed when TIMER2 Compare Match occurs.
 * 
 * @param LocalPtr Pointer to the callback function.
 */
void TIMER2_COMP_SetCallBack(void (*LocalPtr)(void));


#endif /* TIMERS_INTERFACING_H_ */
/**
 * @file TIMERS_Private.h
 * @author Abdelrahman Ahmed Moussa (abdelrahman.ahmed0599@gmail.com  , https://www.linkedin.com/in/-abdelrahman-ahmed)
 * @brief  This file is a header file that contains the private functions prototypes and the static global variables for the Timers driver.
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef TIMERS_PRIVATE_H_
#define TIMERS_PRIVATE_H_

/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                          Static Global Vaiables                              */
/*                                                                              */
/*------------------------------------------------------------------------------*/
static TIMERS_Pre_CLK_t Timer0_Clk=TIMER_Pre_CLK_8;
static TIMERS_Pre_CLK_t Timer1_Clk=TIMER_Pre_CLK_8;
static TIMERS_Pre_CLK_t Timer2_Clk=TIMER_Pre_CLK_8;

/*-------------*/
/*   Timer0    */
/*-------------*/
static void (*TIMER0_OVF_Fptr)(void)=NULL_PTR;
static void (*TIMER0_COMP_Fptr)(void)=NULL_PTR;

/*-------------*/
/*   Timer1    */
/*-------------*/
static void (*TIMER1_OVF_Fptr)(void)=NULL_PTR;
static void (*TIMER1_COMPA_Fptr)(void)=NULL_PTR;
static void (*TIMER1_COMPB_Fptr)(void)=NULL_PTR;
static void (*TIMER1_CAPT_Fptr)(void)=NULL_PTR;

/*-------------*/
/*   Timer2    */
/*-------------*/
static void (*TIMER2_OVF_Fptr)(void)=NULL_PTR;
static void (*TIMER2_COMP_Fptr)(void)=NULL_PTR;

/*------------------------------------------------------------------------------*/
/*                                                                              */
/*                          Static Functions Prototypes                         */
/*                                                                              */
/*------------------------------------------------------------------------------*/

/**
 * @brief Sets the timer mode for the specified timer.
 *
 * This function sets the timer mode for the specified timer. The timer mode determines
 * the behavior and operation of the timer.
 *
 * @param timer The timer to set the mode for (TIMER0, TIMER1, or TIMER2).
 * @param timerMode The mode to set for the timer (TIMER_NORMAL, TIMER_PWM_PHASE_CORRECT_8_BIT, TIMER_PWM_PHASE_CORRECT_9_BIT, TIMER_PWM_PHASE_CORRECT_10_BIT, TIMER_CTC_OCRN_TOP, TIMER_FAST_PWM_8_BIT, TIMER_FAST_PWM_9_BIT, TIMER_FAST_PWM_10_BIT, TIMER_PWM_PHASE_FREQUENCY_CORRECT_ICR1_TOP, TIMER_PWM_PHASE_FREQUENCY_CORRECT_OCR1A_TOP, TIMER_PWM_PHASE_CORRECT_ICR1_TOP, TIMER_PWM_PHASE_CORRECT_OCR1A_TOP, TIMER_CTC_ICR1_TOP, TIMER_FAST_PWM_ICR1_TOP, or TIMER_FAST_PWM_OCR1A_TOP).
 */
static void TIMER_setTimer_Mode(TIMERn_t timer, TIMER1_Modes_t timerMode);

/**
 * @brief Sets the compare mode for the specified compare unit.
 *
 * This function sets the compare mode for the specified compare unit. The compare mode determines
 * the behavior of the compare unit.
 *
 * @param OCx The compare unit to set the mode for (OC0, OC1A, OC1B, or OC2).
 * @param compareMode The mode to set for the compare unit (TIMER_OC_MODE_NO_OC0, TIMER_OC_MODE_TOGGLE_OC0, TIMER_OC_MODE_CLEAR_OC0, or TIMER_OC_MODE_SET_OC0).
 */
static void TIMER_setOc_Mode(TIMER_OCx_t OCx, TIMER_OC_MODE_t compareMode);

/**
 * @brief Sets the clock select for the specified timer.
 *
 * This function sets the clock select for the specified timer. The clock select determines
 * the clock source for the timer.
 *
 * @param timer The timer to set the clock select for (TIMER0, TIMER1, or TIMER2).
 * @param CLk_select The clock select to set for the timer (TIMER_Pre_CLK_0, TIMER_Pre_CLK_1, TIMER_Pre_CLK_8, TIMER_Pre_CLK_64, TIMER_Pre_CLK_256, TIMER_Pre_CLK_1024, TIMER_Pre_ExtT0_F, or TIMER_Pre_ExtT0_R).
 */
static void TIMER_setClk(TIMERn_t timer, TIMERS_Pre_CLK_t CLk_select);

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*                                  Enums                                      */
/*                                                                             */
/*-----------------------------------------------------------------------------*/

// Timer/Counter Control Register TCCR0
enum {
	TCCR0_CS00 =0,  // Clock Select
	TCCR0_CS01   ,  // Clock Select
	TCCR0_CS02   ,  // Clock Select
	TCCR0_WGM01  ,  // Waveform Generation Mode
	TCCR0_COM00  ,  // Compare Match Output Mode
	TCCR0_COM01  ,  // Compare Match Output Mode
	TCCR0_WGM00  ,  // Waveform Generation Mode
	TCCR0_FOC0 =7   // Force Output Compare
	};
	
//Timer/Counter1 Control Register A TCCR1A
enum {
	TCCR1A_WGM10,   // Waveform Generation Mode
	TCCR1A_WGM11,	// Waveform Generation Mode
	TCCR1A_FOC1B,	// Force Output Compare for Channel B
	TCCR1A_FOC1A,	// Force Output Compare for Channel A
	TCCR1A_COM1B0,	// Compare Output Mode for Channel B
	TCCR1A_COM1B1,	// Compare Output Mode for Channel B
	TCCR1A_COM1A0,	// Compare Output Mode for Channel A
	TCCR1A_COM1A1	// Compare Output Mode for Channel A
	};

//Timer/Counter1 Control Register B TCCR1B
enum {
	 TCCR1B_CS10=0,   // Clock Select
	 TCCR1B_CS11,	   // Clock Select
	 TCCR1B_CS12,	   // Clock Select
	 TCCR1B_WGM12,	   // Waveform Generation Mode
	 TCCR1B_WGM13,	   // Waveform Generation Mode
	 TCCR1B_ICES1=6,  // Input Capture Edge Select
	 TCCR1B_ICNC1     // Input Capture Noise Canceler
	 };

//Timer/Counter Control Register TCCR2
enum {
	TCCR2_CS20,   // Clock Select
	TCCR2_CS21,	  // Clock Select
	TCCR2_CS22,	  // Clock Select
	TCCR2_WGM21,  // Waveform Generation Mode
	TCCR2_COM20,  // Compare Output Mode
	TCCR2_COM21,  // Compare Output Mode
	TCCR2_WGM20,  // Waveform Generation Mode
	TCCR2_FOC2	  // Force Output Compare
     };

//Asynchronous Status_Register ASSR
enum {
	ASSR_TCR2UB,  // Timer/Counter Control Register2 Update Busy
	ASSR_OCR2UB,  // Output Compare Register2 Update Busy
	ASSR_TCN2UB,  // Timer/Counter2 Update Busy
	ASSR_AS2 	  // Asynchronous Timer/Counter2
	};

// Timer/Counter Interrupt Mask Register TIMSK
enum {
	TIMSK_TOIE0 =0,    // Timer/Counter0 Overflow Interrupt Enable
	TIMSK_OCIE0	  ,    // Timer/Counter0 Output Compare Match Interrupt Enable
	TIMSK_TOIE1	  ,    //
	TIMSK_OCIE1B  ,    //
	TIMSK_OCIE1A  ,    //
	TIMSK_TICIE1  ,    //
	TIMSK_TOIE2	  ,    //
	TIMSK_OCIE2 =7     //
	};

// Timer/Counter Interrupt Flag Register TIFR
enum {
	TIFR_TOV0  =0,   // Timer/Counter0 Overflow Flag
	TIFR_OCF0	 ,   // Output Compare Flag 0
	TIFR_TOV1	 ,   //
	TIFR_OCF1B   ,   //
	TIFR_OCF1A   ,   //
	TIFR_ICF1    ,   //
	TIFR_TOV2	 ,   //
	TIFR_OCF2  =7    //
	};


#endif /* TIMERS_PRIVATE_H_ */
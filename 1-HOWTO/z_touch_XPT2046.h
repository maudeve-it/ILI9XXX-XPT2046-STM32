/*
 * 	z_touch_XPT2046.h
 *	rel. TouchGFX.1.30
 *
 *  Created on: 05 giu 2023
 *      Author: mauro
 *
 *  licensing: https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/blob/c097f0e7d569845c1cf98e8d930f2224e427fd54/LICENSE
 *
 *  Installing and using this library follow instruction on: https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32
 *
 *  WARNING:
 *	in main.h put the #insert of this file BELOW the #insert of z_displ_ILIxxxx.h
 *
 *  If using TouchGFX,
 *  you have also to add the below include:
#include "main.h"
 *  into STM32TouchController.cpp file
 *  changing also sampleTouch()
 *  as shown here:
bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
	return ((bool) Touch_TouchGFXSampleTouch(&x, &y));
}
 *
 *  see also z_displ_ili9XXX.h
 *
 */
#ifndef __XPT2046_H
#define __XPT2046_H


/*||||||||||| USER/PROJECT PARAMETERS |||||||||||*/

/*****************     STEP 1      *****************
 **************** PORT PARAMETERS *****************
 ** properly set the below the 2 defines to address
 ********  the SPI port defined on CubeMX *********/

#define TOUCH_SPI_PORT 	hspi2
#define TOUCH_SPI 		SPI2


/*****************     STEP 2      *****************
 **********   KEY REPEAT FOR TOUCHGFX   ***********
 * used only in TouchGFX integration
 * - set a value above 0 defining the timeout (ms)
 *   before starting key repeat, reading touch sensor
 * - set 0 disabling key repeat (single pulse)
 * - set -1 for a continuous touch needed by
 *   "dragging" widgets
 * (see GitHub page indicated on top for details)
 **************************************************/
#define DELAY_TO_KEY_REPEAT -1

/*|||||||| END OF USER/PROJECT PARAMETERS ||||||||*/




/*|||||||||||||| DEVICE PARAMETERS |||||||||||||||||*/
/* you should need to change nothing from here on */

/**************************************************
 * this is the command to send to XPT2046 asking to
 * poll axis and return corresponging value.
 ****************************************	**********/
#define X_AXIS		0xD0
#define Y_AXIS		0x90
#define Z_AXIS		0xB0


/**********************************************************************************
 *	polling XPT2046 axis, the returning value exceeding the below limit
 *	indicates there is no touch. WARNING: a "random within limit" value is returned
 *	sometimes (often) even if there is no touch, so at least two consecutive r
 *	eadings must be performed to confirm a touch
 **********************************************************************************/
#ifdef ILI9341
#define X_THRESHOLD		0x0200	//below threeshold there is no touch
#define Z_THRESHOLD		0x0200	//below threeshold there is no touch
#endif
#ifdef ILI9488
#define X_THRESHOLD		0x0500	//below threeshold there is no touch
#define Z_THRESHOLD		0x0500	//below threeshold there is no touch
#endif


/**********************************************************************************
 ***************************** CALIBRATION PARAMETERS *****************************
 **********************************************************************************
 * parameters for the linear conversion from a touch sensor reading, to
 * the XY display position
 * using the formula:
 * Xdispl = AX * Xtouch + BX
 * Ydispl = AY * Ytouch + BY
 *
 **********************************************************************************/

#ifdef ILI9341
#define T_ROTATION_0
#define AX 0.00801f
#define BX -11.998f
#define AY 0.01119f
#define BY -39.057f
#endif

#ifdef ILI9488_V1
#define T_ROTATION_270
#define AX 0.016f
#define BX -20.0f
#define AY 0.011f
#define BY -15.0f
#endif

#ifdef ILI9488_V2
#define T_ROTATION_0
#define AX -0.0112f
#define BX 336.0f
#define AY 0.0166f
#define BY -41.38f
#endif




/**********************************************************************************
 * parameters screen/touch orientation: set the touch orientation to the corresponding
 * screen orientation:
 * on ILI9341 0° on touch correspond to 0° of the screen
 * on ILI9341 0° on touch correspond to 270° of the screen
 * set also the size of a 0° row and a 90° row (a 0° height)
 **********************************************************************************/
#ifdef T_ROTATION_0
#define TOUCH0 			Displ_Orientat_0
#define TOUCH90 		Displ_Orientat_90
#define TOUCH180 		Displ_Orientat_180
#define TOUCH270 		Displ_Orientat_270
#define TOUCH_0_WIDTH 	DISPL_WIDTH
#define TOUCH_0_HEIGHT	DISPL_HEIGHT
#endif

#ifdef T_ROTATION_90
#define TOUCH0 			Displ_Orientat_90
#define TOUCH90 		Displ_Orientat_180
#define TOUCH180 		Displ_Orientat_270
#define TOUCH270 		Displ_Orientat_0
#define TOUCH_0_WIDTH 	DISPL_HEIGHT
#define TOUCH_0_HEIGHT	DISPL_WIDTH
#endif

#ifdef T_ROTATION_180
#define TOUCH0 			Displ_Orientat_180
#define TOUCH90 		Displ_Orientat_270
#define TOUCH180 		Displ_Orientat_0
#define TOUCH270 		Displ_Orientat_90
#define TOUCH_0_WIDTH 	DISPL_WIDTH
#define TOUCH_0_HEIGHT	DISPL_HEIGHT
#endif

#ifdef T_ROTATION_270
#define TOUCH0 			Displ_Orientat_270
#define TOUCH90 		Displ_Orientat_0
#define TOUCH180 		Displ_Orientat_90
#define TOUCH270 		Displ_Orientat_180
#define TOUCH_0_WIDTH 	DISPL_HEIGHT
#define TOUCH_0_HEIGHT	DISPL_WIDTH
#endif



/*|||||||||||||| INTERFACE PARAMETERS |||||||||||||||||*/

/**********************************************************************************
 * next parameters are used only in TouchGFX: in Touch_TouchGFXSampleTouch() and
 * Touch_GotATouch(), helping in using dragging widgets like scrolling lists
 * You can try to change the below parameters only if your display looses quality or over-used
 **********************************************************************************/
#define TOUCHGFX_TIMING 60		//delay between 2 consecutive Touch_GotATouch(2)readings		(0=disabled)
#define TOUCHGFX_SENSITIVITY 1  //square of X pixels size having the same value					(1 disabled)
#define TOUCHGFX_MOVAVG 1		//makes position based on average of the last X readings		(1 disabled)
#define TOUCHGFX_REPEAT_IT 0	// after a long touch (dragging) repeat X times last position	(0=disabled)
#if DELAY_TO_KEY_REPEAT==-1
#define TOUCHGFX_REPEAT_NO 0	// after a REPEAT_IT repeat X times a no touch					(0=disabled)
#else
#define TOUCHGFX_REPEAT_NO 5	// after a REPEAT_IT repeat X times a no touch					(0=disabled)
#endif

/*||||||||||| END OF INTERFACE PARAMETERS ||||||||||||*/


/*|||||||||||||| FUNCTION DECLARATIONS |||||||||||||||||*/

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

uint8_t Touch_In_XY_area(uint16_t xpos,uint16_t ypos,uint16_t width,uint16_t height);
uint8_t Touch_GotATouch(uint8_t reset);
uint8_t Touch_WaitForUntouch(uint16_t delay);
uint8_t Touch_WaitForTouch(uint16_t delay);
uint8_t Touch_PollTouch();
void Touch_GetXYtouch(uint16_t *x, uint16_t *y, uint8_t *isTouch);

#ifdef DISPLAY_USING_TOUCHGFX
uint8_t Touch_TouchGFXSampleTouch(int32_t *x, int32_t *y);
#endif /* DISPLAY_USING_TOUCHGFX */

#endif /* __XPT2046_H */


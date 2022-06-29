/*
 *
 *  Created on: 2 giu 2022
 *      Author: mauro
 *
 *  licensing: https://github.com/maudeve-it/ILI9486-STM32/blob/main/LICENSE
 *
 *	WARNING WARNING WARNING:
 *	in main.h put the #insert of this file BELOW the #insert of ILIxxxx.h
 *
 */
#ifndef __XPT2046_H
#define __XPT2046_H


/***** USER/PROJECT PARAMETERS *****/
/**************** PORT PARAMETERS *****************
 ** properly set the below the 2 defines to address
 ********  the SPI port defined on CubeMX *********/

#define TOUCH_SPI_PORT 	hspi1
#define TOUCH_SPI 		SPI1


/**********************************************************************************
 * define here interrupt assigned to pendown display signaling.
 * this interrupt will be disabled during SPI communication with touch device
 **********************************************************************************/
#define PENDOWN_IRQ	EXTI15_10_IRQn

/***** END OF "USER/PROJECT PARAMETERS" *****/


/***** DEVICE PARAMETERS *****/
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
 * parameters for the linear conversion from a touch sensor reading, to
 * the XY display position
 * use the formula:
 * Xpos = AX * Xtouch + BX
 * Ypos = AY * Ytouch + BY
 *
 **********************************************************************************/
#ifdef ILI9341
#define AX 0.00801f
#define BX -11.998f
#define AY 0.01119f
#define BY -39.057f
#endif
#ifdef ILI9488
#define AX 0.0221f
#define BX -27.2f
#define AY 0.0163f
#define BY -31.3f
#endif


/**********************************************************************************
 * parameters screen/touch orientation: set the touch orientation to the corresponding
 * screen orientation:
 * on ILI9341 0° on touch correspond to 0° of the screen
 * on ILI9341 0° on touch correspond to 270° of the screen
 * set also the size of a 0° row and a 90° row (a 0° height)
 **********************************************************************************/
#ifdef ILI9341
#define TOUCH0 			Displ_Orientat_0
#define TOUCH90 		Displ_Orientat_90
#define TOUCH180 		Displ_Orientat_180
#define TOUCH270 		Displ_Orientat_270
#define TOUCH_0_WIDTH 	DISPL_WIDTH
#define TOUCH_0_HEIGHT	DISPL_HEIGHT
#endif

#ifdef ILI9488
#define TOUCH0 			Displ_Orientat_270
#define TOUCH90 		Displ_Orientat_0
#define TOUCH180 		Displ_Orientat_90
#define TOUCH270 		Displ_Orientat_180
#define TOUCH_0_WIDTH 	DISPL_HEIGHT
#define TOUCH_0_HEIGHT	DISPL_WIDTH
#endif


/***** END OF "DEVICE PARAMETERS" *****/



/**********************************************************************************
 * program declarations and definitions
 **********************************************************************************/


typedef struct {
	uint8_t	isTouch;
	uint16_t Xpos;
	uint16_t Ypos;
}sTouchData;



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

//uint8_t Touch_IsTouchingNow(void);
uint8_t Touch_GotATouch();
uint8_t Touch_WaitForUntouch(uint16_t delay);
uint8_t Touch_WaitForTouch(uint16_t delay);
uint16_t Touch_PollAxis(uint8_t cmd);
sTouchData Touch_GetXYtouch(void);


#endif /* __XPT2046_H */

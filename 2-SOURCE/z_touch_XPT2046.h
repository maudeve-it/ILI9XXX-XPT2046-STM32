/*
 *
 *  Created on: 2 giu 2022
 *      Author: mauro
 *
 *  licensing: https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/blob/c097f0e7d569845c1cf98e8d930f2224e427fd54/LICENSE
 *
 *	WARNING WARNING WARNING:
 *	in main.h put the #insert of this file BELOW the #insert of z_displ_ILIxxxx.h
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
/***** END OF "USER/PROJECT PARAMETERS" *****/



/**************** DEVICE PARAMETERS ***************/
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
 * parameters for the linear conversion from a touch sensor reading, to
 * the XY display position
 * use the formula:
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


/***** END OF "DEVICE PARAMETERS" *****/



/**********************************************************************************
 * program declarations and definitions
 **********************************************************************************/


typedef struct {
	uint8_t	isTouch;
	uint16_t Xpos;
	uint16_t Ypos;
}sTouchData;


void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

uint8_t Touch_In_XY_area(uint16_t xpos,uint16_t ypos,uint16_t width,uint16_t height);
uint8_t Touch_GotATouch(uint8_t reset);
uint8_t Touch_WaitForUntouch(uint16_t delay);
uint8_t Touch_WaitForTouch(uint16_t delay);
sTouchData Touch_GetXYtouch();


#endif /* __XPT2046_H */

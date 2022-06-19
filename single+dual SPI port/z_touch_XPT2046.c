/*
 * z_touch_XPT2046.c
 *
 *  Created on: 2 giu 2022
 *      Author: mauro
 *
 *  licensing: https://github.com/maudeve-it/ILI9486-STM32/blob/main/LICENSE
 *
 */



#include "main.h"

extern SPI_HandleTypeDef TOUCH_SPI_PORT;
extern volatile uint8_t Displ_SpiAvailable;  			// 0 if SPI is busy or 1 if it is free (transm cplt)

extern Displ_Orientat_e current_orientation;			// indicates the active display orientation. Set by Displ_Orientation

volatile uint8_t Touch_PenDown=0;						// set to 1 by pendown interrupt callback, reset to 0 by sw



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin==TOUCH_INT_Pin)
		Touch_PenDown=1;

}





/******************************************
 * @brief	enable touch, disabling display
 ******************************************/
void Touch_Select(void) {
	if (!HAL_GPIO_ReadPin(DISPL_CS_GPIO_Port, DISPL_CS_Pin))
		HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_RESET);
	if (TOUCH_SPI==DISPL_SPI){
		SET_SPI_BAUDRATE(TOUCH_PRESCALER);	//change SPI port speed as per touch needs
	}
}




/******************************************
 * @brief	disable touch
 ******************************************/
void Touch_UnSelect(void) {
	HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET);
}





/*******************************************************************************
 * @brief			Poll display for the current level of X, Y, or Z
 * @params	axis	use only one of the three options X_AXIS, Y_AXIS or Z_AXIS
 * @return			the level measured on the "axis" axis
 * 					PLEASE NOTE this function should be only for internal usage
 * 								left here available only for test and training.
 * 								Use Touch_GetXYTouch() instead of this function
 *******************************************************************************/
uint16_t Touch_PollAxis(uint8_t axis) {
	uint8_t poll[2] = {0,0};
	uint32_t poll16;
	
	while (!Displ_SpiAvailable) {};  // waiting for a free SPI port. Flag is set to 1 by transmission-complete interrupt callback

	HAL_NVIC_DisableIRQ(PENDOWN_IRQ);
	Touch_Select();

	Displ_SpiAvailable=0;
	HAL_SPI_Transmit(&TOUCH_SPI_PORT, &axis, 1, 10);
	if (HAL_SPI_Receive(&TOUCH_SPI_PORT, poll, 2, 10) == HAL_OK) {
		poll16 = (poll[0]<<8) + poll[1];
	}
	else {
		poll16 = 0;
	}
	Touch_UnSelect();
	Displ_SpiAvailable=1;
	if (poll16!=0)
		__NOP();
	HAL_NVIC_ClearPendingIRQ(PENDOWN_IRQ);
	HAL_NVIC_EnableIRQ(PENDOWN_IRQ);
	HAL_NVIC_ClearPendingIRQ(PENDOWN_IRQ);
	Touch_PenDown=0;    //reset interrupt flag
	return poll16;
}





/***********************************************************
 * @brief			poll touch screen and returning its XY screen position
 * #return	xypos	isTouch 	is 1 if detected a touch, otherwise 0;
 * 					Xpos,Ypos	in case isTouch=1 contain touch coordinates
 ************************************************************/
sTouchData Touch_GetXYtouch(void) {
const uint8_t pollingLevel=4;
sTouchData XYposition;
uint8_t k;

uint32_t touchx,touchy,touch;

// if no PenDown notification, return a no touch
	XYposition.isTouch=0;
	if (!Touch_PenDown)
		return XYposition;  // no touch: return 0
	Touch_PenDown=0;

	touch=0;
	for (k=0;k<(1<<pollingLevel);k++)
		touch += Touch_PollAxis(Y_AXIS);
	touch >>= pollingLevel; //takes the average

	touchy=(AY*touch+BY);

	touch=0;
	for (k=0;k<(1<<pollingLevel);k++)
		touch += Touch_PollAxis(X_AXIS);
	touch >>= pollingLevel;  //takes the average

	if (touch<=X_THRESHOLD)
		return XYposition;	// no touch: return 0
	touchx=(AX*touch+BX);

// calculating coordinates as per screen orientation
	switch (current_orientation) {
	case TOUCH0:
		XYposition.Xpos=touchx;
		XYposition.Ypos=touchy;
		break;
	case TOUCH90:
		XYposition.Xpos=touchy;
		XYposition.Ypos=TOUCH_0_WIDTH-touchx;
		break;
	case TOUCH180:
		XYposition.Xpos=TOUCH_0_WIDTH-touchx;
		XYposition.Ypos=(TOUCH_0_HEIGHT - touchy);
		break;
	case TOUCH270:
		XYposition.Xpos=(TOUCH_0_HEIGHT- touchy);
		XYposition.Ypos=touchx;
		break;
	}
// set flag indicating there was a touch
	XYposition.isTouch=1;
	return XYposition;
}





/***********************************************************
 * @brief			wait for a touch within an assigned time
 * @params	delay	max time (ms) waiting for a touch, 0=infinite
 * #return	1 		if touched within "delay" period
 * 			0		if elapsed time with no touch
 * 					PLEASE NOTE: not resetting Touch_PenDown
 ***********************************************************/
uint8_t Touch_WaitForTouch(uint16_t delay) {
	uint16_t starttime;

	starttime = HAL_GetTick();
	while (!Touch_PenDown) {
		if ((delay!=0) && ((HAL_GetTick()-starttime)>delay))
			return 0;
	};
	return 1;
}






/*************************************************************
 * @brief			wait for the pen left within an assigned time
 * @params	delay	max time (ms) waiting for leaving touch, 0=infinite
 * #return	1 		if display with no touch
 * 			0		if elapsed time still touching display
 *************************************************************/
uint8_t Touch_WaitForUntouch(uint16_t delay) {
	uint16_t starttime;
	uint8_t pen_up=0;

	starttime = HAL_GetTick();
	while (1) {
		if ((delay!=0) && ((HAL_GetTick()-starttime)>delay))
			return 0;
		if (Touch_PollAxis(Z_AXIS)<=Z_THRESHOLD)
			pen_up=1;
//		if (Touch_PollAxis(Y_AXIS)>=Y_THRESHOLD)     // Y_AXIS no more used since introducing ILI9488
//			pen_up=1;
		if (Touch_PollAxis(X_AXIS)<=X_THRESHOLD)
			pen_up=1;
		if (pen_up) {            //	Pen is now up: reset Touch_PenDown anyway.
			HAL_Delay(10);		// pen is Up just now: wait just a few
			Touch_PenDown=0;
			return 1;
		}
	}
}






/***********************************************************
 * @brief			check if interrupt registered a touch
 * @params	delay	max time (ms) waiting for a touch, 0=infinite
 * #return	1 		if touched within "delay" period
 * 			0		if elapsed time with no touch
 ***********************************************************/
uint8_t Touch_GotATouch() {

	return Touch_PenDown;
}






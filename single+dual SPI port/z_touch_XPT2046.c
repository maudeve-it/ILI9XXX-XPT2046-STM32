/*
 * z_touch_XPT2046.c
 *
 *  Created on: 2 giu 2022
 *      Author: mauro
 *
 *  licensing: https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/blob/c097f0e7d569845c1cf98e8d930f2224e427fd54/LICENSE
 *
 */



#include "main.h"

extern SPI_HandleTypeDef TOUCH_SPI_PORT;
extern volatile uint8_t Displ_SpiAvailable;  			// 0 if SPI is busy or 1 if it is free (transm cplt)

extern Displ_Orientat_e current_orientation;			// indicates the active display orientation. Set by Displ_Orientation

volatile uint8_t Touch_PenDown=0;						// set to 1 by pendown interrupt callback, reset to 0 by sw


//void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin){
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin==TOUCH_INT_Pin)
		if (!HAL_GPIO_ReadPin(TOUCH_INT_GPIO_Port, GPIO_Pin))
			Touch_PenDown=1;

}




/******************************************
 * @brief	enable touch, disabling display
 ******************************************/
void Touch_Select(void) {
	if (!HAL_GPIO_ReadPin(DISPL_CS_GPIO_Port, DISPL_CS_Pin))
		HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_SET);
	if (TOUCH_SPI==DISPL_SPI){
		SET_SPI_BAUDRATE(TOUCH_PRESCALER);	//change SPI port speed as per touch needs
	}
	HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_RESET);
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
 * 								Use Touch_GetXYTouch() instead
 *******************************************************************************/
uint16_t Touch_PollAxis(uint8_t axis) {
	uint8_t poll[2] = {0,0};
	uint32_t poll16;
	
	while (!Displ_SpiAvailable) {};  // waiting for a free SPI port. Flag is set to 1 by transmission-complete interrupt callback

// disable interrupt while enquiring the touch sensor because it triggers the interrupt pin
	HAL_NVIC_DisableIRQ(TOUCH_INT_EXTI_IRQn);
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

//enable back interrupt after reading the sensor
	HAL_NVIC_ClearPendingIRQ(TOUCH_INT_EXTI_IRQn);
	HAL_NVIC_EnableIRQ(TOUCH_INT_EXTI_IRQn);
//	HAL_NVIC_ClearPendingIRQ(TOUCH_INT_EXTI_IRQn);
	Touch_PenDown=0;    //reset interrupt flag, anyway.

	return poll16;
}





/*********************************************************************************
 * @brief			polls touch screen and returning its XY screen position
 * 					that's regardless touch recording flag (interrupt received)
 * @return	xypos	isTouch 	is 1 if detected a touch, otherwise 0;
 * 					Xpos,Ypos	in case isTouch=1 contain touch coordinates
 *********************************************************************************/
sTouchData Touch_GetXYtouch(void) {
const uint8_t pollingLevel=4;
sTouchData XYposition;
uint8_t k;

uint32_t touchx,touchy,touch;



// device has a touch get the average value (over "pollingLevel" attempts of X, Y and Z axes readings)

	// reading Z
	touch=0;
	for (k=0;k<(1<<pollingLevel);k++)
		touch += Touch_PollAxis(Z_AXIS);
	touch >>= pollingLevel;  //takes the average
	if (touch<=Z_THRESHOLD) {
		XYposition.isTouch=0;
		return XYposition;	// no touch: return 0
	}

	// reading X
	touch=0;
	for (k=0;k<(1<<pollingLevel);k++)
		touch += Touch_PollAxis(X_AXIS);
	touch >>= pollingLevel;  //takes the average
	if (touch<=X_THRESHOLD) {
		XYposition.isTouch=0;
		return XYposition;	// no touch: return 0
	}
	touchx=(AX*touch+BX);

	// reading Y - there is no a threshold for Y
	touch=0;
	for (k=0;k<(1<<pollingLevel);k++)
		touch += Touch_PollAxis(Y_AXIS);
	touch >>= pollingLevel; //takes the average

	touchy=(AY*touch+BY);


//having X and Y axis average values
// calculating coordinates as per screen orientation
	switch (current_orientation)
	{
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
 * 					PLEASE NOTE: not resetting Touch recording flag
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
 * #return	1 		if no touch on display
 * 			0		if elapsed time still touching display
 * PLEASE NOTE		if pen up, it resets the touch recording flag
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
//		if (Touch_PollAxis(Y_AXIS)>=Y_THRESHOLD)     // check on Y_AXIS no more used since introducing ILI9488
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
 * 					resetting touch flag anyway
 * @returns		1	if recorded a touch
 * 				0	if no touch recorded
 ***********************************************************/
uint8_t Touch_GotATouch() {
uint8_t result = Touch_PenDown;
	Touch_PenDown=0;
	return result;
}






/***********************************************************
 * @brief			check if there is a touch inside the
 * 					display area defined by parameters
 * @params	xpos,
 * 			ypos,
 * 			width,
 * 			height	display area to be polled for a touch
 * @return	1		if there is a touch inside area
 * 			0		if no touch or touch outside area defined
 ***********************************************************/
uint8_t Touch_In_XY_area(uint16_t xpos,uint16_t ypos,uint16_t width,uint16_t height) {
sTouchData posXY;
	posXY=Touch_GetXYtouch();
	if (!posXY.isTouch)
		return 0;
	if (posXY.Xpos>=xpos)
		if (posXY.Xpos<xpos+width)
			if (posXY.Ypos>=ypos)
				if (posXY.Ypos<ypos+height)
					return 1;
	return 0;
}



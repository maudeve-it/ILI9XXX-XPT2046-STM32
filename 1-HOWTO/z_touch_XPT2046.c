/*
 * z_touch_XPT2046.c
 * 	rel. TouchGFX.1.20
 *
 *  Created on: 2 giu 2022
 *      Author: mauro
 *
 *  licensing: https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/blob/c097f0e7d569845c1cf98e8d930f2224e427fd54/LICENSE
 *
 *	Install and use this library following instruction on: https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32
 *
*/


/*
 *
 * change "return" inside sampleTouch() into STM32TouchController.cpp file
 * as below
 * and add also the below include
 *

#include "main.h"

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
	return ((bool) Touch_TouchGFXSampleTouch(&x, &y));
}
*/


#include "main.h"

extern SPI_HandleTypeDef TOUCH_SPI_PORT;
extern volatile uint8_t Displ_SpiAvailable;  			// 0 if SPI is busy or 1 if it is free (transm cplt)

extern Displ_Orientat_e current_orientation;			// indicates the active display orientation. Set by Displ_Orientation

volatile uint8_t Touch_PenDown=0;						// set to 1 by pendown interrupt callback, reset to 0 by sw



void Touch_HandlePenDownInterrupt (){
		Touch_PenDown=1;
#ifdef DISPLAY_USING_TOUCHGFX
		touchgfxSignalVSync();
#endif
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin==TOUCH_INT_Pin){
		Touch_HandlePenDownInterrupt();
	}
}



void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin){
	HAL_GPIO_EXTI_Callback(GPIO_Pin);
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){
	HAL_GPIO_EXTI_Callback(GPIO_Pin);
}




/******************************************
 * @brief	enable touch, disabling display
 * 			set SPI baudrate as needed
 ******************************************/
void Touch_Select(void) {
	if (TOUCH_SPI==DISPL_SPI){														// if touch and display share the same SPI port
		if (!HAL_GPIO_ReadPin(DISPL_CS_GPIO_Port, DISPL_CS_Pin))					// if display selected
			HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_SET); 		// unselect display
		SET_SPI_BAUDRATE(TOUCH_PRESCALER);											//change SPI port speed as per touch needs
		HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_RESET);
	}
}



/******************************************
 * @brief	disable touch
 ******************************************/
void Touch_UnSelect(void) {
	HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET);				// unselect touch
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
	uint8_t PenDownStatus=Touch_PenDown;
	
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
	Touch_PenDown=PenDownStatus;	// restore Touch_PendDown value entering this routine

	return poll16;
}





/*********************************************************************************
 * @brief			polls touch screen and returning its XY screen position
 * 					that's regardless touch recording flag (interrupt received)
 * @return	x,y		in case isTouch=1 contain touch coordinates
 * 			isTouch is 1 if detected a touch, otherwise 0;
 *********************************************************************************/
void Touch_GetXYtouch(uint16_t *x, uint16_t *y, uint8_t *isTouch){

const uint8_t pollingLevel=4;
//sTouchData XYposition;
uint8_t k;

uint32_t touchx,touchy,touch;



// get the average value (over "1<<pollingLevel" attempts of X, Y and Z axes readings)

	// reading Z
	touch=0;
	for (k=0;k<(1<<pollingLevel);k++)
		touch += Touch_PollAxis(Z_AXIS);
	touch >>= pollingLevel;  //takes the average
	if (touch<=Z_THRESHOLD) {
		*isTouch=0;
		return;	// no touch: return 0
	}

	// reading X
	touch=0;
	for (k=0;k<(1<<pollingLevel);k++)
		touch += Touch_PollAxis(X_AXIS);
	touch >>= pollingLevel;  //takes the average
	if (touch<=X_THRESHOLD) {
		*isTouch=0;
		return;	// no touch: return 0
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
		*x=touchx;
		*y=touchy;
		break;
	case TOUCH90:
		*x=touchy;
		*y=(TOUCH_0_WIDTH-touchx);
		break;
	case TOUCH180:
		*x=(TOUCH_0_WIDTH-touchx);
		*y=(TOUCH_0_HEIGHT - touchy);
		break;
	case TOUCH270:
		*x=(TOUCH_0_HEIGHT- touchy);
		*y=touchx;
		break;
	}

// set flag indicating there was a touch
	*isTouch=1;
	return;
}





/***********************************************************
 * @brief			wait for a touch within an assigned time
 * @params	delay	max time (ms) waiting for a touch, 0=infinite
 * #return	1 		if touched within "delay" period
 * 			0		if elapsed time with no touch
 * 					PLEASE NOTE: doesn't reset Touch recording flag
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
 * 					resetting touch flag if asked by parameter
 * 					if you just need to get touch, reset the flag anytime
 * 					If handle the whole touch time (e.g. handling key-repeat,
 * 						reset flag only when you get "no-touch" reading sensor
 * @params	reset	reset touch flag if non 0
 * @returns		1	if recorded a touch
 * 				0	if no touch recorded
 ***********************************************************/
uint8_t Touch_GotATouch(uint8_t reset) {
uint8_t result = Touch_PenDown;
	if (reset)
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
//sTouchData posXY;
uint16_t x,y;
uint8_t isTouch;
	Touch_GetXYtouch(&x, &y, &isTouch);
	if (!isTouch)
		return 0;
	if (x>=xpos)
		if (x<xpos+width)
			if (y>=ypos)
				if (y<ypos+height)
					return 1;
	return 0;
}



#ifdef DISPLAY_USING_TOUCHGFX

/***********************************************************
 * @brief	Linking function to TouchGFX
 * 			Handles key repeat (controlled by PAUSE_TO_KEY_REPEAT)
 * @return	1 if detected a touch, otherwise 0
 * @usage	in STM32TouchController.cpp add this line
 *			#include "main.h",
 *			then, into STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
 *			change "return false;"
 *			into:  "return ((bool) Touch_TouchGFXSampleTouch(&x, &y));"
 *
 * 			that's enough for touch integration
 ***********************************************************/
uint8_t Touch_TouchGFXSampleTouch(int32_t *x, int32_t *y){
	//	sTouchData result;
	uint8_t isTouch=0;				// preset to no touch
	uint16_t xx=0,yy=0;  			// need to convert library coordinates type (uint16_t) to TouchGFX one (int32_t)
	static uint8_t flipTouch=0;		// switches 0/1, on every function call, until sensor is touched allowing to return key repeat
	static uint32_t touchTime=0; 	// tick value get on the first touch. 0 means display untouched.

	if (Touch_GotATouch(0)){				// polls interrupt flag not resetting it
		Touch_GetXYtouch(&xx,&yy,&isTouch);	// get touch sensor position
		if (!isTouch){				// received a "no touch"
			touchTime=0;				// set display as untouched
			Touch_GotATouch(1);			// reset interrupt touch flag
		} else {					// display touched
			if (touchTime==0){				// if previously untouched
				touchTime=HAL_GetTick();	// store tick value at touch time
				flipTouch=1;				// set switch to send touch now
			} else {						// not a new touch
				if ((DELAY_TO_KEY_REPEAT > 0) && ((HAL_GetTick()-touchTime)>DELAY_TO_KEY_REPEAT)) {	// if timeout to key repeat is over (0 means no key repeat)
					flipTouch=!flipTouch;	// alternate every time function is called
				} else
					flipTouch=0;			// return a "no touch"
			}
			if (flipTouch) { 	// return position only if the switching flag is on
				*x=xx;
				*y=yy;
			} else {			// otherwise return "no touch" from display
				isTouch = 0;
			}
		}
	}
	return isTouch;
}

#endif

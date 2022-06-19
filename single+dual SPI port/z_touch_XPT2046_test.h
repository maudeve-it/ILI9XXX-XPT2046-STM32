/*
 * z_touch_XPT2046_test.h
 *
 *  Created on: 2 giu 2022
 *      Author: mauro
 *
 *  This is related to the functions testing features and showing performance
 *  you don't need this file in the production project
 *
 *  licensing: https://github.com/maudeve-it/ILI9486-STM32/blob/main/LICENSE
 *
 *  Do you want to test functions?
 *  follow STEP_1 in z_displ_ILI9488_test.c
 *  then:
 *
 *  STEP_2
 *	in the main loop remove or comment previous command and put:
 *	Touch_ShowData();					// shows data returned by XPT2046 controller: try touching everywhere the display
 *
 *  STEP_3
 *	in the main loop remove or comment previous command and put:
 *	Touch_TestCalibration();			// compute and shows parameters to adopt converting XPT2046 data into display coordinates.
 *		 								// compare the (green) data shown with data in z_touch_XPT2046.h and, in case of needs, change it.
 *
 *  STEP_4
 *	in the main loop remove or comment previous command and put:
 *	Touch_TestDrawing();				//move pen on display while touching to check quality of parameters detected on step 3
 *										// repeat test on 4 display orientation
 *
 */

#ifndef INC_Z_TOUCH_XPT2046_TEST_H_
#define INC_Z_TOUCH_XPT2046_TEST_H_

#include <stdlib.h>



#define NUM_READINGS	20		//reading repetitions in calibration




void Touch_ShowData(void);
void Touch_TestCalibration(void);
void Touch_PollingTime(void);
void DrawCross(uint16_t x,uint16_t y,uint16_t fcol);

void Touch_TestAreas(void);
void Touch_TestDrawing();

//void DrawCross(uint16_t x,uint16_t y,uint16_t fcol);
//void MoveCross(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t fcol,uint16_t bcol);



#endif /* INC_Z_TOUCH_XPT2046_TEST_H_ */

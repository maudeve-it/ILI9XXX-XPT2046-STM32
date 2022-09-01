/*
 * z_touch_XPT2046_test.h
 *
 *  Created on: 2 giu 2022
 *      Author: mauro
 *
 *  This is related to the functions testing features and showing performance
 *  you don't need this file in the production project
 *
 *  licensing: https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/blob/c097f0e7d569845c1cf98e8d930f2224e427fd54/LICENSE
 *
 *  Do you want to usae these test functions?
 *  follow STEP_1 in z_displ_ILI9488_test.c
 *  then:
 *
 *  STEP_2
 *	in the main loop remove or comment previous command and put:
 *	Touch_ShowData();					// it shows data returned by XPT2046 controller: try touching everywhere the display
 *
 *  STEP_3
 *	in the main loop remove or comment previous command and put:
 *	Touch_TestCalibration();			// it compute and shows parameters to adopt converting XPT2046 data into display coordinates.
 *		 								// compare the (green) data shown with data in z_touch_XPT2046.h and, in case of needs, change it.
 *
 *  STEP_4
 *	in the main loop remove or comment previous command and put:
 *	Touch_TestDrawing();				// move pen on display while touching to check quality of parameters detected and set on step 3
 *										// it repeats test on 4 display orientation
 *
 */

#ifndef INC_Z_TOUCH_XPT2046_TEST_H_
#define INC_Z_TOUCH_XPT2046_TEST_H_

#include <stdlib.h>



#define NUM_READINGS	40		//reading repetitions in calibration




void Touch_ShowData(void);
void Touch_TestCalibration(void);
void DrawCross(uint16_t x,uint16_t y,uint16_t fcol);
void Touch_TestDrawing();



#endif /* INC_Z_TOUCH_XPT2046_TEST_H_ */

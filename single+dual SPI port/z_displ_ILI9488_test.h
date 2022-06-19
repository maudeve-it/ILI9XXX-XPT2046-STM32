/*
 * z_displ_ILI9488_test.h
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
 *
 *  STEP 1
 *
 *  on main.c (USER CODE BEGIN 2) put:
 *  Displ_Init(Displ_Orientat_180);			// initialize display controller
 *  Displ_CLS(BLACK);						// clear the screen
 *  Displ_BackLight('I');  					// initialize backlight
 *  Displ_BackLight('1');					// light-up display ad max light level
 *
 *  then, in the main loop put:
 *  Displ_PerfTest();						// shows graphics, measuring time.
 *  and run.
 *  Next STEPs on z_touch_XPT2046_test.c
 *
 *
 */

#ifndef INC_Z_DISPL_ST7735_TEST_H_
#define INC_Z_DISPL_ST7735_TEST_H_

#include <stdio.h>
#include <stdlib.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))


#define BGCOLOR BLACK

void Displ_PerfTest();
void Displ_ColorTest();
void Displ_TestAll ();

#endif /* INC_Z_DISPL_ST7735_TEST_H_ */

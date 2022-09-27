/*
 * z_displ_ILI9488_test.c
 *
 *  Created on: Mar 25, 2022
 *      Author: mauro
 *
 *  This is related to the functions testing features and showing performance
 *  you don't need this file in the production project
 *
 *  licensing: https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/blob/c097f0e7d569845c1cf98e8d930f2224e427fd54/LICENSE
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
 *
 *  Next STEPs on z_touch_XPT2046_test.c
 *
 *
 */



#include "main.h"

extern int16_t _width;       								///< (oriented) display width
extern int16_t _height;      								///< (oriented) display height





void testLines(uint16_t color)
{
    int           x1, y1, x2, y2,
                  w = _width,
                  h = _height;

    Displ_CLS(BLACK);

    x1 = y1 = 0;
    y2    = h - 1;
//    for (x2 = 0; x2 < w; x2 += 6) drawLine(x1, y1, x2, y2, color);
    for (x2 = 0; x2 < w; x2 += 6) Displ_Line(x1, y1, x2, y2, color);
    x2    = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) Displ_Line(x1, y1, x2, y2, color);

    Displ_CLS(BLACK);

    x1    = w - 1;
    y1    = 0;
    y2    = h - 1;
    for (x2 = 0; x2 < w; x2 += 6) Displ_Line(x1, y1, x2, y2, color);
    x2    = 0;
    for (y2 = 0; y2 < h; y2 += 6) Displ_Line(x1, y1, x2, y2, color);

    Displ_CLS(BLACK);

    x1    = 0;
    y1    = h - 1;
    y2    = 0;
    for (x2 = 0; x2 < w; x2 += 6) Displ_Line(x1, y1, x2, y2, color);
    x2    = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) Displ_Line(x1, y1, x2, y2, color);

    Displ_CLS(BLACK);

    x1    = w - 1;
    y1    = h - 1;
    y2    = 0;
    for (x2 = 0; x2 < w; x2 += 6) Displ_Line(x1, y1, x2, y2, color);
    x2    = 0;
    for (y2 = 0; y2 < h; y2 += 6) Displ_Line(x1, y1, x2, y2, color);

}




void testFastLines(uint16_t color1, uint16_t color2)
{
    int           x, y, w = _width, h = _height;

    Displ_CLS(BLACK);
    for (y = 0; y < h; y += 5) Displ_Line(0, y, w-1, y, color1);
    for (x = 0; x < w; x += 5) Displ_Line(x, 0, x, h-1, color2);
}





void testRects(uint16_t color)
{
	int           n, i, i2,
	cx = _width  / 2,
	cy = _height / 2;

	Displ_CLS(BLACK);
	n     = min(_width, _height);
	for (i = 2; i < n; i += 6) {
		i2 = i / 2;
		Displ_Border(cx - i2, cy - i2, i, i, 1, color);
	}
}




void testFilledRects(uint16_t color1, uint16_t color2)
{
	int           n, i, i2,
	cx = _width  / 2 - 1,
	cy = _height / 2 - 1;

	Displ_CLS(BLACK);
	n = min(_width, _height);
	for (i = n; i > 0; i -= 6) {
		i2    = i / 2;
		Displ_FillArea(cx - i2, cy - i2, i, i, color1);
		Displ_Border(cx - i2, cy - i2, i, i, 1, color2);
	}
}




void testFilledCircles(uint8_t radius, uint16_t color)
{
    int x, y, w = _width, h = _height, r2 = radius * 2;
    Displ_CLS(BLACK);
    for (x = radius; x < w; x += r2) {
        for (y = radius; y < h; y += r2) {
        	Displ_fillCircle(x, y, radius, color);
        }
    }
}






void testCircles(uint8_t radius, uint16_t color)
{
    int           x, y, r2 = radius * 2,
                        w = _width  + radius,
                        h = _height + radius;
    // Screen is not cleared for this one -- this is
    // intentional and does not affect the reported time.
    for (x = 0; x < w; x += r2) {
        for (y = 0; y < h; y += r2) {
            Displ_drawCircle(x, y, radius, color);
        }
    }
}






void testTriangles()
{
    int           n, i, cx = _width  / 2 - 1,
                        cy = _height / 2 - 1;

    Displ_CLS(BLACK);
    n     = min(cx, cy);
    for (i = 0; i < n; i += 5) {
    	Displ_drawTriangle(
            cx    , cy - i, // peak
            cx - i, cy + i, // bottom left
            cx + i, cy + i, // bottom right
            color565(0, 0, i));
    }
}





void testFilledTriangles() {
    int           i, cx = _width  / 2 - 1,
                     cy = _height / 2 - 1;

    Displ_CLS(BLACK);
    for (i = min(cx, cy); i > 10; i -= 5) {
    	Displ_fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color565(0, i, i));
    	Displ_drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color565(i, i, 0));
    }
}






void testRoundRects() {
    int           w, i, i2, red, step,
                  cx = _width  / 2 - 1,
                  cy = _height / 2 - 1;

    Displ_CLS(BLACK);
    w     = min(_width, _height);
    red = 0;
    step = (256 * 6) / w;
    for (i = 0; i < w; i += 6) {
        i2 = i / 2;
        red += step;
        Displ_drawRoundRect(cx - i2, cy - i2, i, i, i / 8, color565(red, 0, 0));
    }
}





void testFilledRoundRects() {
    int           i, i2, green, step,
                  cx = _width  / 2 - 1,
                  cy = _height / 2 - 1;

    Displ_CLS(BLACK);
    green = 256;
    step = (256 * 6) / min(_width, _height);
    for (i = min(_width, _height); i > 20; i -= 6) {
        i2 = i / 2;
        green -= step;
        Displ_fillRoundRect(cx - i2, cy - i2, i, i, i / 8, color565(0, green, 0));
    }
}





void TestChar(){

	uint16_t x,y,k,a,b;
	uint8_t c;

	for (k=0;k<2500;k++) {
		a=rand();
		b=rand();
		x=a % (_width-11) ;
		y=b % (_height-18) ;
		c=a % 26+'A';
		Displ_WChar(x, y, c, Font16, 1, YELLOW, RED);
		x=b % (_width-11) ;
		y=a % (_height-18) ;
		c=b % 26+'A';
		Displ_WChar(x, y, c, Font16, 1, RED, YELLOW);
	}
};



void wait(uint16_t delay){
uint16_t time;
volatile uint32_t dummy1,dummy2;

	time=HAL_GetTick();
	dummy1=0;
	while ((HAL_GetTick()-time)<delay){
		dummy2=dummy1;
		dummy1=dummy2;
	}

}



void TestFillScreen(uint16_t delay) {
	Displ_CLS(RED);
	if (delay)
		wait(delay);
    Displ_CLS(GREEN);
	if (delay)
		wait(delay);
    Displ_CLS(BLUE);
	if (delay)
		wait(delay);
    Displ_CLS(YELLOW);
	if (delay)
		wait(delay);
    Displ_CLS(BLACK);
	if (delay)
		wait(delay);
}






void TestHVLine() {
	uint16_t k,x,y,l,a,b;
    Displ_CLS(BLACK);
    for (k=0 ; k<15000;k++) {
    	a=rand();
    	b=rand();
    	x=a % _width ;
    	y=b % _height ;
    	l=a % (_width - x);
    	Displ_FillArea(x, y, l, 1, BLUE);
    	x=b % _width ;
    	y=a % _height ;
    	l=b % (_height - y);
    	Displ_FillArea(x, y, 1, l, CYAN);
    }
}






/* @brief private function for TestDisplay() */
void Displ_Page(char * str1,char * str2,char * str3, uint8_t mode) {
const uint16_t bcol0=BLACK, col1=WHITE, col2=WHITE, col3=WHITE, bcol1=BLUE, bcol2=BLACK, bcol3=BLACK;
	Displ_FillArea(0, 21, _width, 72, bcol0);
	Displ_CString(1,21, _width-1, 21+24, str1, Font24, 1, col1,bcol1);
	Displ_CString(1,54, _width-1, 54+24, str2, Font20, 1, col2,bcol2);
	Displ_CString(1,77, _width-1, 77+24, str3, Font20, 1, col3,bcol3);
}






void testFillScreen()
{
	Displ_CLS(RED);
	Displ_CLS(GREEN);
	Displ_CLS(BLUE);
	Displ_CLS(YELLOW);
	Displ_CLS(BLACK);
}






void Displ_ColorTest(){
	const uint8_t colnum=8;
	const uint8_t rownum=3;
	const uint16_t colortab[]={DD_WHITE,RED,BLUE,GREEN,YELLOW,MAGENTA,CYAN,WHITE, \
								DDD_WHITE,D_RED,D_BLUE,D_GREEN,D_YELLOW,D_MAGENTA,D_CYAN,D_WHITE, \
								DDDD_WHITE,DD_RED,DD_BLUE,DD_GREEN,DD_YELLOW,DD_MAGENTA,DD_CYAN,DD_WHITE};
	static Displ_Orientat_e orientation = Displ_Orientat_0;
	uint16_t x,y,dx,dy;
	Displ_Orientation(orientation);
	if (orientation==Displ_Orientat_0)
		orientation=Displ_Orientat_90;
	else
		orientation=Displ_Orientat_0;
	dx=_width/colnum;
	dy=_height/rownum;
	for(y=0; y<rownum;y++){
		for(x=0; x<colnum; x++){
			Displ_FillArea(x*dx, y*dy, dx, dy, colortab[y*colnum+x]);
		}
		__NOP();
	}
	if ((x*dx)<_width)
		Displ_FillArea(x*dx, 0, (_width-x*dx), _height, BLACK);
	if ((y*dy)<_height)
		Displ_FillArea(0, y*dy, _width, (_height-y*dy), BLACK);
}







void Displ_TestAll (){
	testFillScreen();
	testLines(CYAN);
	testFastLines(RED, BLUE);
	testRects(GREEN);
	testFilledRects(YELLOW, MAGENTA);
	testFilledCircles(10, MAGENTA);
	testCircles(10, WHITE);
	testTriangles();
	testFilledTriangles();
	testRoundRects();
	testFilledRoundRects();
}





void Displ_PerfTest() {
	uint32_t time[6];
	char riga[40];
	uint8_t k;

	for (uint8_t k1=0;k1<4;k1++){

		switch (k1){
		case 0:
			Displ_Orientation(Displ_Orientat_0);
			break;
		case 1:
			Displ_Orientation(Displ_Orientat_90);
			break;
		case 2:
			Displ_Orientation(Displ_Orientat_180);
			break;
		case 3:
			Displ_Orientation(Displ_Orientat_270);
			break;
		}

		time[1] = HAL_GetTick();
		Displ_TestAll();
		time[1] = HAL_GetTick()-time[1];
		sprintf(riga,"%ld ms",time[1]);
		Displ_Page("TEST 1","TestAll:",riga,0);
		HAL_Delay(3000);

		time[2] = HAL_GetTick();
		for (k=0;k<10;k++)
			TestFillScreen(0);
		time[2] = HAL_GetTick()-time[2];
		sprintf(riga,"%ld ms",time[2]);
		Displ_Page("TEST 2","50 screens:",riga,0);
		HAL_Delay(3000);

		time[3] = HAL_GetTick();
		TestHVLine();
		time[3] = HAL_GetTick()-time[3];
		sprintf(riga,"%ld ms",time[3]);
		Displ_Page("TEST 3","30k lines:",riga,0);
		HAL_Delay(3000);

		Displ_CLS(BLACK);
		time[4] = HAL_GetTick();
		TestChar();
		time[4] = HAL_GetTick()-time[4];
		sprintf(riga,"%ld ms",time[4]);
		Displ_Page("TEST 4","5000 chars:",riga,0);
		HAL_Delay(3000);


		Displ_CLS(BGCOLOR);

		Displ_FillArea(0, 0, _width, _height, BGCOLOR);

		uint16_t deltah=Font24.Height;
		Displ_CString(0,0,_width,deltah, "RESULTS", Font24, 1, WHITE,BLUE);


		for (uint8_t k=1;k<5;k++) {
			switch (k) {
			case 0:
				//				sprintf(riga,"INITIAL SETUP: %ld ms",time[k]);
				break;
			case 1:
				sprintf(riga,"STD TEST   %4ld",time[k]);
				break;
			case 2:
				sprintf(riga,"50 SCREENS %4ld",time[k]);
				break;
			case 3:
				sprintf(riga,"30k LINES  %4ld",time[k]);
				break;
			case 4:
				sprintf(riga,"5k CHARS   %4ld",time[k]);
				break;
			}
			//		Displ_WString(0, 10+deltah+(k)*Font20.Height*2, riga, Font20, 1, WHITE, BGCOLOR);
			Displ_CString(0, 10+deltah+(k)*Font20.Height*2, _width,  10+deltah+Font20.Height*((k)*2+1),riga, Font20, 1, WHITE, BGCOLOR);
		};

#ifdef DISPLAY_DIMMING_MODE
		{
			uint32_t currlevel=Displ_BackLight('Q'); 		//query about the current dimming level and store it;
			uint32_t maxlevel=Displ_BackLight('F'); 		//set display backlight 100% and get the corresponding value (that means ARR)
			uint16_t dtime=4000/(maxlevel*3);				//set dtime to let the three below loops run in about 4 seconds regardless the ARR value defined in CubeMX
			if (dtime==0)
				dtime=1;
			Displ_BackLight('0');  // turn off display

			for (uint32_t k=0; k<=maxlevel;k++){
				Displ_BackLight('+');  						// increase light
				HAL_Delay(dtime);
			}
			HAL_Delay(100);
			for (uint32_t k=0; k<=maxlevel;k++){
				Displ_BackLight('-');  						// decrease light
				HAL_Delay(dtime);
			}
			HAL_Delay(100);
			for (uint32_t k=0; k<=maxlevel;k++){
				Displ_BackLight('+');  						// increase light
				HAL_Delay(dtime);
			}
			HAL_Delay(1000);								// 1 second final delay. total time showing results is 5 seconds

			Displ_BackLight('0');							//restoring previous backlight level: start resetting backlight level to 0
			while (currlevel>Displ_BackLight('+')) {};		//restoring previous backlight level: now, increase light until it is set to the "currlevel" stored above
		}
#else
		HAL_Delay(5000);
#endif
	}
}




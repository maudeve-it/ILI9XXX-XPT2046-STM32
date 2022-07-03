/*
 * z_touch_XPT2046_test.c
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


#include "main.h"

extern int16_t _width;       								///< (oriented) display width
extern int16_t _height;      								///< (oriented) display height





/*************************************************************
 * used by Touch_TestDrawing() and Touch_TestCalibration()
 *************************************************************/
void DrawCross(uint16_t x,uint16_t y,uint16_t fcol){
	uint8_t ray=10;
	Displ_Line(x-ray, y-ray, x+ray, y+ray, fcol);
	Displ_Line(x-ray, y+ray, x+ray, y-ray, fcol);
	Displ_Line(x-ray, y, x+ray, y, fcol);
	Displ_Line(x, y-ray, x, y+ray, fcol);

}





/*************************************************************
 * used by Touch_TestDrawing() and Touch_TestCalibration()
 *************************************************************/
void MoveCross(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t fcol,uint16_t bcol){
	const uint8_t steps=20;
	int16_t deltax,deltay;

	deltax = (x2-x1)/steps;
	deltay = (y2-y1)/steps;

	while ((x1!=x2) || (y1!=y2)) {
		DrawCross(x1,y1,bcol);

		x1=((abs((x2-x1))>abs(deltax)) ? (x1+deltax) : x2);
		y1=((abs((y2-y1))>abs(deltay)) ? (y1+deltay) : y2);

		DrawCross(x1,y1,fcol);
		HAL_Delay(1000/steps);
	}
}
















extern uint16_t Touch_PollAxis(uint8_t axis);


/*****************************************************
 * polls the 3 axis of touch device and shows values returned
 *****************************************************/
void Touch_ShowData(void)
{
	uint16_t x_touch;
	uint16_t y_touch;
	uint16_t z_touch;
	char text[30];

	Displ_FillArea(0,0,_width,_height,WHITE);

	while (1) {
		Displ_FillArea(101,5,40,10,RED);

		z_touch = Touch_PollAxis(Z_AXIS);
		x_touch = Touch_PollAxis(X_AXIS);
		y_touch = Touch_PollAxis(Y_AXIS);

		Displ_FillArea(101,5,40,10,WHITE);

		sprintf(text,"X=%#X      -",x_touch);
		Displ_WString(10,30,text,Font20,1,BLUE,WHITE);
		sprintf(text,"Y=%#X      -",y_touch);
		Displ_WString(10,50,text,Font20,1,BLUE,WHITE);
		sprintf(text,"Z=%#X      -",z_touch);
		Displ_WString(10,70,text,Font20,1,BLUE,WHITE);
		HAL_Delay(100);
	}

}



/****************************************
 * a test with a continue touch polling,
 * drawing values returned,
 * until touch is released
 ****************************************/
void Touch_TestDrawing() {
	extern uint8_t	Touch_PenDown;
	uint16_t px=0,py;
	sTouchData posTouch;

	for (uint8_t k=0;k<4;k++){

		switch (k){
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

		Displ_CLS(DD_BLUE);
		Displ_CString(0,10,_width,Font16.Height+10,"Test Drawing",Font16,1,WHITE,DD_BLUE);

		Touch_WaitForTouch(0);

		while (1) {
			Touch_PenDown=1;	//in this demo we want a continuous touch reading
			//Resetting Touch_PenDown I force a touch sensor reading
			//otherwise I would read just the first after the touch
			posTouch=Touch_GetXYtouch();
			if (!posTouch.isTouch) //if there is no touch: stop drawing
				break;
			if (px!=0)
				DrawCross(px,py,DD_BLUE);
			DrawCross(posTouch.Xpos,posTouch.Ypos,WHITE);
			px=posTouch.Xpos;
			py=posTouch.Ypos;
			HAL_Delay(30);
		}
	}
}






void Touch_TestCalibration(){

	const uint16_t shift=80;
	char text[30];
	uint16_t x[5]={shift,_width-shift,shift,_width-shift,_width>>1};
	uint16_t y[5]={shift,_height-shift,_height-shift,shift,_height>>1};
	uint32_t read_x[5]={0,0,0,0,0};
	uint32_t read_y[5]={0,0,0,0,0};
	uint32_t r_x,r_y,r_z;
	uint32_t xx,yy;
	uint8_t k,h;
	float ax[2];
	float bx[2];
	float ay[2];
	float by[2];
	float axx,bxx,ayy,byy,e;


	Displ_CLS(WHITE);
	for (uint8_t k=0; k<4; k++) {
		switch (k) {
		case 0:
			strcpy(text,"Press and briefly hold");
			break;
		case 1:
			strcpy(text,"stylus on the target.");
			break;
		case 2:
			strcpy(text,"Repeat as the target");
			break;
		case 3:
			strcpy(text,"moves around the screen.");
			break;
		}
		Displ_CString(0,10+Font12.Height*k,_width,10+Font12.Height*(1+k),text,Font12,1,BLACK,WHITE);

	}

	HAL_Delay(1000);
	Touch_WaitForUntouch(0);

	for (h=0;h<5;h++){

		DrawCross(x[h],y[h],BLACK);


// wait for stylus
		Touch_WaitForTouch(0);

// makes NUM_READINGS touch pollings calculating average value
		k=0;
		while (k<NUM_READINGS) {
			r_x=Touch_PollAxis(X_AXIS);
			r_y=Touch_PollAxis(Y_AXIS);
			r_z=Touch_PollAxis(Z_AXIS);
			if ((r_z>Z_THRESHOLD) && (r_x>X_THRESHOLD)) {
				read_x[h]+=r_x;
				read_y[h]+=r_y;
				k++;
				HAL_Delay(10);
			}

		}
		read_x[h]=read_x[h]/NUM_READINGS;
		read_y[h]=read_y[h]/NUM_READINGS;

		if (h!=4)
			MoveCross(x[h],y[h],x[h+1],y[h+1],BLACK,WHITE);

		// wait for user removing stylus
		Touch_WaitForUntouch(0);


	}

// detect ax,bx and ay,by parameters of the linear equation converting touch coordinates to display ones:
// x(display) = ax * x(touch) + bx
// y(display) = ay * y(touch) + by
// making two polling (ax{0] and ax[1],...
// and calculating average values into axx,...

	ax[0]=(x[0]+0.0f)-x[1];
	bx[0]=((x[1]+0.0f)*read_x[0])-((x[0]+0.0f)*read_x[1]);
	e=((read_x[0]+0.0f)-read_x[1]);
	ax[0]=ax[0]/e;
	bx[0]=bx[0]/e;

	ax[1]=(x[2]+0.0f)-x[3];
	bx[1]=((x[3]+0.0f)*read_x[2])-((x[2]+0.0f)*read_x[3]);
	e=((read_x[2]+0.0f)-read_x[3]);
	ax[1]=ax[1]/e;
	bx[1]=bx[1]/e;

	ay[0]=(y[0]+0.0f)-y[1];
	by[0]=((y[1]+0.0f)*read_y[0])-((y[0]+0.0f)*read_y[1]);
	ay[0]=ay[0]/((read_y[0]+0.0f)-read_y[1]);
	by[0]=by[0]/((read_y[0]+0.0f)-read_y[1]);

	ay[1]=(y[2]+0.0f)-y[3];
	by[1]=((y[3]+0.0f)*read_y[2])-((y[2]+0.0f)*read_y[3]);
	ay[1]=ay[1]/((read_y[2]+0.0f)-read_y[3]);
	by[1]=by[1]/((read_y[2]+0.0f)-read_y[3]);

	// axx is the average between ax[0] and ax[1]

	axx = (ax[0] + ax[1])/2;
	bxx = (bx[0] + bx[1])/2;
	ayy = (ay[0] + ay[1])/2;
	byy = (by[0] + by[1])/2;


	Displ_CLS(WHITE);

	k=1;
	sprintf(text,"Default:");
	Displ_WString(10,10+Font12.Height*k++,text,Font12,1,BLACK,WHITE);
	sprintf(text,"Ax=%f Bx=%f",AX,BX);
	Displ_WString(10,10+Font12.Height*k++,text,Font12,1,BLACK,WHITE);
	sprintf(text,"Ay=%f By=%f",AY,BY);
	Displ_WString(10,10+Font12.Height*k++,text,Font12,1,BLACK,WHITE);
	sprintf(text,"Current test:");
	Displ_WString(10,10+Font12.Height*k++,text,Font12,1,BLACK,WHITE);
	sprintf(text,"Ax=%f Bx=%f",ax[0],bx[0]);
	Displ_WString(10,10+Font12.Height*k++,text,Font12,1,RED,WHITE);
	sprintf(text,"Ay=%f By=%f",ay[0],by[0]);
	Displ_WString(10,10+Font12.Height*k++,text,Font12,1,RED,WHITE);
	sprintf(text,"Ax=%f Bx=%f",ax[1],bx[1]);
	Displ_WString(10,10+Font12.Height*k++,text,Font12,1,BLUE,WHITE);
	sprintf(text,"Ay=%f By=%f",ay[1],by[1]);
	Displ_WString(10,10+Font12.Height*k++,text,Font12,1,BLUE,WHITE);
	sprintf(text,"Ax=%f Bx=%f",axx,bxx);
	Displ_WString(10,10+Font12.Height*k++,text,Font12,1,DD_GREEN,WHITE);
	sprintf(text,"Ay=%f By=%f",ayy,byy);
	Displ_WString(10,10+Font12.Height*k++,text,Font12,1,DD_GREEN,WHITE);

	xx=(ax[0]*read_x[4]+bx[0]);
	yy=(ay[0]*read_y[4]+by[0]);
	DrawCross(xx,yy,RED);
	xx=(ax[1]*read_x[4]+bx[1]);
	yy=(ay[1]*read_y[4]+by[1]);
	DrawCross(xx,yy,BLUE);
	xx=(axx*read_x[4]+bxx);
	yy=(ayy*read_y[4]+byy);
	DrawCross(xx,yy,DD_GREEN);

	Touch_WaitForTouch(0);
}






/*
 * z_touch_XPT2046_menu.c
 *
 *  Created on: 13 giu 2022
 *      Author: mauro
 *
 *  licensing: https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/blob/c097f0e7d569845c1cf98e8d930f2224e427fd54/LICENSE
 *
 */


#include "main.h"


extern int16_t _width;
extern int16_t _height;


// menus declaration
MenuRow Menu1[5];
MenuRow Menu2[2];




void InitMenu(){
	const uint16_t eGap=30;
	const uint16_t vGap=20;
	const uint16_t vBord=20;

// Menu1
	for (uint8_t k=0;k<5;k++){
		Menu1[k].X=eGap;
		Menu1[k].Y=eGap+(2*vBord+vGap+Font24.Height)*k;
		Menu1[k].W=_width-2*eGap;
		Menu1[k].H=Font24.Height+2*vBord;
	}
	strcpy(Menu1[0].Desc,"Item1");
	strcpy(Menu1[1].Desc,"Item2");
	strcpy(Menu1[2].Desc,"Item3");
	strcpy(Menu1[3].Desc,"MENU 2");
	strcpy(Menu1[4].Desc,"EXIT");

// Menu2
	for (uint8_t k=0;k<2;k++){
		Menu2[k].X=eGap;
		Menu2[k].Y=eGap+(2*vBord+vGap+Font24.Height)*k;
		Menu2[k].W=_width-2*eGap;
		Menu2[k].H=Font24.Height+2*vBord;
	}
	strcpy(Menu2[0].Desc,"Item1");
	strcpy(Menu2[1].Desc,"BACK");

}



void DrawMenu(MenuRow *menu,uint8_t menusize){
	uint8_t k;
	Displ_CLS(BLACK);

	for (k=0;k<menusize;k++){
		Displ_CString(menu[k].X, menu[k].Y, menu[k].X+menu[k].W-1, menu[k].Y+menu[k].H-1,menu[k].Desc,Font24,1,WHITE,DD_BLUE);
		Displ_Border(menu[k].X, menu[k].Y, menu[k].W, menu[k].H,1,D_CYAN);
	}
}




uint8_t CheckMenu(MenuRow *menu,uint8_t menusize){
uint8_t k,result;
sTouchData posTouch;

// if not registred any touch return a null choice
	if (!Touch_GotATouch())
		return 255;
// otherwise
	posTouch=Touch_GetXYtouch();
	if ((posTouch.Xpos>_width) || (posTouch.Ypos>_height)) //that's not a touch
		return 255;
	result=254; //preset result as a touch outside menu buttons
	for (k=0;k<menusize;k++){
		if ((posTouch.Xpos>=menu[k].X) && (posTouch.Xpos<=(menu[k].X+menu[k].W)) && (posTouch.Ypos>=menu[k].Y) && (posTouch.Ypos<=(menu[k].Y+menu[k].H))) {  // the menu item was touched!
			Displ_CString(menu[k].X, menu[k].Y, menu[k].X+menu[k].W-1, menu[k].Y+menu[k].H-1,menu[k].Desc,Font24,1,WHITE,ORANGE);
			Displ_Border(Menu1[k].X, Menu1[k].Y, Menu1[k].W, Menu1[k].H,1,YELLOW);
			DrawCross(posTouch.Xpos,posTouch.Ypos,YELLOW);
			result=k;
			break;
		}
	}
	Touch_WaitForUntouch(0);
	return result;
}






void RunMenu2(){
	uint8_t result=0;
//	uint16_t delay; //delay to add after a touch detection and serving
//	uint16_t timeTouch; //time of the last touch

//	delay=50;
	while (1) {
		if (result!=255)
			DrawMenu(Menu2,	(sizeof(Menu2)/sizeof(Menu2[0])));
		result=CheckMenu(Menu2,(sizeof(Menu2)/sizeof(Menu2[0])));
		switch (result) {
		case 0:
			Displ_FillArea(10,10,20,20,GREEN);
			HAL_Delay(1000);
			break;
		case 1:
			return;
			break;
		case 254: //if touch outside menu items
			Displ_FillArea(10,10,20,20,MAGENTA);
			HAL_Delay(100);
			break;
		case 255: //no touch
			break;
		}
	}
}





void RunMenu1(){
	uint8_t result=0;
//	uint16_t delay; //delay to add after a touch detection and serving
//	uint16_t timeTouch; //time of the last touch

//	delay=50;
	while (1) {
		if (result!=255)
			DrawMenu(Menu1,	(sizeof(Menu1)/sizeof(Menu1[0])));
		result=CheckMenu(Menu1,(sizeof(Menu1)/sizeof(Menu1[0])));
		switch (result) {
		case 0:
		case 1:
		case 2:
			Displ_FillArea(10,10,20,20,GREEN);
			HAL_Delay(1000);
			break;
		case 3:
			RunMenu2();
			break;
		case 4:
			return;
			break;
		case 254: //if touch outside menu items
			Displ_FillArea(10,10,20,20,ORANGE);
			HAL_Delay(100);
			break;
		case 255: //no touch
			break;
		}
		if (result!=255)
			__NOP();

	}
}






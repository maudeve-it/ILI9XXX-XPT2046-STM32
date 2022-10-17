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
#define Menu1Size 5
sMenuItem Menu1[Menu1Size];
#define Menu2Size 2
sMenuItem Menu2[Menu2Size];





void InitMenu(){
#ifdef ILI9488
	const uint16_t eGap=30;
	const uint16_t vGap=20;
	const uint16_t vBord=20;
#endif
#ifdef ILI9341
	const uint16_t eGap=10;
	const uint16_t vGap=15;
	const uint16_t vBord=10;
#endif

// Menu1
	for (uint8_t k=0;k<Menu1Size;k++){
		Menu1[k].X=eGap;
		Menu1[k].Y=eGap+(2*vBord+vGap+Font24.Height)*k;
		Menu1[k].W=_width-2*eGap;
		Menu1[k].H=Font24.Height+2*vBord;
		Menu1[k].BkgUnsel=DD_BLUE;
		Menu1[k].BorUnsel=D_CYAN;
		Menu1[k].InkUnsel=WHITE;
		Menu1[k].BkgSel=ORANGE;
		Menu1[k].BorSel=YELLOW;
		Menu1[k].InkSel=WHITE;
		Menu1[k].font=Font24;
		Menu1[k].fontSize=1;

	}
	strcpy(Menu1[0].Desc,"Item1");
	strcpy(Menu1[1].Desc,"Item2");
	strcpy(Menu1[2].Desc,"Item3");
	strcpy(Menu1[3].Desc,"MENU 2");
	strcpy(Menu1[4].Desc,"EXIT");



// Menu2
	for (uint8_t k=0;k<Menu2Size;k++){
		Menu2[k].X=eGap;
		Menu2[k].Y=eGap+(2*vBord+vGap+Font24.Height)*k;
		Menu2[k].W=_width-2*eGap;
		Menu2[k].H=Font24.Height+2*vBord;
		Menu2[k].BkgUnsel=DD_BLUE;
		Menu2[k].BorUnsel=D_CYAN;
		Menu2[k].InkUnsel=WHITE;
		Menu2[k].BkgSel=ORANGE;
		Menu2[k].BorSel=YELLOW;
		Menu2[k].InkSel=WHITE;
		Menu2[k].font=Font24;
		Menu2[k].fontSize=1;
	}
	strcpy(Menu2[0].Desc,"Item1");
	strcpy(Menu2[1].Desc,"BACK");

}




/***************************************************************************
 * @brief 	menu element drawing
 * 			function of the menu system: you shouldn't need to change it
 * @params  menuItem	menuitem structure to draw
 * 			selected	0 uses unselected colors
 * 						1 uses selected
 ***************************************************************************/
void DrawMenuItem(sMenuItem *item, uint8_t selected){
uint16_t bor,bkg,ink;
	bor=(selected ? item->BorSel : item->BorUnsel);
	bkg=(selected ? item->BkgSel : item->BkgUnsel);
	ink=(selected ? item->InkSel : item->InkUnsel);
	Displ_CString(item->X, item->Y, item->X+item->W-1, item->Y+item->H-1,item->Desc,item->font,item->fontSize,ink,bkg);
	Displ_Border(item->X, item->Y, item->W, item->H,1,bor);
}





/***************************************************************************
 * @brief 	full menu drawing
 * 			function of the menu system: you shouldn't need to change it
 * @params  menu		structure to check
 * 			menusize	number of elements into menu
 ***************************************************************************/
void DrawMenu(sMenuItem *menu,uint8_t menusize){
uint8_t k;

	Displ_CLS(BLACK);
	for (k=0;k<menusize;k++){
		DrawMenuItem(&menu[k], 0);
	}
}




/***************************************************************************
 * @brief 	verifies if there is a touch over menu returning option selected
 * 			function of the menu system: you shouldn't need to change it
 * @params  menu		structure to check
 * 			menusize	number of elements into menu
 * @return	menu item selected
 ***************************************************************************/
uint8_t CheckMenu(sMenuItem *menu,uint8_t menusize){
uint8_t k,result;

// if not registred any touch return a null choice
	if (!Touch_GotATouch(1))
		return 255;

	for (k=0;k<menusize;k++){
		if (Touch_In_XY_area(menu[k].X,menu[k].Y,menu[k].W,menu[k].H)){  //polling if touch was in the current menu item area
			result=k;
			break;
		}
	}
	DrawMenuItem(&menu[k], 1); //draw selected

	Touch_WaitForUntouch(0);
	return result;
}





/******************************************************
 * @brief actions done by Menu1 - specific funcion to implement for each menu
 * 		  the "switch case" below defines actions to perform for each menu item
 * 		  consider that result has
 * 		  0-253 	the menu item chosen
 * 		  254 		kdisplay was clicked outside items menu area
 * 		  255		ther was no touch on the menu
 * 		  after any actions repeat menu unless there is a specific "return"
 ******************************************************/
void RunMenu1(){
	uint8_t result=0;
//	uint16_t delay; //delay to add after a touch detection and serving
//	uint16_t timeTouch; //time of the last touch

//	delay=50;
	while (1) {
		if (result!=255)
			DrawMenu(Menu1,	(sizeof(Menu1)/sizeof(Menu1[0])));
		result=CheckMenu(Menu1,(sizeof(Menu1)/sizeof(Menu1[0])));

//actions
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







/******************************************************
 * @brief actions done by Menu2 - specific funcion to implement for each menu
 * 		  the "switch case" below defines actions to perform for each menu item
 * 		  consider that result has
 * 		  0-253 	the menu item chosen
 * 		  254 		kdisplay was clicked outside items menu area
 * 		  255		ther was no touch on the menu
 * 		  after any actions repeat menu unless there is a specific "return"
 ******************************************************/
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




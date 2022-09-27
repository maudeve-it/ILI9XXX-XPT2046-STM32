/*
 * z_touch_XPT2046_menu.h
 *
 *  Created on: 13 giu 2022
 *      Author: mauro
 *
 *  licensing: https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/blob/c097f0e7d569845c1cf98e8d930f2224e427fd54/LICENSE
 *
 */

#ifndef INC_Z_TOUCH_XPT2046_MENU_H_
#define INC_Z_TOUCH_XPT2046_MENU_H_



// Menu data definitions and declarations


typedef struct {
	uint16_t X;			//position and size of the menu element
	uint16_t Y;
	uint16_t W;
	uint16_t H;
	char Desc[20];		//text of the menu item
	sFONT font;			//font to be used
	uint8_t fontSize;
	uint16_t BkgUnsel;   //background color when item is not selected
	uint16_t BorUnsel;   //border color when item is not selected
	uint16_t InkUnsel;   //text color when item is not selected
	uint16_t BkgSel;     //background color with item selected
	uint16_t BorSel;     //border color with item selected
	uint16_t InkSel;     //text color with item selected
}sMenuItem;


void InitMenu();
void DrawMenu(sMenuItem *menu,uint8_t menusize);
uint8_t CheckMenu(sMenuItem *menu,uint8_t menusize);
void RunMenu2();
void RunMenu1();







#endif /* INC_Z_TOUCH_XPT2046_MENU_H_ */

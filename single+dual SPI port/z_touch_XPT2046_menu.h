/*
 * z_touch_XPT2046_menu.h
 *
 *  Created on: 13 giu 2022
 *      Author: mauro
 */

#ifndef INC_Z_TOUCH_XPT2046_MENU_H_
#define INC_Z_TOUCH_XPT2046_MENU_H_



// Menu data definitions and declarations


typedef struct
{
	uint16_t X;
	uint16_t Y;
	uint16_t W;
	uint16_t H;
	char Desc[20];
}MenuRow;


void InitMenu();
void DrawMenu(MenuRow *menu,uint8_t menusize);
uint8_t CheckMenu(MenuRow *menu,uint8_t menusize);
void RunMenu2();
void RunMenu1();







#endif /* INC_Z_TOUCH_XPT2046_MENU_H_ */

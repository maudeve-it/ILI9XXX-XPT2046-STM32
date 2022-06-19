/*
 *
 *  Created on: 2 giu 2022
 *      Author: mauro
 *
 *  licensing: https://github.com/maudeve-it/ILI9486-STM32/blob/main/LICENSE
 *
 */


#ifndef __Z_DISPL_ILI9488_H
#define __Z_DISPL_ILI9488_H

/******************    STEP 0    *****************
 * which display are you usng?
 *************************************************/

//#define ILI9341
#define ILI9488



/******************    STEP 1    ******************
 **************** PORT PARAMETERS *****************
 ** properly set the below the 2 defines to address
 ********  the SPI port defined on CubeMX *********/

#define DISPL_SPI_PORT 	hspi1
#define DISPL_SPI 		SPI1



/*****************     STEP 2      ****************
 ************* SPI COMMUNICATION MODE *************
 ** enable SPI mode wanted uncommenting ONE row ***
 ******* Same as configuration set on CubeMX ******/

//#define DISPLAY_SPI_POLLING_MODE
//#define DISPLAY_SPI_INTERRUPT_MODE
#define DISPLAY_SPI_DMA_MODE // (mixed: polling/DMA, see below)

/***************************************
 * set data length lower limit to
 * enable DMA instead to direct polling
 ***************************************/
#define DISPL_DMA_CUTOFF 	10    // used only in DMA_MODE



/*****************     STEP 3      *****************
 ***************** Backlight timer *****************
 * if you want dimming backlight UNCOMMENT the
 * DISPLAY_DIMMING_MODE below define and properly
 * set other defines.
 *
 * Using backlight as a switch (only on/off) leave
 * DISPLAY_DIMMING_MODE commented
 *
 * if DIMMING:
 * On CubeMX set DISPL_LED pin as a timer PWM pin.
 * Timer COUNTER PERIOD (ARR) defines dimming light steps:
 * keep it low value - e.g. 10 - if dimming with buttons,
 * use higher value - e.g. 100 - if dimming with encoder, ...
 * Avoiding display flickering timer PRESCALER should
 * let timer clock to be higher than COUNTER PERIOD * 100 Hz.
 * Set all other defines below */
#define DISPLAY_DIMMING_MODE						// uncomment this define to enable dimming function otherwise there is an on/off switching function
#define BKLIT_TIMER 				TIM3			//timer used (PWMming DISPL_LED pin)
#define BKLIT_T 					htim3			//timer used
#define BKLIT_CHANNEL				TIM_CHANNEL_2	//channel used
#define BKLIT_CCR					CCR2			//Capture-compare register
#define BKLIT_STBY_LEVEL 			1				//Display backlight level when in stand-by (levels are CNT values)
#define BKLIT_INIT_LEVEL 			5				//Display backlight level on startup


/*****************     STEP 4      *****************
 ************* frame buffer DEFINITION *************
 * BUFLEVEL defines buffer size:
 * buffer size is 2^BUFLEVEL, 2 means 4 bytes buffer,
 * 9 means 512 bytes.
 * IT MUST BE 10 OR MORE:
 * -	10 needed for 1 char in Font24 size
***************************************************/
#define BUFLEVEL 11
#define SIZEBUF 1<<BUFLEVEL


/********     STEP 5      ********
 * here you have to define the prescaler value to assign
 * SPI port when changing from DISPLAY to TOUCH and viceversa
 *
 *********************************/
#define DISPL_PRESCALER SPI_BAUDRATEPRESCALER_2
#define TOUCH_PRESCALER SPI_BAUDRATEPRESCALER_64


/********     STEP 6      ********
 ******   display size      ******
 *********************************/
#ifdef ILI9488
#define DISPL_WIDTH  320		// 0 orientation
#define DISPL_HEIGHT 480		// 0 orientation
#endif

#ifdef ILI9341
#define DISPL_WIDTH  240		// 0 orientation
#define DISPL_HEIGHT 320		// 0 orientation
#endif


/********     STEP 7      ********
 ******   color depth      *******
 * choose one of the two color
 * depth available RGB565 and RGB666
 *********************************/
#ifdef ILI9341
#define RGB565
#endif
#ifdef ILI9488
#define RGB666
#endif



/**** End Of Configuration Parameters ********************************************************/

#include <string.h>

typedef enum {
	Displ_Orientat_0,
	Displ_Orientat_90,
	Displ_Orientat_180,
	Displ_Orientat_270
} Displ_Orientat_e;


#define SPI_COMMAND GPIO_PIN_RESET  //DISPL_DC_Pin level sending commands
#define SPI_DATA GPIO_PIN_SET		//DISPL_DC_Pin level sending data


/**********************************************************
 * macro changing SPI baudrate prescaler
 * (used before any changes between display<->touch devices
 **********************************************************/
#define SET_SPI_BAUDRATE(_P_SCALER_) 	DISPL_SPI->CR1 &= (uint16_t) ~SPI_CR1_BR_Msk; \
										DISPL_SPI->CR1 |= _P_SCALER_


/***************************
 * Color names
 ***************************/
#define	RED			0xF800
#define	GREEN		0x07E0
#define	BLUE		0x001F
#define YELLOW		0xFFE0
#define MAGENTA		0xF81F
#define ORANGE		0xFD00
#define CYAN		0x07FF
#define	D_RED 		0xC000
#define	D_GREEN		0x0600
#define	D_BLUE		0x0018
#define D_YELLOW	0xC600
#define D_MAGENTA	0xC018
#define D_ORANGE	0xC300
#define D_CYAN		0x0618
#define	DD_RED		0x8000
#define	DD_GREEN	0x0400
#define DD_BLUE		0x0010
#define DD_YELLOW	0x8400
#define DD_MAGENTA	0x8020
#define DD_ORANGE	0x8200
#define DD_CYAN		0x0410
#define WHITE		0xFFFF
#define D_WHITE		0xC618
#define DD_WHITE	0x8410
#define DDD_WHITE	0x4208
#define DDDD_WHITE	0x2104
#define	BLACK		0x0000
#define color565(r, g, b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | (((b) & 0xF8) >> 3))


//ILI9488 LCD commands
#define ILI9488_SLEEP_OUT			0x11
#define ILI9488_DISPLAY_ON			0x29
#define ILI9488_PIXEL_FORMAT    	0x3A
#define ILI9488_RGB_INTERFACE   	0xB0
#define ILI9488_MEMWR				0x2C
#define ILI9488_COLUMN_ADDR			0x2A
#define ILI9488_PAGE_ADDR			0x2B

#define ILI9XXX_POWER1				0xC0
#define ILI9XXX_POWER2				0xC1
#define ILI9XXX_VCOM1				0xC5
#define ILI9XXX_VCOM2				0xC7
#define ILI9XXX_POWERA				0xCB
#define ILI9XXX_POWERB				0xCF
#define ILI9XXX_DFC				 	0xB6


#define ILI9488_MEMCONTROL	0x36
#define ILI9488_MADCTL_BGR	0x08
#define ILI9488_MADCTL_MV 	0x20
#define ILI9488_MADCTL_MY 	0x80
#define ILI9488_MADCTL_MX 	0x40




//ILI9341 LCD commands
#define ILI9341_RESET				0x01
#define ILI9341_SLEEP_OUT			0x11
//#define ILI9341_GAMMA				0x26
#define ILI9341_DISPLAY_OFF			0x28
#define ILI9341_DISPLAY_ON			0x29

#define ILI9341_COLUMN_ADDR			0x2A
#define ILI9341_PAGE_ADDR			0x2B
#define ILI9341_GRAM				0x2C
#define ILI9341_TEARING_OFF			0x34
#define ILI9341_TEARING_ON			0x35
#define ILI9341_DISPLAY_INVERSION	0xb4
#define ILI9341_MAC			        0x36
#define ILI9341_PIXEL_FORMAT    	0x3A
#define ILI9341_WDB			    	0x51
#define ILI9341_WCD				    0x53
#define ILI9341_RGB_INTERFACE   	0xB0
#define ILI9341_FRC					0xB1
#define ILI9341_BPC					0xB5
#define ILI9341_DFC				 	0xB6
#define ILI9341_Entry_Mode_Set		0xB7
#define ILI9341_POWER1				0xC0
#define ILI9341_POWER2				0xC1
#define ILI9341_VCOM1				0xC5
#define ILI9341_VCOM2				0xC7
#define ILI9341_POWERA				0xCB
#define ILI9341_POWERB				0xCF
#define ILI9341_PGAMMA				0xE0
#define ILI9341_NGAMMA				0xE1
#define ILI9341_DTCA				0xE8
#define ILI9341_DTCB				0xEA
#define ILI9341_POWER_SEQ			0xED
#define ILI9341_3GAMMA_EN			0xF2
#define ILI9341_INTERFACE			0xF6
#define ILI9341_PRC				   	0xF7
#define ILI9341_VERTICAL_SCROLL 	0x33

#define ILI9341_MEMCONTROL	0x36
#define ILI9341_MADCTL_MY 	0x80
#define ILI9341_MADCTL_MX 	0x40
#define ILI9341_MADCTL_MV 	0x20
#define ILI9341_MADCTL_ML	0x10
#define ILI9341_MADCTL_RGB	0x00
#define ILI9341_MADCTL_BGR	0x08
#define ILI9341_MADCTL_MH	0x04




#define _swap_int16_t(a, b)  { int16_t t = a; a = b; b = t; }


void Displ_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void Displ_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

void Displ_fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void Displ_drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);

void Displ_Border(int16_t x, int16_t y, int16_t w, int16_t h, int16_t t,  uint16_t color);
void Displ_CLS(uint16_t bgcolor);
void Displ_CString(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, const char* str, sFONT font, uint8_t size, uint16_t color, uint16_t bgcolor);
void Displ_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void Displ_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void Displ_Init(Displ_Orientat_e orientation);
void Displ_Line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void Displ_FillArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void Displ_Orientation(Displ_Orientat_e orientation);
void Displ_Pixel(uint16_t x, uint16_t y, uint16_t color);
void Displ_WChar(uint16_t x, uint16_t y, char ch, sFONT font, uint8_t size, uint16_t color, uint16_t bgcolor);
void Displ_WString(uint16_t x, uint16_t y, const char* str, sFONT font, uint8_t size, uint16_t color, uint16_t bgcolor);
void Displ_Transmit(GPIO_PinState DC_Status, uint8_t* data, uint16_t dataSize );
void Displ_WriteCommand(uint8_t cmd);
void Displ_WriteData(uint8_t* buff, size_t buff_size);

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);

uint32_t  Displ_BackLight(uint8_t cmd);




#endif /* __Z_DISPL_ILI9341_H */

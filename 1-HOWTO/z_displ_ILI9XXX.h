/*
 * 	z_displ_ILI9488.h
 *	rel. TouchGFX.1.30
 *
 *  Created on: 05 giu 2023
 *      Author: mauro
 *
 *  licensing: https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/blob/c097f0e7d569845c1cf98e8d930f2224e427fd54/LICENSE
 *
 *	Installing and using this library follow instruction on: https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32
 *
 *  These are the init instruction to put in you main() USER CODE BEGIN 2
 *
 *  (if Direct Handling)
 *  Displ_Init(Displ_Orientat_0);			// initialize display controller - set orientation parameter as per your needs
 *  Displ_CLS(BLACK);						// clear the screen - BLACK or any other color you prefer
 *  Displ_BackLight('I');  					// initialize backlight
 *
 *  (if using TouchGFX - button mode)
 *  Displ_Init(Displ_Orientat_0);			// initialize display controller - set orientation parameter as per TouchGFX setup
 * 	touchgfxSignalVSync();					// ask display syncronization
 *  Displ_BackLight('I');  					// initialize backlight
 *
 *  (if using TouchGFX - full mode)
 *  Displ_Init(Displ_Orientat_0);			// initialize display controller - set orientation parameter as per TouchGFX setup
 *  Displ_BackLight('I');  					// initialize backlight
 *  HAL_TIM_Base_Start_IT(&TGFX_T);			// start TouchGFX tick timer
 *
 *  see also z_touch_XPT2046.h
 *
 */


#ifndef __Z_DISPL_ILI9XXX_H
#define __Z_DISPL_ILI9XXX_H


/*||||||||||| USER/PROJECT PARAMETERS |||||||||||*/

/*****************     STEP 1      *****************
 ************ Enable TouchGFX interface ************
 * uncommenting the below #define to enable
 * functions interfacing TouchGFX
 ***************************************************/
#define DISPLAY_USING_TOUCHGFX


/******************    STEP 2    *****************
 * which display are you using?
 *************************************************/
//#define ILI9341
//#define ILI9488_V1
#define ILI9488_V2


/******************    STEP 3    ******************
 **************** PORT PARAMETERS *****************
 ** properly set the below th 2 defines to address
 ********  the SPI port defined on CubeMX *********
 **************************************************/
#define DISPL_SPI_PORT 	hspi2
#define DISPL_SPI 		SPI2


/******************    STEP 4     ******************
 ***************** SPI PORT SPEED  *****************
 * define HERE the prescaler value to assign SPI port 
 * when transferring data to/from DISPLAY or TOUCH
 * Keep in mind that Touch SPI Baudrate should be no more than 1 Mbps
 ***************************************************/
#define DISPL_PRESCALER SPI_BAUDRATEPRESCALER_4     //prescaler assigned to display SPI port
#define TOUCH_PRESCALER SPI_BAUDRATEPRESCALER_256	//prescaler assigned to touch device SPI port


/*****************     STEP 5      *****************
 ************* SPI COMMUNICATION MODE **************
 *** enable SPI mode want, uncommenting ONE row ****
 **** (Setup the same configuration on CubeMX) *****
 ***************************************************/
//#define DISPLAY_SPI_POLLING_MODE
//#define DISPLAY_SPI_INTERRUPT_MODE
#define DISPLAY_SPI_DMA_MODE // (mixed: polling/DMA, see below)


/*****************     STEP 6      *****************
 ***************** Backlight timer *****************
 * if you want dimming backlight UNCOMMENT the
 * DISPLAY_DIMMING_MODE below define and properly
 * set other defines.
 * Using backlight as a switch (only on/off) leave
 * DISPLAY_DIMMING_MODE commented
 * if DIMMING:
 * On CubeMX set DISPL_LED pin as a timer PWM pin.
 * Timer COUNTER PERIOD (ARR) defines dimming light steps:
 * keep it low value - e.g. 10 - if dimming with buttons,
 * use higher value - e.g. 100 - if dimming with encoder, ...
 * Avoiding display flickering timer PRESCALER should
 * let timer clock to be higher than COUNTER PERIOD * 100 Hz.
 * Set all other defines below 
 ***************************************************/
#define DISPLAY_DIMMING_MODE						// uncomment this define to enable dimming function otherwise there is an on/off switching function
#define BKLIT_TIMER 				TIM2			//timer used (PWMming DISPL_LED pin)
#define BKLIT_T 					htim2			//timer used
#define BKLIT_CHANNEL				TIM_CHANNEL_1	//channel used
#define BKLIT_CCR					CCR1			//Capture-compare register used (same number as channel)
#define BKLIT_STBY_LEVEL 			50				//Display backlight level when in stand-by (levels are CNT values)
#define BKLIT_INIT_LEVEL 			100				//Display backlight level on startup


/*****************     STEP 7      *****************
 ***************** TouchGFX Time base timer *****************
 * If using library in TouchGFX-full-mode
 * (see GitHub page indicated on top for details)
 * you have to set #define DELAY_TO_KEY_REPEAT -1
 * in "z_touch_XPT2046.h" and setup a timer as a 
 * time base for TouchGFX. 
 * It has to be set to generate a
 * HAL_TIM_PeriodElapsedCallback 60 times per second
 * That timer has to be assigned to the below macros.
 * if not in TouchGFX-full-mode: assign macros to 
 * an unused timer
 ***************************************************/
#define TGFX_TIMER			TIM3
#define TGFX_T				htim3


/*****************     STEP 8      *****************
 ************* frame buffer DEFINITION *************
 * IF NO TOUCHGFX: 
 * BUFLEVEL defines size of each one of the 2 SPI 
 * buffers: buffer size is 2^BUFLEVEL so 2 means 
 * 4 bytes buffer and 10 means 1 kbyte (each).
 * It must be not below 10!
 * If TOUCHGFX:
 * buffers are not used if display handles RGB565. 
 * If display uses RGB666 one buffer will be used for 
 * color format translation. In this case set
 * BUFLEVEL following this table:
 * TouchGFX buffers>10KB need BUFLEVEL 15
 * TouchGFX buffers>5KB  need BUFLEVEL 14
 * TouchGFX buffers>2700bytes need BUFLEVEL 13
 * TouchGFX buffers>1300bytes need BUFLEVEL 12
***************************************************/
#define BUFLEVEL 13

/*|||||||| END OF USER/PROJECT PARAMETERS ||||||||*/


/*|||||||||||||| DEVICE PARAMETERS |||||||||||||||||*/
/* you shouldn't need to change anything here after */

#ifdef ILI9488_V1
#define ILI9488
#endif
#ifdef ILI9488_V2
#define ILI9488
#endif


/***************   color depth      ****************
 *** choose one of the two color depth available *** 
 ***** to use on the display RGB565 and RGB666 *****
 ***************************************************/
#ifdef ILI9341
#define Z_RGB565
#endif
#ifdef ILI9488_V1
#define Z_RGB666
#endif
#ifdef ILI9488_V2
#define Z_RGB565
#endif


/***************   display size      ***************
 ***************************************************/
#ifdef ILI9341
#define DISPL_WIDTH  240		// 0 orientation
#define DISPL_HEIGHT 320		// 0 orientation
#endif
#ifdef ILI9488
#define DISPL_WIDTH  320		// 0 orientation
#define DISPL_HEIGHT 480		// 0 orientation
#endif


/************* from POLLING to DMA *****************
 *** below DISPL_DMA_CUTOFF data size, transfer ****
 ****** will be polling, even if DMA enabled *******
 ***************************************************/
#define DISPL_DMA_CUTOFF 	20    // (bytes) used only in DMA_MODE

/*||||||||||| END OF DEVICE PARAMETERS ||||||||||||*/


#include <string.h>

typedef enum {
	Displ_Orientat_0,
	Displ_Orientat_90,
	Displ_Orientat_180,
	Displ_Orientat_270
} Displ_Orientat_e;


#define SPI_COMMAND 	GPIO_PIN_RESET  	//DISPL_DC_Pin level sending commands
#define SPI_DATA 		GPIO_PIN_SET		//DISPL_DC_Pin level sending data

// set the buffers size as per BUFLEVEL and DISPLAY_USING_TOUCHGFX
// (if using TouchGFX, don't buffers from this library)
#define SIZEBUF (1<<BUFLEVEL)


/*******************************
 * Color names
 *******************************/
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


/**********************************
/ ILI9XXX LCD family commands
 **********************************/
#define ILI9XXX_SLEEP_OUT			0x11	//wake up display
#define ILI9XXX_DISPLAY_ON			0x29	// enable display
#define ILI9XXX_PIXEL_FORMAT    	0x3A	// RGB565/RGB666/...
#define ILI9XXX_RGB_INTERFACE   	0xB0	// type of communication (full duplex, half, etc.)
#define ILI9XXX_MEMWR				0x2C	// writes into memory
#define ILI9XXX_COLUMN_ADDR			0x2A	// set area display to write into
#define ILI9XXX_PAGE_ADDR			0x2B	// set area display to write into
#define ILI9XXX_MADCTL				0x36	// order followed writing into memory (-> screen orientation)
#define ILI9XXX_MADCTL_0DEG 		0X88	// parameter of MADCTL command
#define ILI9XXX_MADCTL_90DEG 		0xE8	// parameter of MADCTL command
#define ILI9XXX_MADCTL_180DEG 		0x48	// parameter of MADCTL command
#define ILI9XXX_MADCTL_270DEG 		0x28	// parameter of MADCTL command

#define ILI9XXX_INIT_SHORT_DELAY	5		// Hal_Delay parameter
#define ILI9XXX_INIT_LONG_DELAY		150		// Hal_Delay parameter

#define ILI9XXX_POWER0				0xC0
#define ILI9XXX_POWER1				0xC1
#define ILI9488_POWER2				0xC2
#define ILI9341_POWERA				0xCB
#define ILI9341_POWERB				0xCF

/**********************************************************
 * macro setting SPI baudrate prescaler
 **********************************************************/
#define SET_DISPL_SPI_BAUDRATE			DISPL_SPI->CR1 &= (uint16_t) ~SPI_CR1_BR_Msk; \
										DISPL_SPI->CR1 |= DISPL_PRESCALER

#define SET_TOUCH_SPI_BAUDRATE			TOUCH_SPI->CR1 &= (uint16_t) ~SPI_CR1_BR_Msk; \
										TOUCH_SPI->CR1 |= TOUCH_PRESCALER
/**********************************************************/


#define _swap_int16_t(a, b)  { int16_t t = a; a = b; b = t; }


#ifndef DISPLAY_USING_TOUCHGFX
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
#endif /* ! DISPLAY_USING_TOUCHGFX */
void Displ_FillArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void Displ_Orientation(Displ_Orientat_e orientation);
void Displ_Init(Displ_Orientat_e orientation);

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);

uint32_t  Displ_BackLight(uint8_t cmd);


#ifdef DISPLAY_USING_TOUCHGFX
int touchgfxDisplayDriverTransmitActive();
void touchgfxDisplayDriverTransmitBlock(const uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
extern void DisplayDriver_TransferCompleteCallback();
extern void touchgfxSignalVSync(void);
#endif /* DISPLAY_USING_TOUCHGFX */

#endif /* __Z_DISPL_ILI9XXX_H */

/*
 * 	z_displ_ILI94XX.c
 * 	rel. TouchGFX.1.20
 *
 *  Created on: Dec 27, 2022
 *      Author: mauro
 *
 *  licensing: https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/blob/c097f0e7d569845c1cf98e8d930f2224e427fd54/LICENSE
 *
 *	To install and use this library follow instruction on: https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32
 *
 */


#include "main.h"

extern SPI_HandleTypeDef DISPL_SPI_PORT;

#ifdef DISPLAY_DIMMING_MODE
extern TIM_HandleTypeDef BKLIT_T;
#endif

extern volatile uint8_t Touch_PenDown;				// set to 1 by pendown interrupt callback, reset to 0 by sw

Displ_Orientat_e current_orientation;				// it records the active display orientation. Set by Displ_Orientation
volatile uint8_t Displ_SpiAvailable=1;  			// 0 if SPI is busy or 1 if it is free (transm cplt)

int16_t _width;       								///< (oriented) display width
int16_t _height;      								///< (oriented) display height

// if using TouchGFX buffer are not used (size set to 2 bytes for software convenience)
// unless using ILI9488V1.0 (RGB666) which needs dispBuffer1 for color format conversion
// if not using TouchGFX double buffering is needed
#ifdef DISPLAY_USING_TOUCHGFX
static uint8_t dispBuffer2[2];
#ifdef Z_RGB666
static uint8_t dispBuffer1[SIZEBUF];
#else
static uint8_t dispBuffer1[2];
#endif // Z_RGB666
#else
static uint8_t dispBuffer1[SIZEBUF];
static uint8_t dispBuffer2[SIZEBUF];
#endif //DISPLAY_USING_TOUCHGFX
static uint8_t *dispBuffer=dispBuffer1;




/******************************************
 * @brief	enable display, disabling touch
 * 			device selected if CS low
 ******************************************/
void Displ_Select(void) {
	if (TOUCH_SPI==DISPL_SPI){														// if SPI port shared (display <-> touch)
		if (HAL_GPIO_ReadPin(DISPL_CS_GPIO_Port, DISPL_CS_Pin)) {					// if display not yet selected
			HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET); 		// unselect touch
			SET_SPI_BAUDRATE(DISPL_PRESCALER);   									//change SPI port speed as per display needs
			HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_RESET);	// select display
		}
	}
}




/**************************
 * @BRIEF	engages SPI port communicating with displayDC_Status
 * 			depending on the macro definition makes transmission in Polling/Interrupt/DMA mode
 * @PARAM	DC_Status 	indicates if sending command or data
 * 			data		buffer data to send
 * 			dataSize	number of bytes in "data" to be sent
 * 			isTouchGFXBuffer 1 only when called by touchgfxDisplayDriverTransmitBlock (for byte endian conversion). All other cases 0
 **************************/
void Displ_Transmit(GPIO_PinState DC_Status, uint8_t* data, uint16_t dataSize, uint8_t isTouchGFXBuffer ){

	while (!Displ_SpiAvailable) {};  // waiting for a free SPI port. Flag is set to 1 by transmission-complete interrupt callback

	HAL_GPIO_WritePin(DISPL_DC_GPIO_Port, DISPL_DC_Pin, DC_Status);
	Displ_Select();

	if (isTouchGFXBuffer){
#ifdef Z_RGB565
//if color format is RGB565 just swap even and odd bytes correcting endianess for ILI driver
		uint32_t *limit=(uint32_t*)(data+dataSize);
		for (uint32_t *data32=(uint32_t*)data; data32<limit; data32++) {
			*data32=__REV16(*data32);
		}
#else
//if display color format is RGB666: convert RGB565 received by TouchGFX and swap bytes

		uint8_t *buf8Pos=dispBuffer1; 							//using a local pointer

		uint16_t *limit=(uint16_t*)(data+dataSize);
		for (uint16_t *data16=(uint16_t*)data; (data16<limit) & ((buf8Pos-dispBuffer1)<(SIZEBUF-3)); data16++) {

			*(buf8Pos++)=((*data16 & 0xF800)>>8);  // R color
			*(buf8Pos++)=((*data16 & 0x07E0)>>3);  // G color
			*(buf8Pos++)=((*data16 & 0x001F)<<3);  // B color
		}

		data=dispBuffer1; 				//data (pointer to data to transfer via SPI) has point to converted buffer
		dataSize=(buf8Pos-dispBuffer1);	//and dataSize has contain the size of the converted buffer

#endif //Z_RGB565
	}


#ifdef DISPLAY_SPI_INTERRUPT_MODE
		Displ_SpiAvailable=0;
		HAL_SPI_Transmit_IT(&DISPL_SPI_PORT , data, dataSize);
#else
#ifdef DISPLAY_SPI_DMA_MODE
		if (dataSize<DISPL_DMA_CUTOFF) {
#endif //DISPLAY_SPI_DMA_MODE
			Displ_SpiAvailable=0;
			HAL_SPI_Transmit(&DISPL_SPI_PORT , data, dataSize, HAL_MAX_DELAY);
			Displ_SpiAvailable=1;

#ifdef DISPLAY_USING_TOUCHGFX
			if (isTouchGFXBuffer){
				DisplayDriver_TransferCompleteCallback();
			}
#endif //DISPLAY_USING_TOUCHGFX
#ifdef DISPLAY_SPI_DMA_MODE
		} else {
			Displ_SpiAvailable=0;
			HAL_SPI_Transmit_DMA(&DISPL_SPI_PORT , data, dataSize);
		}
#endif //DISPLAY_SPI_DMA_MODE
#endif //DISPLAY_SPI_INTERRUPT_MODE
	}




/**********************************
 * @BRIEF	transmit a byte in a SPI_COMMAND format
 **********************************/
void Displ_WriteCommand(uint8_t cmd){
	Displ_Transmit(SPI_COMMAND, &cmd, sizeof(cmd),0);
}




/**********************************
 * @BRIEF	transmit a set of data in a SPI_DATA format
 * @PARAM 	data		buffer data to send
 * 			dataSize	number of bytes in "data" to be sent
 * 			isTouchGFXBuffer 1 only when called by touchgfxDisplayDriverTransmitBlock (for byte endian conversion). All other cases 0
 **********************************/
void Displ_WriteData(uint8_t* buff, size_t buff_size, uint8_t isTouchGFXBuffer){
	if (buff_size==0) return;
	Displ_Transmit(SPI_DATA, buff, buff_size, isTouchGFXBuffer);
}





/**********************************
 * @brief	ILIXXX initialization sequence
 **********************************/
void ILI9XXX_Init(){
	Displ_Select();

	HAL_GPIO_WritePin(DISPL_RST_GPIO_Port, DISPL_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(DISPL_RST_GPIO_Port, DISPL_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(150);

/******************************************
 * below lines shlould empower brightness
 * and quality with a higher power
 * consumption. I haven't seen meaningful
 * differences on both displays: REMOVED
 * ****************************************
	uint8_t data[10];

		Displ_WriteCommand(ILI9XXX_POWER0);
	#ifdef ILI9341
		data[0]=0x3F; //default 0x21, 3.3V=0x09, 5V=2B
		Displ_WriteData(data,1);
	#endif
	#ifdef ILI9488
		data[0]=0x1F; //default 0E, 3.3V=0x09, 5V=17
		data[1]=0x1F; //default 0E, 5V=17
		Displ_WriteData(data,2);
	#endif

		Displ_WriteCommand(ILI9XXX_POWER1);
	#ifdef ILI9341
		data[0]=0x00; //default 0x00
	#endif
	#ifdef ILI9488
		data[0]=0x40; //default 0x44
	#endif
		Displ_WriteData(data,1);

	#ifdef ILI9488
		Displ_WriteCommand(ILI9488_POWER2);
		data[0]=0x44; //default 0x44
		Displ_WriteData(data,1);
	#endif

	#ifdef ILI9341
		Displ_WriteCommand(ILI9341_POWERA);
		data[0]=0x39; //fixed
		data[1]=0x2C; //fixed
		data[2]=0x00; //fixed
		data[3]=0x35; //default 0x34
		data[4]=0x00; //default 0x02
		Displ_WriteData(data,5);
		Displ_WriteCommand(ILI9341_POWERB);
		data[0]=0x00; //fixed
		data[1]=0x99; //default 0x81
		data[2]=0x30; //default 0x30
		Displ_WriteData(data,3);
	#endif
*/


	Displ_WriteCommand(ILI9XXX_PIXEL_FORMAT);
#ifdef Z_RGB666
	Displ_WriteData((uint8_t *)"\x66",1,0);		// RGB666
#endif
#ifdef Z_RGB565
	Displ_WriteData((uint8_t *)"\x55",1,0);		// RGB565
#endif
	Displ_WriteCommand(ILI9XXX_RGB_INTERFACE);
	Displ_WriteData((uint8_t *)"\x80",1,0);        // disable MISO pin

	Displ_WriteCommand(ILI9XXX_RGB_INTERFACE);
	Displ_WriteData((uint8_t *)"\x80",1,0);        // disable MISO pin

	Displ_WriteCommand(ILI9XXX_SLEEP_OUT);
	HAL_Delay(120);

	Displ_WriteCommand(ILI9XXX_DISPLAY_ON);
	HAL_Delay(5);

}



/**********************************************
 * @brief	defines the display area involved
 * 			in a writing operation and set
 * 			display ready to receive pixel
 * 			information
 * @param  x1,y1,x2,y2 top left and bottom
 * 					   right corner of the area
 * 					   to write
 **********************************************/
void Displ_SetAddressWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	static uint8_t data[4];

	((uint32_t *)data)[0]=(((x2 & 0xFF)<<24) | ((x2 & 0xFF00)<<8) | ((x1 & 0xFF)<<8) | ((x1 & 0xFF00)>>8) );
	Displ_WriteCommand(ILI9XXX_COLUMN_ADDR);
	Displ_WriteData(data, 4,0);

	((uint32_t *)data)[0]=(((y2 & 0xFF)<<24) | ((y2 & 0xFF00)<<8) | ((y1 & 0xFF)<<8) | ((y1 & 0xFF00)>>8) );
	Displ_WriteCommand(ILI9XXX_PAGE_ADDR);
	Displ_WriteData(data, 4,0);
	Displ_WriteCommand(ILI9XXX_MEMWR);
}




/*****************************************************
 * @brief				first display initialization.
 * @param	orientation	display orientation
 *****************************************************/
void Displ_Init(Displ_Orientat_e orientation){
	if (TOUCH_SPI==DISPL_SPI){													// if touch and display share the same SPI port
		HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_SET); 		// unselect display (will be selected at writing time)
		HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET);		// unselect touch (will be selected at writing time)
	} else {																	// otherwise leave both port permanently selected
		HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_RESET); 	// select display
		HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_RESET);	// select touch
	}
	ILI9XXX_Init();
	Displ_Orientation(orientation);
}







/**********************************************
 * @brief		set orientation of the display
 * @param  	m	orientation
 **********************************************/
void Displ_Orientation(Displ_Orientat_e orientation){
	static uint8_t data[1];
	switch(orientation) {
		case Displ_Orientat_0:
			data[0]=ILI9XXX_MADCTL_0DEG;
			_height = DISPL_HEIGHT;
			_width = DISPL_WIDTH;
			break;
		case Displ_Orientat_90:
			data[0]=ILI9XXX_MADCTL_90DEG;
			_height = DISPL_WIDTH;
			_width = DISPL_HEIGHT;
			break;
		case Displ_Orientat_180:
			data[0]=ILI9XXX_MADCTL_180DEG;
			_height = DISPL_HEIGHT;
			_width = DISPL_WIDTH;
			break;
		case Displ_Orientat_270:
			data[0]=ILI9XXX_MADCTL_270DEG;
			_height = DISPL_WIDTH;
			_width = DISPL_HEIGHT;
			break;
	}
	Displ_WriteCommand(ILI9XXX_MADCTL);
	Displ_WriteData(data,1,0);
	current_orientation = orientation;  //stores active orientation into a global variable for touch routines
}




void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi){
	if (hspi->Instance==DISPL_SPI) {
		Displ_SpiAvailable=1;
	}
}






void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
	if (hspi->Instance==DISPL_SPI) {
		Displ_SpiAvailable=1;
//		Touch_PenDown=0;    //reset touch interrupt flag: writing onto display will trigger the display interrupt pin

#ifdef DISPLAY_USING_TOUCHGFX
		DisplayDriver_TransferCompleteCallback();
#endif
	}
}





/*****************************
 * @brief	fill a rectangle with a color
 * @param	x, y	top left corner of the rectangle
 * 			w, h 	width and height of the rectangle
 ******************************/
void Displ_FillArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
/* four steps:
 * -	define area size to file and data size to transfer
 * -	setup data buffer to transfer
 * -	transfer data to display
 * -	swap buffers
 */

	uint32_t k,x1,y1,area,times;

	if((x >= _width) || (y >= _height) || (w == 0) || (h == 0)) return;//

	x1=x + w - 1;
	if (x1 > _width) {
		x1=_width;
	}

	y1=y + h - 1;
	if (y1 > _height) {
		y1=_height;
	}


// SETUP DISPLAY DATA BUFFER TO TRANSFER
#ifdef Z_RGB565 // setting up dispBuffer in RGB565 format

	uint32_t data32;

	data32=(color>>8) | (color<<8) | (color<<24); 	// supposing color is 0xABCD, data32 becomes 0xCDABCDAB - set a 32 bit variable with swapped endians
	area=((y1-y+1)*(x1-x+1)); 						// area to fill in 16bit pixels
	uint32_t *buf32Pos=(uint32_t *)dispBuffer; 		//dispBuffer defined in bytes, buf32Pos access it as 32 bit words
	if (area<(SIZEBUF>>1)) 							// if area is smaller than dispBuffer
		times=(area>>1)+1; 							// number of times data32 has to be loaded into buffer
	else
		times=(SIZEBUF>>2);  						// dispBuffer size as 32bit-words
	for (k = 0; k < times; k++)
		*(buf32Pos++)=data32; 						// loads buffer moving 32bit-words

#endif
#ifdef Z_RGB666 // setting up dispBuffer in RGB666 format
	uint32_t datasize;

	uint8_t Rbyte=(color & 0xF800)>>8;
	uint8_t Gbyte=(color & 0x07E0)>>3;
	uint8_t Bbyte=(color & 0x001F)<<3;

	area=(((y1-y+1)*(x1-x+1))*3); 		// area to fill in bytes (3 bytes per pixel)
	uint8_t *buf8Pos=dispBuffer; 		//using a local pointer: changing values next

	datasize = (area<(SIZEBUF-3) ? area : (SIZEBUF-3));  //as buf8Pos receives 3 bytes each cycle we must be sure that SIZEBUF will be not overridden in the next loop

	k=0;
	while ((buf8Pos-dispBuffer)<=datasize){
		*(buf8Pos++)=Rbyte;
		*(buf8Pos++)=Gbyte;
		*(buf8Pos++)=Bbyte;
	}
	datasize=(buf8Pos-dispBuffer);
#endif

//START WRITING TO DISPLAY
	Displ_SetAddressWindow(x, y, x1, y1);

#ifdef Z_RGB565 // transferring RGB666 format dispBuffer
	times=(area>>(BUFLEVEL-1));  					//how many times buffer must be sent via SPI. It is (BUFFLEVEL-1) because area is 16-bit while dispBuffer is 8-bit
	for  (k=0;k<times;k++) {
		Displ_WriteData(dispBuffer,SIZEBUF,0);
	}
	Displ_WriteData(dispBuffer,(area<<1)-(times<<BUFLEVEL),0);
#endif
#ifdef Z_RGB666 // transferring RGB666 format dispBuffer
	times=(area/datasize);  					//how many times buffer must be sent via SPI.
	for  (k=0;k<times;k++) {
		Displ_WriteData(dispBuffer,datasize,0);
	}
	Displ_WriteData(dispBuffer,(area-times*datasize),0);      //transfer last data frame
#endif

//BUFFER SWAP
	dispBuffer = (dispBuffer==dispBuffer1 ? dispBuffer2 : dispBuffer1); // swapping buffer

}




#ifndef DISPLAY_USING_TOUCHGFX

/*****************************************
 * WARNING: non tested, never used
 *****************************************/
void ILI9488_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data, uint32_t size){
	Displ_SetAddressWindow(x, y, w+x-1, h+y-1);
	Displ_WriteData(data,size,0);
}




/***********************
 * @brief	print a single pixel
 * @params	x, y	pixel position on display
 * 			color	... to be printed
 ***********************/
void Displ_Pixel(uint16_t x, uint16_t y, uint16_t color) {
    if((x >= _width) || (y >= _height))
        return;
    Displ_FillArea(x, y, 1, 1, color);

}




/*****************
 * @brief	clear display with a color.
 * @param	bgcolor
 *****************/
void Displ_CLS(uint16_t bgcolor){
	Displ_FillArea(0, 0, _width, _height, bgcolor);
}





void Displ_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

//    writePixel(x0  , y0+r, color);
    Displ_Pixel(x0  , y0+r, color);
    Displ_Pixel(x0  , y0-r, color);
    Displ_Pixel(x0+r, y0  , color);
    Displ_Pixel(x0-r, y0  , color);

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        Displ_Pixel(x0 + x, y0 + y, color);
        Displ_Pixel(x0 - x, y0 + y, color);
        Displ_Pixel(x0 + x, y0 - y, color);
        Displ_Pixel(x0 - x, y0 - y, color);
        Displ_Pixel(x0 + y, y0 + x, color);
        Displ_Pixel(x0 - y, y0 + x, color);
        Displ_Pixel(x0 + y, y0 - x, color);
        Displ_Pixel(x0 - y, y0 - x, color);
    }
}







void drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if (cornername & 0x4) {
            Displ_Pixel(x0 + x, y0 + y, color);
            Displ_Pixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
        	Displ_Pixel(x0 + x, y0 - y, color);
        	Displ_Pixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
        	Displ_Pixel(x0 - y, y0 + x, color);
        	Displ_Pixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
        	Displ_Pixel(x0 - y, y0 - x, color);
        	Displ_Pixel(x0 - x, y0 - y, color);
        }
    }
}









void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color)
{
	int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
    	 Displ_Line(x0+x, y0-y, x0+x, y0+y+1+delta, color);
    	 Displ_Line(x0+y, y0-x,x0+y, y0+x+1+delta, color);
    }
    if (cornername & 0x2) {
      Displ_Line(x0-x, y0-y, x0-x, y0+y+1+delta, color);
      Displ_Line(x0-y, y0-x, x0-y, y0+x+1+delta, color);
    }
  }
}






void Displ_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    Displ_Line(x0, y0-r, x0, y0+r, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}






/************************************************************************
 * @brief	draws a line from "x0","y0" to "x1","y1" of the given "color"
 ************************************************************************/
void Displ_Line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
	int16_t l,x,steep,ystep,err,dx, dy;

    if (x0==x1){  // fast solve vertical lines
    	if (y1>y0){
    		Displ_FillArea(x0, y0, 1, y1-y0+1, color);
    	}
    	else {
    		Displ_FillArea(x0, y1, 1, y0-y1+1, color);
    	}
    	return;
    }
    if (y0==y1){ // fast solve horizontal lines
    	if (x1>x0)
    		Displ_FillArea(x0, y0, x1-x0+1, 1, color);
    	else
    		Displ_FillArea(x1, y1, x0-x1+1, 1, color);
    	return;
    }

    steep = (y1>y0 ? y1-y0 : y0-y1) > (x1>x0 ? x1-x0 : x0-x1);

    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    dx = x1 - x0;
    err = dx >> 1;
    if (y0 < y1) {
        dy = y1-y0;
        ystep =  1 ;
    } else {
        dy = y0-y1;
        ystep =  -1 ;
    }

    l=00;
    for (x=x0; x<=x1; x++) {
    	l++;
        err -= dy;
        if (err < 0) {
        	if (steep) {
        		Displ_FillArea(y0, x0, 1, l, color);
            } else {
            	Displ_FillArea(x0, y0, l, 1, color);
            }
            y0 += ystep;
            l=0;
            x0=x+1;
            err += dx;
        }
    }
    if (l!=0){
    	if (steep) {
    		Displ_FillArea(y0, x0, 1, l-1, color);
    	} else {
    		Displ_FillArea(x0, y0, l-1,1, color);
    	}
    }
}










/***********************
 * @brief	print an empty rectangle of a given thickness
 * @params	x, y	top left corner
 * 			w, h	width and height
 * 			t		border thickness
 * 			color	border color, inner part unchanged
 ***********************/
void Displ_Border(int16_t x, int16_t y, int16_t w, int16_t h, int16_t t,  uint16_t color){
	Displ_FillArea(x, y, w, t, color);
	Displ_FillArea(x, y+h-t, w, t, color);
	Displ_FillArea(x, y, t, h, color);
	Displ_FillArea(x+w-t, y, t, h, color);
}





void Displ_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
    Displ_Line(x0, y0, x1, y1, color);
    Displ_Line(x1, y1, x2, y2, color);
    Displ_Line(x2, y2, x0, y0, color);
}







void Displ_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{

    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }
    if (y1 > y2) {
        _swap_int16_t(y2, y1); _swap_int16_t(x2, x1);
    }
    if (y0 > y1) {
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }

    if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if(x1 < a)      a = x1;
        else if(x1 > b) b = x1;
        if(x2 < a)      a = x2;
        else if(x2 > b) b = x2;
//        drawFastHLine(a, y0, b-a+1, color);
        Displ_Line(a, y0, b, y0, color);
        return;
    }

    int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
    int32_t
    sa   = 0,
    sb   = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if(y1 == y2) last = y1;   // Include y1 scanline
    else         last = y1-1; // Skip it

    for(y=y0; y<=last; y++) {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) _swap_int16_t(a,b);
//        drawFastHLine(a, y, b-a+1, color);
        Displ_Line(a, y, b, y, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = (int32_t)dx12 * (y - y1);
    sb = (int32_t)dx02 * (y - y0);
    for(; y<=y2; y++) {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) _swap_int16_t(a,b);
//      drawFastHLine(a, y, b-a+1, color);
        Displ_Line(a, y, b, y, color);
    }
}









/***********************
 * @brief	display one character on the display
 * @param 	x,y: top left corner of the character to be printed
 * 			ch, font, color, bgcolor: as per parameter name
 * 			size: (1 or 2) single or double wided printing
 **********************/
void Displ_WChar(uint16_t x, uint16_t y, char ch, sFONT font, uint8_t size, uint16_t color, uint16_t bgcolor) {
    uint32_t i, b, bytes, j, bufSize, mask;

    const uint8_t *pos;
	uint8_t wsize=font.Width; //printing char width

	if (size==2)
		wsize<<= 1;
	bufSize=0;
	bytes=font.Height * font.Size ;
	pos=font.table+(ch - 32) * bytes ;//that's char position in table
	switch (font.Size) {
		case 3:
			mask=0x800000;
			break;
		case 2:
			mask=0x8000;
			break;
		default:
			mask=0x80;
	}

#ifdef Z_RGB565

	uint16_t color1, bgcolor1;
	uint16_t *dispBuffer16=(uint16_t *)dispBuffer;

	color1 = ((color & 0xFF)<<8 | (color >> 8));      		//swapping byte endian: STM32 is little endian, ST7735 is big endian
	bgcolor1 = ((bgcolor & 0xFF)<<8 | (bgcolor >> 8));		//swapping byte endian: STM32 is little endian, ST7735 is big endian

	for(i = 0; i < (bytes); i+=font.Size){
		b=0;
		switch (font.Size) {
			case 3:
				b=pos[i]<<16 | pos[i+1]<<8 | pos[i+2];
				break;
			case 2:
				b=pos[i]<<8 | pos[i+1];
				break;
			default:
				b=pos[i];
		}

		for(j = 0; j < font.Width; j++) {
			if((b << j) & mask)  {
				dispBuffer16[bufSize++] = color1;
				if (size==2){
					dispBuffer16[bufSize++] = color1;
				}
			} else {
				dispBuffer16[bufSize++] = bgcolor1;
				if (size==2) {
					dispBuffer16[bufSize++] = bgcolor1;
				}
			}
		}
	}
	bufSize<<=1;
#endif

#ifdef Z_RGB666
//  setting up char image in RGB666 format

	uint8_t Rcol=(color & 0xF800)>>8;
	uint8_t Gcol=(color & 0x07E0)>>3;
	uint8_t Bcol=(color & 0x001F)<<3;
	uint8_t Rbak=(bgcolor & 0xF800)>>8;
	uint8_t Gbak=(bgcolor & 0x07E0)>>3;
	uint8_t Bbak=(bgcolor & 0x001F)<<3;

	for(i = 0; i < (bytes); i+=font.Size){
		b=0;
		switch (font.Size) {
			case 3:
				b=pos[i]<<16 | pos[i+1]<<8 | pos[i+2];
				break;
			case 2:
				b=pos[i]<<8 | pos[i+1];
				break;
			default:
				b=pos[i];
		}

		for(j = 0; j < font.Width; j++) {
			if((b << j) & mask)  {
				dispBuffer[bufSize++] = Rcol;
				dispBuffer[bufSize++] = Gcol;
				dispBuffer[bufSize++] = Bcol;

				if (size==2){
					dispBuffer[bufSize++] = Rcol;
					dispBuffer[bufSize++] = Gcol;
					dispBuffer[bufSize++] = Bcol;
				}
			} else {
				dispBuffer[bufSize++] = Rbak;
				dispBuffer[bufSize++] = Gbak;
				dispBuffer[bufSize++] = Bbak;
				if (size==2) {
					dispBuffer[bufSize++] = Rbak;
					dispBuffer[bufSize++] = Gbak;
					dispBuffer[bufSize++] = Bbak;
				}
			}
		}
	}
#endif

	Displ_SetAddressWindow(x, y, x+wsize-1, y+font.Height-1);
	Displ_WriteData(dispBuffer,bufSize,0);
	dispBuffer = (dispBuffer==dispBuffer1 ? dispBuffer2 : dispBuffer1); // swapping buffer

}










void Displ_drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if(r > max_radius) r = max_radius;
    Displ_Line(x+r, y, x+w-r-1, y, color);
    Displ_Line(x+r, y+h-1, x-1+w-r, y+h-1, color);
    Displ_Line(x, y+r, x, y-1+h-r, color); // Left
    Displ_Line(x+w-1, y+r, x+w-1, y-1+h-r, color); // Right
    drawCircleHelper(x+r    , y+r    , r, 1, color);
    drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
    drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
    drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}





void Displ_fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if(r > max_radius) r = max_radius;
    Displ_FillArea(x+r, y, w-2*r, h, color);
    fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
    fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}







/************************
 * @brief	print a string on display starting from a defined position
 * @params	x, y	top left area-to-print corner
 * 			str		string to print
 * 			font	to bu used
 * 			size	1 (normal), 2 (double width)
 * 			color	font color
 * 			bgcolor	background color
 ************************/
void Displ_WString(uint16_t x, uint16_t y, const char* str, sFONT font, uint8_t size, uint16_t color, uint16_t bgcolor) {
	uint16_t delta=font.Width;
	if (size>1)
		delta<<=1;

    while(*str) {
/*
 *  these rows split oversize string in more screen lines
        if(x + font.Width >= _width) {
            x = 0;
            y += font.Height;
            if(y + font.Height >= _height) {
                break;
            }
            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }
 */
        Displ_WChar(x, y, *str, font, size, color, bgcolor);
        x += delta;
        str++;
    }
}





/************************
 * @brief	print a string on display centering into a defined area
 * @params	x0, y0	top left area corner
 * 			x1, y1	bottom right corner
 * 			str		string to print
 * 			font	to bu used
 * 			size	1 (normal), 2 (double width)
 * 			color	font color
 * 			bgcolor	background color
 ************************/
void Displ_CString(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, const char* str, sFONT font, uint8_t size, uint16_t color, uint16_t bgcolor) {
	uint16_t x,y;
	uint16_t wsize=font.Width;
	static uint8_t cambia=0;
	if (size>1)
		wsize<<=1;
	if ((strlen(str)*wsize)>(x1-x0+1))
		x=x0;
	else
		x=(x1+x0+1-strlen(str)*wsize) >> 1;
	if (font.Height>(y1-y0+1))
		y=y0;
	else
		y=(y1+y0+1-font.Height) >> 1;

	if (x>x0){
		Displ_FillArea(x0,y0,x-x0,y1-y0+1,bgcolor);
	} else
		x=x0; // fixing here mistake could be due to roundings: x lower than x0.
	if (x1>(strlen(str)*wsize+x0))
		Displ_FillArea(x1-x+x0-1,y0,x-x0+1,y1-y0+1,bgcolor);

	if (y>y0){
		Displ_FillArea(x0,y0,x1-x0+1,y-y0,bgcolor);
	} else
		y=y0; //same comment as above
	if (y1>=(font.Height+y0))
		Displ_FillArea(x0,y1-y+y0,x1-x0+1,y-y0+1,bgcolor);

	cambia = !cambia;

	Displ_WString(x, y, str, font, size, color, bgcolor);

}


#endif





/**************************************
 * @brief		set backlight level
 * 				PLEASE NOTE: if not in "DIMMING MODE" only 'F', '1', '0' and 'Q' available
 * @param	cmd	'S'		put display in stby (light level=BKLIT_STBY_LEVEL)
 * 				'W' 	wake-up from stdby restoring previous level
 *				'+'		add 1 step to the current light level
 *				'-'		reduce 1 step to the current light level
 *				'F','1'	set the display level to max
 *				'0'		set the display level to 0 (off)
 *				'I'		'Initialize'  IT MUST BE in dimming mode
 *              'Q'		do nothing, just return current level
 * @return		current backlight level
 **************************************/
uint32_t  Displ_BackLight(uint8_t cmd) {

#ifdef DISPLAY_DIMMING_MODE
	static uint16_t memCCR1=0;  			//it stores CCR1 value while in stand-by
#endif

	switch (cmd) {
#ifndef DISPLAY_DIMMING_MODE
	case 'F':
	case '1':
		HAL_GPIO_WritePin(DISPL_LED_GPIO_Port, DISPL_LED_Pin, GPIO_PIN_SET);
		break;
	case '0':
		HAL_GPIO_WritePin(DISPL_LED_GPIO_Port, DISPL_LED_Pin, GPIO_PIN_RESET);
		break;
#else
	case 'F':
	case '1':
		BKLIT_TIMER->BKLIT_CCR=BKLIT_TIMER->ARR;
		break;
	case '0':
		BKLIT_TIMER->BKLIT_CCR=0;
		break;
	case 'W':
		BKLIT_TIMER->BKLIT_CCR=memCCR1;					//restore previous level
		break;
	case 'S':
		memCCR1=BKLIT_TIMER->BKLIT_CCR;
		if (BKLIT_TIMER->BKLIT_CCR>=(BKLIT_STBY_LEVEL))	//set stby level only if current level is higher
			BKLIT_TIMER->BKLIT_CCR=(BKLIT_STBY_LEVEL);
		break;
	case '+':
		if (BKLIT_TIMER->ARR>BKLIT_TIMER->BKLIT_CCR)		// if CCR1 has not yet the highest value (ARR)
			++BKLIT_TIMER->BKLIT_CCR;
		else
			BKLIT_TIMER->BKLIT_CCR=BKLIT_TIMER->ARR;
		break;
	case '-':
		if (BKLIT_TIMER->BKLIT_CCR>0)					// if CCR1 has not yet the lowest value (0)
			--BKLIT_TIMER->BKLIT_CCR;
		else
			BKLIT_TIMER->BKLIT_CCR=0;
		break;
	case 'I':
	  	HAL_TIM_PWM_Start(&BKLIT_T, BKLIT_CHANNEL);
		BKLIT_TIMER->BKLIT_CCR=BKLIT_INIT_LEVEL;
		break;
#endif
	case 'Q':
		break;
	default:
		break;
	}
#ifndef DISPLAY_DIMMING_MODE
	return HAL_GPIO_ReadPin(DISPL_LED_GPIO_Port, DISPL_LED_Pin);
#else
	return (BKLIT_TIMER->BKLIT_CCR);
#endif
}



/************************
 * @brief	TouchGFX integration: returns status of communication to the display
 * @return	1 = there is a transmission running
 * 			0 = no transmission
 ************************/
int touchgfxDisplayDriverTransmitActive(){
	// using the flag indicating SPI port availability
	// already used to drive communication via DMA
	return (!Displ_SpiAvailable);
}


/************************
 * @brief	TouchGFX integration: write to display the block indicated by parameters
 *
 ************************/
void touchgfxDisplayDriverTransmitBlock(const uint8_t* pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	//START WRITING TO DISPLAY
		Displ_SetAddressWindow(x, y, x+w-1, y+h-1);
		Displ_WriteData((uint8_t* )pixels,((w*h)<<1),1);
}

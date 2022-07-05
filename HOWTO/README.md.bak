_**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**Below English text you'll find the Italian version</i>**_

<br>
<br>

## HOW TO setup a project with this software


##### CubeMX setup
- create a new project on STM32CubeIDE
- enable an SPI port. Configuration is:

|Parameter setting field|value|
|---|---|
|mode|Half-Duplex Master|
|NSS|Hardware NSS Output Signal|
|(as per ST7735 datasheet) Baud Rate|<= 15 MBit/s|
  - all other parameter as default:
  
|Parameter setting field|value|
|---|---|
|Frame format|Motorola|
|Data size|8 bit|
|First bit|MSB first|	
|CPOL|low|
|CPHA|Edge|
|CRC calculation|disabled|
|NSS type|Output Hw|
  - if communication in DMA mode:
  
|SPI config label|value|
|---|---|
|DMA settings|enable only TX DMA|
|NVIC settings|enable SPI global interrupt|
- assign these names to SPI pins:

|SPI pin|pinname to assign|
|---|---|
|NSS|DISPL_CS|
|MOSI|DISPL_MOSI|
|SCK|DISPL_SCK|

- enable 3 more pins as GPIO_Output:

|pin configuration|
|---|
|Output push pull|
|No pull-up/down|
	
|pinname to assign|
|---|
|DISPL_LED|
|DISPL_DC|
|DISPL_RST|

##### Downloading SW
- download and install software as described [here](../SOURCE)

##### Parameters setup
into "z_displ_ST7735.h" file you have to setup this configuration:
- section 1 - Port Parameters: here you have to set two macro constant with the SPI port name connecting display
- section 2 - SPI communication mode: uncomment the macro definition related to enabled communication mode (Polling mode, Interrupt mode or DMA mode). You must uncomment no less and no more than ONE definition here
- section 3 - Backlight Mode: see [BACKLIGHT page](../BACKLIGHT)  
- section 4 - Buffer size: define the "Buffer Level" you like (assign a value between 10 and 16 included) see [youtube video](../BACKLIGHT) for information and analisys.  
- section 5 - Display size: despite the ST7735 SPI driver can handle displays of any size and color modes, this library was tested only on a 128x160 display running in RBG565.  

---
## HOW TO: come configurare un progetto con questo software


##### COnfigurazione CubeMX
- crea un nuovo progetto su STM32CubeIDE
- attiva una porta SPI con questa configurazione:

|Parameter setting field|value|
|---|---|
|mode|Half-Duplex Master|
|NSS|Hardware NSS Output Signal|
|(come da datasheet del driver ST7735) Baud Rate|<= 15 MBit/s|
  
|Parameter setting field|value|
|---|---|
|Frame format|Motorola|
|Data size|8 bit|
|First bit|MSB first|	
|CPOL|low|
|CPHA|Edge|
|CRC calculation|disabled|
|NSS type|Output Hw|
  - se attivato DMA:
  
|SPI config label|value|
|---|---|
|DMA settings|enable only TX DMA|
|NVIC settings|enable SPI global interrupt|
- assegnare questi nomi ai pin SPI:

|pin SPI|pinname da assegnare|
|---|---|
|NSS|DISPL_CS|
|MOSI|DISPL_MOSI|
|SCK|DISPL_SCK|
- attivare altri 3 pin come GPIO_Output:

|configurazione pin|
|---|
|Output push pull|
|No pull-up/down|
	
|pinname da assegnare|
|---|
|DISPL_LED|
|DISPL_DC|
|DISPL_RST|

##### Scaricare il SW
- scaricare ed installare il software come descritto [qui](../SOURCE)

##### Configurazione Parametri
nel file "z_displ_ST7735.h" devi impostare questa configurazione:
- sezione 1 - Port Parameters: qui devi impostare due costanti macro con il nome della porta SPI a cui è connesso il display
- section 2 - SPI communication mode: togli il commento alla definizione definition relativa al tipo di comunicazione attivato (Polling moode, Interrupt mode o DMA mode). Devi togliere il commento ad una riga (NON di più e NON di meno) 
- section 3 - Backlight Mode: vedi la [pagina BACKLIGHT](../BACKLIGHT)  
- section 4 - Dimensione del Buffer: imposta il "Buffer Level" che preferisci (assegna un valore tra 10 e 16 inclusi) vedi [il video youtube](../BACKLIGHT) per informazioni ed analisi.
- section 5 - Dimensione Display: anche se il driver SPI ST7735 può gestire display di dimensioni diverse, questa libreria è stata testata solo su un display di 128x160 pixel in modalità RBG565.  
 


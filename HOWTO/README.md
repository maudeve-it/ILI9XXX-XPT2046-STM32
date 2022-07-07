_**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**Below English text you'll find the Italian version</i>**_

<br>
<br>

## HOW TO setup a project with this software
(here using a single SPI port for both: display and touch sensor)


##### CubeMX setup
- create a new project on STM32CubeIDE
- enable one or two (one port for display and one for touch sensor) SPI port.  

SPI configuration is:

|Mode setting field|value|
|---|---|
|mode|Full-Duplex Master|
|NSS|Disable|
  
|Parameter setting field|value|
|---|---|
|Frame format|Motorola|
|Data size|8 bit|
|First bit|MSB first|	
|Baud Rate| irrelevant (parameter is set by software)|
|CPOL|low|
|CPHA|1 Edge|
|CRC calculation|disabled|
|NSS type|Output Hw|

you can choose if transferring SPI data in Polling mode, Interrupt mode or DMA mode.
I strongly suggest you to see [this youtube video](https://youtu.be/oWx1-WmTwag) to see differences 

If communication in DMA mode:
|SPI config label|value|
|---|---|
|DMA settings|enable only TX DMA|
|NVIC settings|enable SPI global interrupt|

Assign these names to SPI pins:
|SPI pin|pinname to assign|speed relevance|
|---|---|---|
|MISO|TOUCH_MISO|X|
|MOSI|DISPL_MOSI|X|
|SCK|DISPL_SCK|X|

"Speed relevance" (here above and below): "X" means that pin speed affects the defined SPI boudrate handling: rise (step by step from LOW to VERY HIGH) pins speed if you see uC cannot handle the defined communication speed.

Enable 1 EXTI interrupt pin:
|pinname to assign|Interrupt mode|pull-up/down|
|---|---|---|
|TOUCH_INT|External interrupt-falling edge|pull-up|

Enable 5 more pins as GPIO_Output:
|pinname to assign|output level|speed relevance|mode|pull-up/down|
|---|---|---|---|
|DISPL_LED|low|-|Output push pull|No pull-up/down|
|DISPL_DC|-|X|Output push pull|No pull-up/down|
|DISPL_RST|low|-|Output push pull|No pull-up/down|
|DISPL_CS|high|X|Output push pull|No pull-up/down|
|TOUCH_CS|hlgh|X|Output push pull|No pull-up/down|


##### Downloading SW
- download and install software as described [here](../SOURCE)

##### Parameters setup
into "z_displ_ILI9XXX.h" file you have to setup this configuration:
- section 1 - define the display you are using (ILI9488 or ILI9346) uncommenting the relative #define
- section 2 - Port Parameters: here you have to set two macro constant with the SPI port name connecting display (see below "z_touch_XPT2046.h" also)
- section 3 - Port Speed: here you must assign bitrate SPI prescaler when transferring data to display or to/from touch sensor. Consider that touch uses baudrates  below 2MBps 
- section 4 - SPI communication mode: uncomment the macro definition related to enabled communication mode (Polling mode, Interrupt mode or DMA mode). You must uncomment no less and no more than ONE definition here
- section 5 - Backlight Mode: see [BACKLIGHT page](../BACKLIGHT)  
- section 6 - Buffer size: define the "Buffer Level" you like (assign a value between 10 and 16 included) see [youtube video](../BACKLIGHT) for information and analisys.  

into "z_touch_XPT2046.h" file you have to setup this configuration:
- section 1 - Port Parameters: here you have to set two macro constant with the SPI port name connecting touch sensor


### Wiring

<img src="https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/blob/main/HOWTO/wiring.png" width="540" height="429">

---
## HOW TO: come configurare un progetto con questo software


##### Configurazione CubeMX
- crea un nuovo progetto su STM32CubeIDE
- attiva una porta SPI con questa configurazione:

|Marametro Mode setting|valore|
|---|---|
|mode|Full-Duplex Master|
|NSS|Disable|
 
|Parameter setting|valore|
|---|---|
|Frame format|Motorola|
|Data size|8 bit|
|First bit|MSB first|	
|Baud Rate| irrilevante (il parametro è impostato via software)|
|CPOL|low|
|CPHA|Edge|
|CRC calculation|disabled|
|NSS type|Output Hw|

Puoi scegliere se il trasferimento dati avviene in Polling Mode, via Interrupt o via DMA
Raccomando di visionare [questo video youtube](https://youtu.be/oWx1-WmTwag) per analizzare le differenti impostazioni 

Se la comunicazione avviene via DMA:
|Campo SPI config|valore|
|---|---|
|DMA settings|attiva solo TX DMA|
|NVIC settings|attiva SPI global interrupt|

Assegnare questi nomi ai pin SPI:
|pin SPI|pinname da assegnare|rilevanza velocità|
|---|---|---|
|NSS|DISPL_CS|X|
|MOSI|DISPL_MOSI|X|
|SCK|DISPL_SCK|X|

"Rilevanza velocità" (qui sopra e più sotto): "X" significa che la velocità del pin ha effetto sulla possibilità di gestire il baudrate della porta SPI: aumentare (un livello alla volta da "LOW" a "VERY HIGH") la velocità dei pin se vedi che il uC non riesce a gestire la velocità di comunicezione impostata. 

Attivare 1 pin come EXTI interrupt:
|Nome pin da assegnare|Interrupt mode|pull-up/down|
|---|---|---|
|TOUCH_INT|External interrupt-falling edge|pull-up|

Attivare altri 5 pin come GPIO_Output:
|Nome pin da assegnare|output level|rilevanza velocità|mode|pull-up/down|
|---|---|---|---|---|
|DISPL_LED|low|-|Output push pull|No pull-up/down|
|DISPL_DC|-|X|Output push pull|No pull-up/down|
|DISPL_RST|low|-|Output push pull|No pull-up/down|
|DISPL_CS|high|X|Output push pull|No pull-updown|
|TOUCH_CS|hlgh|X|Output push pull|No pull-up/down|


##### Scaricare il SW
- scaricare ed installare il software come descritto [qui](../SOURCE)


##### Configurazione Parametri

Nel file "z_displ_ILI9XXX.h" devi impostare questa configurazione:
- sezione 1 - Definisci il display che stai usando (ILI9488 o ILI9346) togliendo ail commento al #define relativo
- sezione 2 - Port Parameters: qui devi impostare due costanti macro con il nome della porta SPI a cui è connesso il display (vedi anche "z_touch_XPT2046.h", sotto)
- sezione 3 - Port Speed: qui devi assegnare il valore al bitrate SPI prescaler quando trasferisce dati al display o al/dal sensore touch. Considera che il sensore touch comunica sotto i 2MBps 
- sezione 4 - SPI communication mode: togli il commento alla definizione definition relativa al tipo di comunicazione attivato (Polling moode, Interrupt mode o DMA mode). Devi togliere il commento ad una riga (NON di più e NON di meno) 
- sezione 5 - Backlight Mode: vedi la [pagina BACKLIGHT](../BACKLIGHT)  
- sezione 6 - Buffer size: definisci il "Buffer Level" che preferisci (assegnaun valire tra 10 e 16 included) vedi [il video youtube](https://youtu.be/oWx1-WmTwag) per informazioni ed analisi.  

Nel file "z_touch_XPT2046.h" devi impostare i seguenti parametri:
- sezione 1 - Port Parameters: qui devi impostare due costanti macro con il nome della porta SPI a cui è connesso il display 



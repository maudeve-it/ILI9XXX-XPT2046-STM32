_**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**Below English text you'll find the Italian version</i>**_

<br>
<br>

## HOW TO setup a project with this software


##### CubeMX setup
- create a new project on STM32CubeIDE
- enable one or two (one port for display and one for touch sensor) SPI port.  
(here description is using ONE single SPI port for both: display and touch sensor)

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
|NSS type|Output Sw|

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

"Speed relevance" (here above and below): "X" means that pin speed affects the defined SPI boudrate handling: rise (step by step from LOW to VERY HIGH) pins speed if you see uC cannot handle the defined communication speed (more information on your uC datasheet). As per [GPIO software guidelines](https://www.st.com/resource/en/application_note/an4899-stm32-microcontroller-gpio-hardware-settings-and-lowpower-consumption-stmicroelectronics.pdf), STM suggests keeping the lower GPIO speed allowing your project handling, reducing power consumption and EMI.

Enable 1 EXTI interrupt pin:
|pinname to assign|Interrupt mode|pull-up/down|
|---|---|---|
|TOUCH_INT|External interrupt-falling edge|pull-up|

Then,<br>
enable the corresponding interrupt in the "NVIC session" of "System Core" configuration.<br> 

Enable 5 more pins as GPIO_Output:
|pinname to assign|output level|speed relevance|mode|pull-up/down|
|---|---|---|---|---|
|DISPL_LED|low|-|see [here](../BACKLIGHT)|No pull-up/down|
|DISPL_DC|-|X|Output push pull|No pull-up/down|
|DISPL_RST|low|-|Output push pull|No pull-up/down|
|DISPL_CS|high|X|Output push pull|No pull-up/down|
|TOUCH_CS|hlgh|X|Output push pull|No pull-up/down|


##### Next steps
- ["How to" add this library to the created project](../2-SOURCE)
- ["How to" handle display backlight](../3-BACKLIGHT)
- ["How to" run the project, test it, and learn using its functions](../4-HELLO_WORLD)


---


### Wiring/cablaggio

<img src="https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/blob/main/1-HOWTO/wiring.png" width="540" height="429">


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
|NSS type|Output Sw|

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

"Rilevanza velocità" (qui sopra e più sotto): "X" significa che la velocità del pin ha effetto sulla possibilità di gestire il baudrate della porta SPI: aumentare (un livello alla volta da "LOW" a "VERY HIGH") la velocità dei pin se vedi che il uC non riesce a gestire la velocità di comunicezione impostata  (maggiori informazioni sul datasheet del tup uC). Come indicato su [GPIO software guidelines](https://www.st.com/resource/en/application_note/an4899-stm32-microcontroller-gpio-hardware-settings-and-lowpower-consumption-stmicroelectronics.pdf), STM suggerisce di usare la minore velocità GPIO che permette di gestire il tuo progetto, riducendo consumi ed EMI. 

Attivare 1 pin come EXTI interrupt:
|Nome pin da assegnare|Interrupt mode|pull-up/down|
|---|---|---|
|TOUCH_INT|External interrupt-falling edge|pull-up|

Poi,<br>
abilitare il corrispondente interrupt nella "NVIC session" della configurazione "System Core".<br> 

Attivare altri 5 pin come GPIO_Output:
|Nome pin da assegnare|output level|rilevanza velocità|mode|pull-up/down|
|---|---|---|---|---|
|DISPL_LED|low|-|vedi [here](../BACKLIGHT)|No pull-up/down|
|DISPL_DC|-|X|Output push pull|No pull-up/down|
|DISPL_RST|low|-|Output push pull|No pull-up/down|
|DISPL_CS|high|X|Output push pull|No pull-updown|
|TOUCH_CS|hlgh|X|Output push pull|No pull-up/down|


##### Prossimi passi
- [Guida per aggiungere la libreria al progetto creato](../2-SOURCE)
- [Guida alla gestione della retroilluminazione](../3-BACKLIGHT)
- [Guida per eseguire il progetto, testarlo, e conoscere le sue funzioni](../4-HELLO_WORLD)


_**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**Below English text you'll find the Italian version</i>**_

<br>
<br>

# HOW TO setup a project with this software


## CubeMX setup

<br><b>
- create a new project on STM32CubeIDE
- enable one or two (one port for display and one for touch sensor) SPI port.  
</b>
(here description is using ONE single SPI port for both: display and touch sensor)<br><br>

  
SPI configuration is:

|Mode|value|
|---|---|
|mode|Full-Duplex Master|
|NSS|Disable|
  
|Parameter|value|
|---|---|
|Frame format|Motorola|
|Data size|8 bit|
|First bit|MSB first|	
|Baud Rate| irrelevant (parameter is set by software)|
|CPOL|low|
|CPHA|1 Edge|
|CRC calculation|disabled|
|NSS type|Output Sw|
  
If you use SPI in "polling mode", do not enable Interrupt and DMA and not consider the below table: go over<br>
If you use SPI in "interrupt mode", do not enable DMA and consider only the NVIC information in the below table<br>
If you use SPI in "DMA mode", enable both Interrupt and DMA following the below table.

|SPI config label|Value|
|---|---|
|DMA settings|enable only TX DMA|
|NVIC settings|enable SPI global interrupt|

<br><b>
- assign these names to SPI pins:</b>

|SPI pin|Pinname to assign|Speed relevance|
|---|---|---|
|MISO|TOUCH_MISO|X|
|MOSI|DISPL_MOSI|X|
|SCK|DISPL_SCK|X|

>"Speed relevance" (here above and below): <br>
>"X" means that pin speed affects the defined SPI boudrate handling: rise (step by step from LOW to VERY HIGH) pins speed if you see uC cannot handle the defined communication speed (more information on your uC datasheet).<br>
>As per [GPIO software guidelines](https://www.st.com/resource/en/application_note/an4899-stm32-microcontroller-gpio-hardware-settings-and-lowpower-consumption-stmicroelectronics.pdf), STM suggests keeping the lower GPIO speed allowing your project handling, reducing power consumption and EMI.<br>

<br><b>
- Enable 1 EXTI interrupt pin:</b>

|pinname to assign|Interrupt mode|pull-up/down|
|---|---|---|
|TOUCH_INT|External interrupt-falling edge|pull-up|

Then enable the corresponding interrupt in the "NVIC session" of "System Core" configuration.<br> 

<br><b>
- Enable 5 more pins as GPIO_Output:</b>

|pinname to assign|output level|speed relevance|mode|pull-up/down|
|---|---|---|---|---|
|DISPL_LED|low|-|see [here](../2-BACKLIGHT)|No pull-up/down|
|DISPL_DC|-|X|Output push pull|No pull-up/down|
|DISPL_RST|low|-|Output push pull|No pull-up/down|
|DISPL_CS|high|X|Output push pull|No pull-up/down|
|TOUCH_CS|hlgh|X|Output push pull|No pull-up/down|

<br><b>
- TouchGFX timer (only if "TouchGFX Full Mode")</b>

<br>Skip this step if you are setting-up a project in "Direct Mode" or "TouchGFX Button Mode"<br><br>
Set-up a basic timer just having a loop 60 times per second, generating a "period elapsed" interrupt:<br>
-  set PSC and ARR as per uC clock giving the timer a period of 1/60 s
-  enable timer interrupt
<br>

## Source and header files

- Copy the .c files into the /Core/Src folder in your STM32CubeIDE project
- Copy the .h files into the /Core/Inc folder in your STM32CubeIDE project
- into the /Core/Inc folder, open the main.h file and add an include directive for every .h file copied. In this order:<br>
<br>

```sh
(main.h)
...
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fonts.h"
#include "z_displ_ILI9XXX.h"
#include "z_displ_ILI9XXX_test.h"
#include "z_touch_XPT2046.h"
#include "z_touch_XPT2046_test.h"
#include "z_touch_XPT2046_menu.h"
/* USER CODE END Includes */
...
```
<br>

>_**PLEASE NOTE</i>**_<br>
>
>- In a "TouchGFX" project you need only  "z_displ_ILI9XXX.h", "z_displ_ILI9XXX.c", "z_touch_XPT2046.h" and "z_touch_XPT2046.c"<br>
>- In a "Direct handling" project you need "_test" files (.c and .h) only if you want to use graphics test/demo functions, you don't need to add it in the production project.<br>
>- Adding "z_touch_XPT2046_test", compiler will ask you to enable printf library into project properties (follow warnings instructions) <br>
<br>

## Next steps
- ["How to" handle display backlight](../2-BACKLIGHT)
- for Direct Handling projects: [(Direct handling) "How to" add this library to the created project](../3B-DIRECT)
- for TouchGFX "button mode" projects: [(TouchGFX) "How to" add this library to the created project](../3A-TOUCHGFX)
- for TouchGFX "full mode" projects: [(TouchGFX) "How to" add this library to the created project](../3C-TOUCHGFX-FM)
- next, for all type of projects:  ["How to" calibrate touch sensor](../4-CALIBRATION)


<br>

---


### Wiring/collegamenti (single port)

<img src="https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/blob/main/1-HOWTO/wiring.png" width="540" height="429">


---
<br>

# HOW TO: come configurare un progetto con questo software
<br>


## Configurazione CubeMX
<br><b>
- crea un nuovo progetto su STM32CubeIDE
- attiva una porta SPI con questa configurazione:
</b><br>

|Parametro|Valore|
|---|---|
|mode|Full-Duplex Master|
|NSS|Disable|
  
|Parametro|Valore|
|---|---|
|Frame format|Motorola|
|Data size|8 bit|
|First bit|MSB first|	
|Baud Rate| irrilevante (il parametro è impostato via software)|
|CPOL|low|
|CPHA|Edge|
|CRC calculation|disabled|
|NSS type|Output Hw|

Se usi SPI in "polling mode" non attivare Interrupt e DMA sulla SPI e non considerare la tabella sotto: procedi oltre<br>
Se usi SPI in "interrupt mode" non attivare DMA e considera solo NVIC settings nella tabella sotto<br>
Se usi SPI in "DMA mode" abilita sia Interrupt sia DMA e segui la tabella sotto
  
|Etichetta SPI|Valore|
|---|---|
|DMA settings|attiva solo TX DMA|
|NVIC settings|attiva SPI global interrupt|

<br><b>
- assegnare questi nomi ai pin SPI:</b>

|Pin SPI|Nome da assegnare|Rilevanza velocità|
|---|---|---|
|MISO|TOUCH_MISO|X|
|MOSI|DISPL_MOSI|X|
|SCK|DISPL_SCK|X|

>"Rilevanza velocità" (qui sopra e sotto):<br>
>"X" significa che la velocità del pin ha effetti sulla velocità della porta SPI: aumenta (per gradi da LOW to VERY HIGH) la velocità dei pin se vedi che il uC non riesce a gestire la velocità di comunicazione assegnata (maggiori informazioni sul datasheet del tuo uC).<br>
>Come indicato in ["GPIO software guidelines"](https://www.st.com/resource/en/application_note/an4899-stm32-microcontroller-gpio-hardware-settings-and-lowpower-consumption-stmicroelectronics.pdf), STM suggerisce di mantenere la minore velocità GPIO che permette la gestione del tuo progetto, ciò per ridurre consumi ed EMI.<br>

<br><b>
-  Attivare 1 pin come EXTI interrupt:</b>

|Nome pin da assegnare|Interrupt mode|pull-up/down|
|---|---|---|
|TOUCH_INT|External interrupt-falling edge|pull-up|

Poi,<br>
abilitare il corrispondente interrupt nella "NVIC session" della configurazione "System Core".<br> 

<br><b>
-  Attivare altri 5 pin come GPIO_Output:</b>

|Nome pin da assegnare|output level|rilevanza velocità|mode|pull-up/down|
|---|---|---|---|---|
|DISPL_LED|low|-|vedi [here](../3-BACKLIGHT)|No pull-up/down|
|DISPL_DC|-|X|Output push pull|No pull-up/down|
|DISPL_RST|low|-|Output push pull|No pull-up/down|
|DISPL_CS|high|X|Output push pull|No pull-updown|
|TOUCH_CS|hlgh|X|Output push pull|No pull-up/down|

<br><b>
- TouchGFX timer (solo se in "TouchGFX Full Mode")
</b>

<br>Salta questo passaggio se imposti un progetto in "Direct Mode" o "TouchGFX Button Mode"<br><br>
Imposta un basic timer che semplicemente abbia un ciclo di 1/60 di secondo, e che genera un "period elapsed" interrupt:<br>
-  imposta PSC and ARR in funzione del clock del uC assegnando al timer un periodo di 1/60 s
-  attiva l'interrupt del timer
<br>


## File sorgente e di testata

- Copia i file .c nella cartella /Core/Src del progetto STM32CubeIDE
- Copia i file .h nella cartella /Core/Inc del progetto STM32CubeIDE
<br>

- Nella cartella /Core/Inc, apri il file main.h e aggiungi la direttiva include (nella sezione "USER CODE BEGIN Includes"?) per ogni file .h copiato.

```sh
(main.h)
...
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fonts.h"
#include "z_displ_ILI9XXX.h"
#include "z_displ_ILI9XXX_test.h"
#include "z_touch_XPT2046.h"
#include "z_touch_XPT2046_test.h"
#include "z_touch_XPT2046_menu.h"
/* USER CODE END Includes */
...
```

<br>

>_**NOTA BENE:</i>**_<br>
>
>- In un progetto "TouchGFX" ti servono solo "z_displ_ILI9XXX.h"y, "z_displ_ILI9XXX.c", "z_touch_XPT2046.h" and "z_touch_XPT2046.c"<br>
>- In un progetto "a gestione diretta" i file "_test" (.c and .h)devono essere copiati nel progetto solo se vuoi usare le funzioni di demo e test. Non servono nella versione di produzione.<br>
>- Aggiungendo i file "z_touch_XPT2046_test", il compilatore chiederà di attivare la libreria "printf" nelle proprietà del progetto (seguire le indicazioni dei warning) <br>


## Prossimi passi
- [Guida alla gestione della retroilluminazione](../2-BACKLIGHT)
- per progetti con Gestione Diretta: [(gestione diretta) Guida per aggiungere la libreria al progetto creato](../3B-DIRECT)
- per progetti TouchGFX "button mode": [(TouchGFX) Guida per aggiungere la libreria al progetto creato](../3A-TOUCHGFX)
- per progetti TouchGFX "full mode": [(TouchGFX) Guida per aggiungere la libreria al progetto creato](../3C-TOUCHGFX-FM)
- poi, per tutti i tipi di progetti:  [Guida per la calibrazione del sensore touch](../4-CALIBRATION)

rel. 1.2: added TouchGFX handling for RGB666 displays (ILI9488v1.0)
<br>
<br>

_**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**Below English text you'll find the Italian version</i>**_

<br>
<br>
##rel. 1.2: added TouchGFX handling for RGB666 displays (ILI9488v1.0)
<br>
<br>

# ILI9XXX/XPT2046-STM32

"ILI9XXX/XPT2046-STM32" repository contains the source code for a set of functions for an STM32 microcontroller handling a ILI9341 based 240x320 and ILI9488 based 320x480 touch display.<br>
Library allows to handle the display using either TouchGFX or directly drawing on screen through library functions.

Developed software is based on HAL over STM32CubeIDE.<br>

|Software has been tested over:||
|---|---|
|STM32F411CE (Weact Studio board)|https://github.com/WeActTC/MiniSTM32F4x1|
|STM32G432KB (Nucleo32 STM board)|https://www.st.com/en/evaluation-tools/nucleo-g431kb.html|
|STM32G071RB (Nucleo64 STM board)|https://www.st.com/en/evaluation-tools/nucleo-g071rb.html|

|Displays used||
|---|---|
|2.4 inch touch TFT-LCD 240x320|[AliExpress](https://it.aliexpress.com/item/32832952750.html)|
||[Amazon](https://www.amazon.it/ILI9341-240x320-Display-pollici-Arduino/dp/B07YTW28FB)|
|3.5 inch touch TFT-LCD 320x480|[AliExpress](https://www.aliexpress.com/item/1005003518881469.html)|
| |[Amazon](https://www.amazon.it/LOLOVI-pollice-480x320-Seriale-Digitale/dp/B09FJYX7YQ/ref=sr_1_5)|
				
---

## Project Folders

(I suggest you to follow this reading order) 
- ["How to" create CubeIDE project using this software (source folder)](./1-HOWTO)
- ["How to" handle display backlight](./2-BACKLIGHT)
- [(Direct handling) "How to" add this library to the created project](./3B-DIRECT)
- [(TouchGFX) "How to" add this library to the created project](./3A-TOUCHGFX)

Youtube video showing TouchGFX integration characteristics and performance: https://youtu.be/g1siKaPox88<br>
Youtube video showing performing tests of library: https://youtu.be/020k2vbLAPc<br>
Youtube video with a detailled analysis of SPI communication to displays: https://youtu.be/oWx1-WmTwag<br>

---
> Please Note:
> The software, schemes and PCB designs in this set of repositories are provided just for 
> illustrative purposes, sharing the personal experience on this project. 
> The software and design artifacts are provided as-is without any mantainence and without
> warranty of any kind. There is no guarantee of fitness for any particular application, 
> and no guarantee of completeness and correctness. 
> By downloading and using these materials the user accepts all risks associated with the
> run of the software and construction and use of the circuits and devices described.<br>
> Fonts licence: fonts are provided by STMicroelectronics. Licencing inside the fonts ".c" files. 
---

## NOTES


---

<br>
<br>

# ILI9XXX/XPT2046-STM32

La repository "ILI9XXX/XPT2046-STM32" contiene il codice relativi ad un set di funzioni per STM32 per la gestione di display LCD touch da 240x320 basato su ILI9341 e da 320x480 basato su ILI9488.<br>
La libreria di funzioni permette di gestire il display in un progetto basato su TouchGFX oppure scrivendo direttamente sul display tramite le funzioni disponibili.<br>

Il software sviluppato, si basa su HAL attraverso STM32CubeIDE.<br>

|Il software è stato testato su||
|---|---|
|STM32F411CE (scheda Weact Studio)|https://github.com/WeActTC/MiniSTM32F4x1|
|STM32G432KB (scheda STM Nucleo32)|https://www.st.com/en/evaluation-tools/nucleo-g431kb.html|
|STM32G071RB (scheda STM Nucleo64)|https://www.st.com/en/evaluation-tools/nucleo-g071rb.html|

|I display utilizzati sono:||
|---|---|
|2.4 inch touch TFT-LCD 240x320|[AliExpress](https://it.aliexpress.com/item/32832952750.html)|
||[Amazon](https://www.amazon.it/Hiletgo%C2%AE-320-risoluzione-Touch-panel-ILI9341/dp/B0798N2HHW/ref=sr_1_3_sspa)|
|3.5 inch touch TFT-LCD 320x480|[AliExpress](https://www.aliexpress.com/item/1005003518881469.html)|
| |[Amazon](https://www.amazon.it/LOLOVI-pollice-480x320-Seriale-Digitale/dp/B09FJYX7YQ/ref=sr_1_5)|

---

## Cartelle del progetto 

- [Guida per creare un progetto CubeIDE per usare questa libreria (il codice è qui)](./1-HOWTO)
- [Guida alla gestione della retroilluminazione](./2-BACKLIGHT)
- [(gestione diretta) Guida per aggiungere la libreria al progetto creato](./3B-DIRECT)
- [(TouchGFX) Guida per aggiungere la libreria al progetto creato](./3A-TOUCHGFX)

Video Youtube con le carateristiche e le prestazioni dell'integrazione TouchGFX: https://youtu.be/g1siKaPox88<br>
Video Youtube con le carateristiche e le prestazioni dellla libreria: https://youtu.be/020k2vbLAPc<br>
Video Youtube con una analisi di dettaglio sulla comunicazione con i display: https://youtu.be/oWx1-WmTwag<br>

---
> Il software e gli schemi di progetto come i layout PCB in questa serie di repository 
> sono messe a disposizione con puro intento illustrativo e di condivisione dell'esperienza fatta
> Il software and gli elementi del progetto sono messi a disposizione "allo stato in cui sono"
> senza impegno di manutenzione e senza garanzie di alcun tipo. Piu' esplicitamente, non c'e' garanzia di 
> adeguatezza a soddisfare specifiche esigenze, ne di completezza o correttezza di alcuna parte.
> Scaricando ed utilizzando questo materiale l'utilizzatore accetta il rischio associato alla
> realizzazione ed all'utilizzo del circuito, del software e dei componenti descritti in questi file.<br> 
La serie di font usata è fornita da STMicroelectronics. Licenza d'uso all'interno dei file ".c" dei font. 

---

## NOTE


---



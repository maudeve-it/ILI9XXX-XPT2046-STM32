<br>


_**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**Below English text you'll find the Italian version</i>**_

<br>
<br>


# ILI9XXX/XPT2046-STM32

"ILI9XXX/XPT2046-STM32" repository contains the source code of a set of functions for an STM32 microcontroller handling a ILI9341-based 240x320 and ILI9488-based 320x480 touch display.<br>
Three modes available using the library:
-	<b>Direct Display Handling</b>:<br>
	library provides a quite fast set of functions directly writing to the display driver via SPI. That allows a lightweight graphics handling (below you can find links to Youtube videos showing library performance)
-	<b>TouchGFX "button mode"</b>:<br>
	following related instructions, you can setup a "reduced impact" TouchGFX project with a good mix in terms of graphics and performance but using mainly (let's say "only") buttons as GUI input widgets 
-	<b>TouchGFX "full mode"</b>:<br>
	this way, you can setup the library to handle the full set of widgets/transitions/animations/etc. provided by TouchGFX (...the full set... well, keep in consideration the "phisical restrictions" imposed by the display boards: small size / resistive touch sensor / no vsync returned / simple SPI port / ...)

Developed software is based on HAL over STM32CubeIDE.<br>
<br>
Software has been tested over:<br>

|||
|---|---|
|STM32F411CE (Weact Studio board)|https://github.com/WeActTC/MiniSTM32F4x1|
|STM32G432KB (Nucleo32 STM board)|https://www.st.com/en/evaluation-tools/nucleo-g431kb.html|
|STM32G071RB (Nucleo64 STM board)|https://www.st.com/en/evaluation-tools/nucleo-g071rb.html|

<br>
Displays used<br>

|||
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
- [(TouchGFX button mode) "How to" add this library to the created project](./3A-TOUCHGFX)
- [(TouchGFX full mode) "How to" add this library to the created project](./3C-TOUCHGFX-FM)
- ["How to" calibrate touch sensor](./4-CALIBRATION)



Youtube video showing TouchGFX integration characteristics and performance: https://youtu.be/g1siKaPox88<br>
Youtube video showing "How to" setup a full "full mode" TouchGFX project: https://youtu.be/bG6XCDyCNzo<br>
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

## Troubleshooting
-	(direct handling and TouchGFX)<br>
	Display works fine in polling mode and interrupt mode but not in DMA mode:<br> 
	https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/issues/2

-	(direct handling and TouchGFX)<br>
	Display graphics works fine but not responding to touch:<br> 
	https://youtu.be/KS5u2-6yruU?t=251
	
-	(only TouchGFX)<br>
	Non-synchronous display:<br> 
	https://youtu.be/KS5u2-6yruU?t=810

-	(direct handling and TouchGFX) (v. 1.2)<br>
	Working with H7:<br> 
	https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/issues/3


<br>
---

## rev. history

-	1.0		-	first release
-	TouchGFX 1.0	-	added TouchGFX integration
-	TouchGFX 1.1	-	bug fixing
-	TouchGFX 1.2	-	added RGB666 handling
-	TouchGFX 1.3	-	easier/enhanced handling of one common SPI port or two separated SPI port for display and touch sensor<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-	improved handling of dragging over the display in TouchGFX, allowing an easier usage of sliders, wheels, etc.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-	above improvement allowing "full mode" availablility handling widgets and transitions on TouchGFX

---
---

<br>
<br>
<br>
<br>


# ILI9XXX/XPT2046-STM32

La repository "ILI9XXX/XPT2046-STM32" contiene il codice relativo ad un set di funzioni per STM32 per la gestione di display LCD touch da 240x320 basato su ILI9341 e da 320x480 basato su ILI9488.<br>
Sono disponibili 3 modi d'uso della libreria:<br>
-	<b>Gestione Diretta del Display</b>:<br>
	la libreria mettte a disposizione un set di funzioni grafiche piuttosto veloce che gestiscono direttamente il display driver via SPI. Ciò permette una gestione grafica "leggera" (più sotto puoi trovare link a video Youtube che mostrano le prestazioni della libreria)
-	<b>TouchGF.X "button mode"</b>:<br>
	seguendo le istruzioni relative , puoi impostare un progetto TouchGFX ad "impatto ridotto" con un buon mix a good mix equilibrio tra qualità grafica e prestazioni, ma usando principalmente (meglio dire "solo")  "buttons" come widget per la gestione del sensore touch via TouchGFX 
-	<b>TouchGFX "full mode"</b>:<br>
	qui, puoi impostare la libreria per gestire l'intero set di widgets/transitions/animations/etc. messe a disposizione da TouchGFX (...l'intero set... beh, non dimenticare i  "limiti fisici" introdotti dalla scheda display: dimensioni ridotte / senslre touch resistivo / indisponiilità di vsync / porta SPI "semplice")

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
- [(TouchGFX "button mode") Guida per aggiungere la libreria al progetto creato](./3A-TOUCHGFX)
- [(TouchGFX "full mode") Guida per aggiungere la libreria al progetto creato](./3C-TOUCHGFX-FM)
- [Guida per la calibrazione del sensore touch](./4-CALIBRATION)

Video Youtube con le carateristiche e le prestazioni dell'integrazione TouchGFX: https://youtu.be/g1siKaPox88<br>
Video Youtube su come impostare un progetto TouchGFX "full mode": https://youtu.be/bG6XCDyCNzo<br>
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

## Soluzione a qualche problema
-	(gestione diretta e TouchGFX)<br>
	Display funziona correttamente in polling mode and interrupt mode ma non in DMA mode:<br> 
	https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/issues/2

-	(gestione diretta e TouchGFX)<br>
	La grafica del display è corretta ma non risponde al tocco:<br> 
	https://youtu.be/KS5u2-6yruU?t=251
	
-	(solo in TouchGFX)<br>
	Display non sincrono:<br> 
	https://youtu.be/KS5u2-6yruU?t=810

-	(gestione diretta e TouchGFX) (libreria v. 1.2)<br>
	Usare la libreria con un H7:<br> 
	https://github.com/maudeve-it/ILI9XXX-XPT2046-STM32/issues/3
	

---

## rev. history

-	1.0		-	prima release
-	TouchGFX 1.0	-	aggiunta integrazione a TouchGFX
-	TouchGFX 1.1	-	rimozione bug
-	TouchGFX 1.2	-	aggiunta gestione RGB666
-	TouchGFX 1.3	-	gestione semplificata/migliorata di una porta SPI comune o due porte distinte per il  display ed il sensore touch<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-	migliorata la gestione del "movimento" del tocco sul display in TouchGFX, permettendo una gestione facilitata di sliders, wheels, ecc.<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-	il miglioramento dopra consente la gestione "completa" di widgets e transizioni su TouchGFX

---
---



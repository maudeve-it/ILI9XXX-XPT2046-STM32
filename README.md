_**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**Below English text you'll find the Italian version</i>**_

<br>
<br>

# ILI9XXX/XPT2046-STM32

"ILI9XXX/XPT2046-STM32" repository contains the source code for a set of functions for an STM32 microcontroller, handling ILI9341 240x320 and ILI9488 320x480 touch display.<br>
<br>
Developed software is based on HAL over STM32CubeIDE.<br>
Software has been tested over<br>

STM32F411CE (Weact Studio board): https://github.com/WeActTC/MiniSTM32F4x1<br>
STM32G432KB (Nucleo32 STM board): https://www.st.com/en/evaluation-tools/nucleo-g431kb.html<br>
STM32G071RB (Nucleo64 STM board): https://www.st.com/en/evaluation-tools/nucleo-g071rb.html<br>
Displays used:<br>
2.4 inch touch TFT-LCD 240x320:  [Aliexpress](https://it.aliexpress.com/item/32832952750.html)<br>
                                 [Amazon](https://www.amazon.it/Hiletgo%C2%AE-320-risoluzione-Touch-panel-ILI9341/dp/B0798N2HHW/ref=sr_1_3_sspa)<br>
3.5 inch touch TFT-LCD 320x480:  [Aliexpress](https://www.aliexpress.com/item/1005003518881469.html)<br>
				<a href="https://www.amazon.it/LOLOVI-pollice-480x320-Seriale-Digitale/dp/B09FJYX7YQ/ref=sr_1_5" target="_blank" >Amazon</a>
				
---

## Project Folders

- [Software folder](./single%2Bdual%20SPI%20port)
- ["How to" add it to a CubeIDE project](./HOWTO)
- ["How to" handle display backlight](./BACKLIGHT)
- ["How to" handle a simple menu](./MENU)

Youtube video showing project characteristics and performance: https://youtu.be/oWx1-WmTwag<br>
Youtube video analyzing SPI communication to displays: https://youtu.be/oWx1-WmTwag<br>


---
> Please Note:
> The software, schemes and PCB designs in this set of repositories are provided just for 
> illustrative purposes, sharing the personal experience on this project. 
> The software and design artifacts are provided as-is without any mantainence and without
> warranty of any kind. There is no guarantee of fitness for any particular application, 
> and no guarantee of completeness and correctness. 
> By downloading and using these materials the user accepts all risks associated with the
> construction and use of the circuits and devices described.<br>
> Fonts licence: fonts are provided by STMicroelectronics. Licencing inside the fonts ".c" files. 
---

## NOTES

20220504: I am still working on these repositories. Consider them uncomplete until this note is deleted.

---

<br>
<br>

# ST7735S-STM32

La repository "ILI9XXX/XPT2046-STM32" contiene il codice relativi ad un set di funzioni per STM32 per la gestione di un display LCD a colori da 1,8 pollici controllato dal driver SPI ST7735S.

Il sowtware sviluppato si basa su HAL attraverso STM32CubeIDE.
|Il software è stato testato su||<br>
|---|---|
|STM32F411CE (scheda Weact Studio)|https://github.com/WeActTC/MiniSTM32F4x1|<br>
|STM32G432KB (scheda STM Nucleo32)|https://www.st.com/en/evaluation-tools/nucleo-g431kb.html|<br>
|STM32G071RB (scheda STM Nucleo64)|https://www.st.com/en/evaluation-tools/nucleo-g071rb.html|<br>
I display utilizzati sono:<br>
|2.4 inch touch TFT-LCD 240x320|https://it.aliexpress.com/item/32832952750.html|<br>
||https://www.amazon.it/Hiletgo%C2%AE-320-risoluzione-Touch-panel-ILI9341/dp/B0798N2HHW/ref=sr_1_3_sspa|<br>
|3.5 inch touch TFT-LCD 320x480|https://www.aliexpress.com/item/1005003518881469.html|<br>
| |https://www.amazon.it/LOLOVI-pollice-480x320-Seriale-Digitale/dp/B09FJYX7YQ/ref=sr_1_5|<br>

---

## Cartelle del progetto 

- [Il Codice è qui](./single%2Bdual%20SPI%20port)
- [Guida per integrare il software in un progetto con CubeIDE](./HOWTO)
- [Guida alla gestione della retroilluminazione](./BACKLIGHT)
- [Guida alla gestione di un semplice menu](./MENU)

Video Youtube con le carateristiche e le prestazioni del progetto: https://youtu.be/oWx1-WmTwag<br>
Video Youtube con l'analisi della comunicazione SPI con i display: https://youtu.be/oWx1-WmTwag<br>

---
> Il software e gli schemi di progetto come i layout PCB in questa serie di repository 
> sono messe a disposizione con puro intento illustrativo e di condivisione dell'esperienza fatta
> Il software and gli elementi del progetto sono messi a disposizione "allo stato in cui sono"
> senza impegno di manutenzione e senza garanzie di alcun tipo. Piu' esplicitamente, non c'e' garanzia di 
> adeguatezza a soddisfare specifiche esigenze, ne di completezza o correttezza di alcuna parte.
> Scaricando ed utilizzando questo materiale l'utilizzatore accetta il rischio associato alla
> realizzazione ed all'utilizzo del circuito e dei componenti descritti in questi file.<br> La serie di font usara è fornita da STMicroelectronics. Licenza d'uso all'interno dei file ".c" dei font. 

---

## NOTE

20220504: Sto ancora aggiornando le repository. Considerale incomplete finche' non rimuovero' questa nota. 

---



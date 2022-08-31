_**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**Below English text you'll find the Italian version</i>**_

<br>
<br>

# Handling backlight

this library can handle display backlight in two different modes:
- as a switched on-off light
- as a dimming backlight.

Backlight mode is set by the macro #define "DISPLAY_DIMMER_MODE" in "z_displ_ILI9XXX.h" file
- If this #define is commented backight is: <b>switching on-off backlight</b>
- Uncommenting the macro #define, backlight is: <b>dimming backlight</b> 
<br>

# ON-OFF mode Backlight
("#define DISPLAY_DIMMER_MODE" commented in "z_displ_ILI9XXX.h" file)
### setup a handling pin on CubeMX
Define a GPIO pin with this configuration:
|pinname to assign|output level|speed relevance|mode|pull-up/down|
|---|---|---|---|---|
|DISPL_LED|low|-|Output push pull|No pull-up/down|

connect the LED pin of the display to DISPL_LED<br>
<br>
That's all. Function "Displ_BackLight" provides these commands (function parameter):
|parameter|description|
|---|---|
|'0'|display off|
|'1'|display on|
|'F'|display on (as same as comand '1')|
|'Q'|no action|

"Displ_BackLight" function returns display status (0=off, 1 = on) so: command 'Q' is to query display status<br>

# Dimming mode Backlight
("#define DISPLAY_DIMMER_MODE" uncommented in "z_displ_ILI9XXX.h" file)<br>
Function "Displ_BackLight" handle backlight in PWM.
<br>

### setup a handling pin on CubeMX
Define a PWM pin:<br>
|pinname to assign|output level|speed relevance|mode|pull-up/down|
|---|---|---|---|---|
|DISPL_LED|low|-|Alternate Function (Timer)|No pull-up/down|
-	enable a PWM channel on a "general purpose" timer (e.g. CH1 on TIM3)<br>
-	setup channel as "PWM mode 1" and "Counter mode UP"<br>
ARR register ("Auto Reload Register" or "Counter period" on CubeMX) defines the number of steps of display light. E.g.: set it to 10 to get 10 light steps available (from 1=v to 10="10/10", and level 0="off")<br>
PSC register (prescaler) higher value is better (reducing power consumption and EMI noise, see STM [GPIO software guidelines](https://www.st.com/resource/en/application_note/an4899-stm32-microcontroller-gpio-hardware-settings-and-lowpower-consumption-stmicroelectronics.pdf)) but must be not too high: ((uC clock / PSC)/ ARR) > 100 Hz, avoiding flickering<br>

### setup z_displ_ILI9XXX.h
align to CubeMX macro parameters:<br>
#define BKLIT_TIMER 				put the timer name (es. TIM3)<br>
#define bklit_t 					put the timer name (es. htim3)<br>
#define BKLIT_CHANNEL				put the used channel (es. TIM_CHANNEL_2)<br>
#define BKLIT_CCR					     indicate the preload register involved by PWM (e.g. CCR2)<br>
<br>
setup parameters:<br>
#define BKLIT_STBY_LEVEL 			set standby level (between 0 and ARR)<br>
you could invoke function with "standby" setting backlight to this predefined value<br>
#define BKLIT_INIT_LEVEL 			set startup level (between 0 and ARR)<br>
This is the backlight level set on initialization<br>
<br>

Function "Displ_BackLight" provides these commands (function parameter):
|parameter|description|
|---|---|
|'0'|display off|
|'1'|display to highest level (=ARR)|
|'F'|display to highest level (as same as comand '1')|
|'I'|initial display setup (mandatory on startup!)|
|'S'|display to standby level|
|'W'|display to the level before last standby (wakeup from standby)|
|'+'|1 step level increase|
|'-'|1 step level decrease|
|'Q'|no action|

"Displ_BackLight" function returns display status (current level) so: command 'Q' is to query display status<br>
PLEASE NOTE: if dimming, run Displ_BackLight('I') on startup! It will also start the timer clock.<br>

---

<br>
<br>

# Gestire la retroilluminazione

questa libreria permette di gestire la retroilluminazione del display in due modi:<br>
- con un interruttore acceso/spento<br>
- con regolazione di luminosità variabile<br>
<br>
Il modo di illuminazione è definito dalla macro define "DISPLAY_DIMMER_MODE" nel file "z_displ_ILI9XXX.h".<br>
- Se la definizione è commentata la retroilluminazione è gestita in modalità acceso/spento<br>
- Togliendo il commento a inizio riga si abilita la gestione con illuminazione regolabile <br>
<br>

# Modalità acceso/spento
("con la riga #define DISPLAY_DIMMER_MODE" commentata nel file "z_displ_ILI9XXX.h")

### configura su CubeMX
Definire un pin GPIO con questa configurazione:
|Nome pin da assegnare|output level|rilevanza velocità|mode|pull-up/down|
|---|---|---|---|---|
|DISPL_LED|low|-|Output push pull|No pull-up/down|

Connettere il pin LED del display a DISPL_LED<br>
Tutto qui. La funzione "Displ_BackLight" mette a disposizione i comandi (parametro passato alla funzione):
|parametro|descrizione|
|---|---|
|'0'|spegne il display|
|'1'|accende  il display|
|'F'|accende il display (come comando '1')|
|'Q'|nessuna azione|

La funzione "Displ_BackLight" restituisce sempre lo stato del display (0=spento, 1 = acceso), il comando 'Q' è usato quindi per ottenere lo stato del display.
<br>

# Modalità "dimmer"
("se viene rimosso il commento a #define DISPLAY_DIMMER_MODE" nel file "z_displ_ILI9XXX.h")<br>
la funzione "Displ_BackLight" gestisce la retroilluminazione controllata in PWM.<br>
<br>
### configura su CubeMX
un pin PWM:<br>
|Nome pin da assegnare|output level|rilevanza velocità|mode|pull-up/down|
|---|---|---|---|---|
|DISPL_LED|low|-|Alternate Function (Timer)|No pull-up/down|

-	attivare un canale PWM di timer "general purpose" (es. CH1 su TIM3)<br>
-	il canale deve essere configurato in "PWM mode 1" e "Counter mode UP"<br>
il registro ARR ("Auto Reload Register", o "Counter period" su CubeMX) definisce il numero di livelli di luminosità del display. Es: impostare a 10 per avere 10 livelli di luminosità disponibili (da 1="1/10 duty cycle" a 10="10/10", piu' il livello 0="display spento")<br>
Maggiore è il valore del registro PSC (prescaler) e minori sono i consumi ed EMI vedi le [GPIO software guidelines](https://www.st.com/resource/en/application_note/an4899-stm32-microcontroller-gpio-hardware-settings-and-lowpower-consumption-stmicroelectronics.pdf)) di STM, ma il valore non deve essere troppo elevato in modo che sia ((clock uC / PSC)/ ARR) > 100 Hz per evitare problemi di flickering<br>

### configura z_displ_ILI9XXX.h

allineare i parametri:<br>
#define BKLIT_TIMER 				indicare il timer usato (es. TIM3)<br>
#define bklit_t 					indicare il timer usato (es. htim3)<br>
#define BKLIT_CHANNEL				indicare il canale usato (es. TIM_CHANNEL_2)<br>
#define BKLIT_CCR					      definire il preload register coinvolto da PWM<br>
<br>
definire i parametri:<br>
#define BKLIT_STBY_LEVEL 			indicare il livello di standby (tra 0 ed ARR)<br>
#define BKLIT_INIT_LEVEL 			indicare il livello di assegnare alla accensione (tra 0 ed ARR)<br>
<br>
La funzione "Displ_BackLight" mette a disposizione i comandi (parametro passato alla funzione):
|parametro|descrizione|
|---|---|
|'0'|spegne il display (livello 0)|
|'1'|display al massimo (livello ARR)|
|'F'|display al massimo (come comando '1')|
|'I'|setup iniziale del display (obbligatorio allo startup!)|
|'S'|display al valore di standby|
|'W'|display al valore precedente all'ultimo standby (risveglio da standby)|
|'+'|aumenta la luminosità di 1 livello|
|'-'|diminuisce la luminosità di 1 livello|
|'Q'|nessuna azione|

La funzione "Displ_BackLight" restituisce sempre lo stato del display (livello di luminosità), il comando 'Q' è usato quindi per interrogare sullo stato del display.<br>

NOTA BENE: se in dimming mode, si deve eseguire Displ_Backlight('I') in fase di startup. Verrà avviato anche il clock del timer
<br>
 




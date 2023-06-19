
_**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**Below English text you'll find the Italian version</i>**_

<br>

# Direct Display Handling Mode


## So...

Now that you have created a new CubeIDE project as per these instructions:

- ["How to" create a CubeIDE project for this library](../1-HOWTO)

Now that you have chosen the backlight handling mode and set it as per these instructions:

- ["How to" handle. display backlight](../2-BACKLIGHT)


You finally can setup the last program parameters and run it!<br>
Follow the below instructions if you like directly handling the display through the graphics functions in the library.<br>
Otherwise:
- if you like handling the display through TouchGFX using only buttons as input widgets, having  a minor impact on the overall performances, go to [(TouchGFX button mode) "How to" add this library to the created project](../3A-TOUCHGFX)<br>
- if you want/need to use all TouchGFX widgets (with the limitations imposed by these displays) go to [(TouchGFX full mode) "How to" add this library to the created project](../3C-TOUCHGFX-FM)<br>


## Parameters setup
you must "tune" the software parameters in two files:<br>
- z_displ_ILI9XXX.h<br>
- z_touch_XPT2046.h<br>

into "z_displ_ILI9XXX.h" file you have to setup this configuration:
- step 1 - TouchGFX parameter: ok, if you are here reading, it means you have to add a comment to parameter in section 0, disabling TouchGFX handling
- step 2 - define the display you are using (ILI9346 or ILI9488 V1.0 or V2.0) uncommenting the related #define
- step 3 - Port Parameters: here you have to set two macro constant with the SPI port name connecting display (see below "z_touch_XPT2046.h" also)
- step 4 - Port Speed: here you must assign bitrate SPI prescaler when transferring data to display or to/from touch sensor. Consider that touch uses baudrates  below 2MBps 
- step 5 - SPI communication mode: uncomment the macro definition related to enabled communication mode (Polling mode, Interrupt mode or DMA mode). You must uncomment no less and no more than ONE definition here
- step 6 - Backlight Mode: see [BACKLIGHT page](../BACKLIGHT)
- step 7 - TouchGFX Time base timer: unused only in "TouchGFX Full Mode", in "Direct Mode" be sure that these parameters refer to an unused timer
- step 8 - Buffer size: that's defining size of buffers storing data to display. It must be not below 10. See [Youtube video](https://youtu.be/oWx1-WmTwag) for information and comments about buffer size.

into "z_touch_XPT2046.h" file you have to setup this configuration:
- step 1 - Port Parameters: here you have to set two macro constant with the SPI port name connecting touch sensor
- step 2 - Delay to Key Repeat: this parameter is used only in "TouchGFX Button Mode". In Direct Mode any value set is meaningless.


## main.c setup

in main.c you now need to initialize display before entering in the main loop, this way:<br>
<br>
  (main.c)
  ```sh
  ...
  /* USER CODE BEGIN 2 */			// "USER CODE BEGIN 2" is after all system initializations and before entering the main loop. Good place to initialize display
  Displ_Init(Displ_Orientat_0);		// initialize the display and set the initial display orientation (here is orientaton: 0°) - THIS FUNCTION MUST PRECEED ANY OTHER DISPLAY FUNCTION CALL.  
  Displ_CLS(BLACK);			// after initialization (above) and before turning on backlight (below), you can draw the initial display appearance. (here I'm just clearing display with a black background)
  Displ_BackLight('I');  			// initialize backlight and turn it on at init level
  /* USER CODE END 2 */
  ...
  ```
## running test functions

Here some test function you can run copying the "_test" files into the project.
You can use them to:
-	check the correct system installation and configuration
-	learn how to use the library functions

Below, three demo/test functions put in the same main loop, but I suggest you to test them one by one, following the below order.

  ```sh
  ...
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) 			// into the main loop you can add the test functions
  {
	// display testing
	Displ_PerfTest();		// shows display graphics and performance
	// touch testing
	Touch_ShowData();		// polls touch device and shows values returned, showing touch interrupt calls also
	Touch_TestDrawing();		// polling touch, converting and drawing position returned
  ...
  ```

---


## using library functions in your projects

Passing the test, display and the library are fully integrated in your software.
Now you can use the library as per your needs:


GRAPHICS

With a short look to the <i><b>Displ_PerfTest()</b></i> function you should find all the graphics functions you need. 
A short description available in the source file should help usage while developing.
There is not much more to say.


TOUCH

There are many way to use touch device with the library functions.
The most performing way is:
-	first of all check if there was a touch: <i><b>Touch_GotATouch()</b></i> returns 1 if interrupt registered a touch. This is a fast function not polling sensor, non involving SPI nor DMA.
-	if touch detected, use (depending of your needs) either <i><b>Touch_GetXYtouch()</b></i> (returning a struct with X and Y position of touch converted to the grapic display coordinates) or <i><b>Touch_In_XY_area(xpos, ypos, width, height)</b></i> (returning 1 if touch inside the area indicated in parameters)

There are two more functions:
-	<i><b>Touch_WaitForTouch(delay)</b></i>
-	<i><b>Touch_WaitForUntouch(delay)</b></i>
<br>
hanging program until event in the function name or <i>delay</i> expired



## touch sensor calibration

If you see that graphics works fine over the display but there is a bad reply (or no reply) touching sensor there could be a problem of calibration.
Check that here: ["How to" calibrate touch sensor](../4-CALIBRATION)




<br>
<br>

---

# Gestione Diretta del Display


## Dunque...
Ora che hai creato un nuovo progetto con CubeIDE project seguendo queste istruzioni:

- [Guida per configurare un progetto con CubeIDE](./1-HOWTO)

Ora che hai scelto la gestione della retroilluminazione, seguendo queste istruzioni:

- [Guida alla gestione della retroilluminazione](./2-BACKLIGHT)

You finally can setup the last program parameters and run it!<br>
Follow the below instructions if you like directly handling the display through the graphics functions in the library.
Otherwise:
- if you like handling the display through TouchGFX using only buttons as input widgets, having  a minor impact on the overall performances, go to [(TouchGFX button mode) "How to" add this library to the created project](../3A-TOUCHGFX)<br>
- if you want/need to use all TouchGFX widgets (with the limitations imposed by these displays) go to [(TouchGFX full mode) "How to" add this library to the created project](../3C-TOUCHGFX-FM)<br>


Puoi finalmente configurare gli ultimi parametri ed eseguire il programma!<br>
Segui le istruzioni sotto se vuoi gestire direttamente il display attraverso le funzioni grafiche della libreria.<br>
Altrimenti:<br>
- se vuoi gestire il display attraverso TouchGFX usando solo bottoni come widgets di ingresso, ottenendo un minore impatto sulle prestazioni complessive, passa a: [(TouchGFX button mode) "How to" add this library to the created project](../3A-TOUCHGFX)<br>
- se vuoi/devi utilizzare tutti i widgets TouchGFX (con le limitazioni imposte da questi display) passa a: [(TouchGFX full mode) "How to" add this library to the created project](../3C-TOUCHGFX-FM)<br>


## Configurazione Parametri
Devi "mettere a punto" i parametri del software in due file:
- z_displ_ILI9XXX.h
- z_touch_XPT2046.h

in "z_displ_ILI9XXX.h" devi impostare questa configurazione:
- step 1 - TouchGFX: se stai leggendo qui devi commentare la macro nella sezione 0, disabilitando la gestione TouchGFX  
- step 2 - Definisci il display che stai usando (ILI9346 or ILI9488 V1.0 or V2.0) togliendo ail commento al #define relativo
- step 3 - Port Parameters: qui devi impostare due costanti macro con il nome della porta SPI a cui è connesso il display (vedi anche "z_touch_XPT2046.h", sotto)
- step 4 - Port Speed: qui devi assegnare il valore al bitrate SPI prescaler quando trasferisce dati al display o al/dal sensore touch. Considera che il sensore touch comunica sotto i 2MBps 
- step 5 - SPI communication mode: togli il commento alla definizione definition relativa al tipo di comunicazione attivato (Polling mode, Interrupt mode o DMA mode). Devi togliere il commento ad una riga (NON di più e NON di meno) 
- step 6 - Backlight Mode: vedi la [pagina BACKLIGHT](../BACKLIGHT)  
- step 7 - Timer per la temporizzazione di TouchGFX: usato solo in "TouchGFX Full Mode", in Direct Mode assicurati che questo parametro non sia associato ad un timer in uso
- step 8 - Buffer size: definisci il "Buffer Level" che preferisci (assegna un valore tra 10 e 16) vedi [il video youtube](https://youtu.be/oWx1-WmTwag) per informazioni e commenti sulle sue dimensioni.  

in "z_touch_XPT2046.h" devi impostare i seguenti parametri:
- step 1 - Port Parameters: qui devi impostare due costanti macro con il nome della porta SPI a cui è connesso il display
- step 2 - Delay to Key Repeat: questo parametro è utilizzato solo in "TouchGFX Button Mode". Qualunque valore impostato non e' significativo qui



## impostazione di main.c 

in main.c ora devi inizializzare il display prima di entrare nel main loop, in questo modo:

  (main.c)
  ```sh
  ...
  /* USER CODE BEGIN 2 */			// "USER CODE BEGIN 2" viene dopo tutte le inizializzazioni del sistema e prima del main loop
  Displ_Init(Displ_Orientat_0);		// inizializza il display ed imposta l'orientamento iniziale - QUESTA FUNZIONE DEVE PRECEDERE OGNI ALTRA FUNZIONE DELLA LIBRERIA
  Displ_CLS(BLACK);				// dopo l'inizializzazione (sopra) e prima di accendere la retroilluminazione (sotto), puoi impostare la schermata iniziale. (qui semplicemente cancello lo schermo con uno sfondo nero) 
  Displ_BackLight('I');  			// inizializza la retroilluminazione e la accende al livello init 
  /* USER CODE END 2 */
  ...
  ```

## eseguire le funzioni di test

Qui sotto alcune funzioni di test disponibili installando i "_test" file.
Puoi usarle per:
-	verificare la corretta installazione e configurazione del sistema
-	imparare come usare le funzioni della libreria

Qui mostro tre funzioni di test chiamate nello stesso main loop, ma suggerisco di provarle una per volta, seguendo lìordine sotto.

  ```sh
  ...
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) 			// nel main loop puoi aggiungere le funzioni di test
  {
	// display testing
	Displ_PerfTest();		// mostra la grafica e le performance del display
	// touch testing
	Touch_ShowData();		// interroga il touch device e mostra i valori restituiti, mostra anche le chiamate ad interrupt fatte dal display
	Touch_TestDrawing();		// interroga il display, converte i valori restituiti e disegna la posizione ricevuta
  ...
  ```

---


## usare la libreria nei tuoi progetti

Superando i test hai la prova che il display e la libreria sono pienamente integrati nel progetto.
Ora puoi usare la libreria secondo le necessità:


GRAFICA

Con una occhiara alla funzione <i><b>Displ_PerfTest()</b></i> dovresti individuare tutte le funzioni grafiche che ti occorrono. 
Una breve descrizione disponibile nel file sorgente dovrebbe aiutare durante lo sviluppo.
Non c'è molto altro da dire.


TOUCH

Ci sono molti modi per usare il sensore touch con la libreria.
Le maggiori prestazioni si hanno così:
-	Prima di tutto verifica se è stato rilevato un tocco: <i><b>Touch_GotATouch()</b></i> restituisce 1 se interrupt ha registrato un tocco. Questa è una funzione veloce che non interroga il sensore e non coinvolge SPI nè DMA.
-	Se è stato rilevato un tocco usa (secondo le necessità) o <i><b>Touch_GetXYtouch()</b></i> (che restituisce una struct con le coordinate X,Y del tocco convertite nelle coordinate grafiche) oppure <i><b>Touch_In_XY_area(xpos, ypos, width, height)</b></i> (che restituisce 1 se rileva il tocco all0interno dell'area indicato dai parametri)

Ci sono alrtre due funzioni:
-	<i><b>Touch_WaitForTouch(delay)</b></i>
-	<i><b>Touch_WaitForUntouch(delay)</b></i>
<br>
che arrestano il programma fino all'evento indicato dal nome della funzione oppure allo scadere del <i>delay</i>.



## calibrazione del sensore touch

Se verifichi the la parte grafica del progetto funziona correttamente ma non ottieni risposta dal sensore touchm ci potrebbe essere la necessità di calibrarlo.
Puoi seguire queste indicazioni: [Guida per la calibrazione del sensore touch](../4-CALIBRATION)

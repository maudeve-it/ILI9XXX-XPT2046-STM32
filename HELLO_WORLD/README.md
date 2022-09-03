_**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**Below English text you'll find the Italian version</i>**_

<br>

## So...

Now that you have created a new CubeIDE project as per these instructions:

- ["How to" create a CubeIDE project for this library](./HOWTO)

Now that you have added the library source and header files to the project as per these instructions:

- ["How to" add this software to the project](./SOURCE)

Now that you have chosen the backlight handling mode and set it as per these instructions:

- ["How to" handle display backlight](./BACKLIGHT)




You finally can setup the last program parameters and run it!<br>



##### Parameters setup
you must "tune" the software parameters in two files:<br>
- z_displ_ILI9XXX.h<br>
- z_touch_XPT2046.h<br>

into "z_displ_ILI9XXX.h" file you have to setup this configuration:
- section 1 - define the display you are using (ILI9346 or ILI9488 V1.0 or V2.0) uncommenting the related #define
- section 2 - Port Parameters: here you have to set two macro constant with the SPI port name connecting display (see below "z_touch_XPT2046.h" also)
- section 3 - Port Speed: here you must assign bitrate SPI prescaler when transferring data to display or to/from touch sensor. Consider that touch uses baudrates  below 2MBps 
- section 4 - SPI communication mode: uncomment the macro definition related to enabled communication mode (Polling mode, Interrupt mode or DMA mode). You must uncomment no less and no more than ONE definition here
- section 5 - Backlight Mode: see [BACKLIGHT page](../BACKLIGHT)  
- section 6 - Buffer size: define the "Buffer Level" you like (assign a value between 10 and 16 included) see [this youtube video](https://youtu.be/oWx1-WmTwag) for information and comments about its sizes available.  

into "z_touch_XPT2046.h" file you have to setup this configuration:
- section 1 - Port Parameters: here you have to set two macro constant with the SPI port name connecting touch sensor

---


##### main.c setup

in main.c you now need to initialize display before entering in the main loop, this way:<br>
<br>
  (main.c)
  ```sh
  ...
  /* USER CODE BEGIN 2 */			// "USER CODE BEGIN 2" is after all sistem initializations and before entering the main loop. Good place to initialize display
  Displ_Init(Displ_Orientat_0);		// initialize the display and set the initial display orientation - THIS FUNCTION MUST PRECEED ANY OTHER DISPLAY FUNCTION CALLS
  Displ_CLS(BLACK);			// after initialization (above) and before turning on backlight (below), you can draw the initial display appearance. (here I'm just clearing display with a black background)
  Displ_BackLight('I');  			// initialize backlight and turn it on at init level
  /* USER CODE END 2 */
  ...
  ```

##### running test functions

Here some test function you can run copying the "_test" files into the project.
You can use them to:
-	check the correct system installation and configuration
-	learn how to use the library functions

Here I show three demo/test functions called in the same main loop, but I suggest you to test them one by one, following the below order.

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

<br>
<br>


## Dunque...
Ora che hai creato un nuovo progetto con CubeIDE project seguendo queste istruzioni:

- [Guida per configurare un progetto con CubeIDE](./HOWTO)

Ora che hai aggiunto i source e header files al progetto, seguendo queste istruzioni:

- [Guida per integrare il software](./SOURCE)

Ora che hai scelto la gestione della retroilluminazione, seguendo queste istruzioni:

- [Guida alla gestione della retroilluminazione](./BACKLIGHT)

Puoi finalmente configurare gli ultimi parametri ed eseguire il programma!



##### Impostazione parametri
Devi "mettere a punto" i parametri del software in due file:
- z_displ_ILI9XXX.h
- z_touch_XPT2046.h

in "z_displ_ILI9XXX.h" devi impostare questa configurazione:
- sezione 1 - Definisci il display che stai usando (ILI9346 or ILI9488 V1.0 or V2.0) togliendo ail commento al #define relativo
- sezione 2 - Port Parameters: qui devi impostare due costanti macro con il nome della porta SPI a cui è connesso il display (vedi anche "z_touch_XPT2046.h", sotto)
- sezione 3 - Port Speed: qui devi assegnare il valore al bitrate SPI prescaler quando trasferisce dati al display o al/dal sensore touch. Considera che il sensore touch comunica sotto i 2MBps 
- sezione 4 - SPI communication mode: togli il commento alla definizione definition relativa al tipo di comunicazione attivato (Polling mode, Interrupt mode o DMA mode). Devi togliere il commento ad una riga (NON di più e NON di meno) 
- sezione 5 - Backlight Mode: vedi la [pagina BACKLIGHT](../BACKLIGHT)  
- sezione 6 - Buffer size: definisci il "Buffer Level" che preferisci (assegna un valore tra 10 e 16 included) vedi [il video youtube](https://youtu.be/oWx1-WmTwag) per informazioni e commenti sulle sue dimensioni.  

in "z_touch_XPT2046.h" devi impostare i seguenti parametri:
- sezione 1 - Port Parameters: qui devi impostare due costanti macro con il nome della porta SPI a cui è connesso il display 


---


##### impostazione di main.c 

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

##### eseguire le funzioni di test

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

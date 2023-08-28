_**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**Below English text you'll find the Italian version</i>**_

<br>


# So...

Now that you have created a new CubeIDE project as per these instructions:

- ["How to" create a CubeIDE project for this library](../1-HOWTO)

Now that you have chosen the backlight handling mode and set it as per these instructions:

- ["How to" handle display backlight](../2-BACKLIGHT)


You are ready to go integrating TouchGFX!<br>
Follow the below instructions if you like handling the display through this GUI systems, AND you want/need handling all TouchGFX widgets (with the limitations imposed by these displays).<br>
Otherwise:
- if you want to directly draw on the display through the library functions, do not proceed here and go to [(Direct handling) "How to" add this library to the created project](../3B-DIRECT)<br>
- if you like handling the display through TouchGFX using only buttons as input widgets, having  a minor impact on the overall performances, go to [(TouchGFX button mode) "How to" add this library to the created project](../3A-TOUCHGFX)<br>
<br>
<br>

## Warnings
When I say that "all TouchGFX widgets are available", I don't mean that all widgets/transitions will give good results. I'm repeating, one more time here, that you must take in consideration limits imposed by the hw you are using: a small display, with a resistive touch sensor, not returning vsync to the uC, communicating via a single SPI port.<br>
TouchGFX provides a rich set of features (and much more can be developed by you), they cover a wide range of displays and uC, and all of that features are available for the usage.<br>
But if I'm not working with an OctoSPI or at least a Chrome-art hw support, I wuould not use a video widget on my projects. <br>
And I never used also the keyboard widget on these displays: that's not an iPhone display. <br>
On the other hand, fortunately, Crome-art is not a mandatory feature to come to good projects with a good UI.<br>
<br>

## Parameters setup
you must "tune" the software parameters in two files:<br>
- z_displ_ILI9XXX.h<br>
- z_touch_XPT2046.h<br>

into "z_displ_ILI9XXX.h" file you have to setup this configuration:
- step 1 - TouchGFX parameter: ok, if you are here reading, it means you have to remove the comment sign to parameter in section 0, enabling TouchGFX handling
- step 2 - define the display you are using (ILI9346 or ILI9488 V1.0 or V2.0) uncommenting the related #define
- step 3 - Port Parameters: here you have to set two macro constant with the SPI port name connecting display (see below "z_touch_XPT2046.h" also)
- step 4 - Port Speed: here you must assign bitrate SPI prescaler when transferring data to display or to/from touch sensor. Consider that touch uses baudrates  below 2MBps 
- step 5 - SPI communication mode: uncomment the macro definition related to enabled communication mode (Polling mode, Interrupt mode or DMA mode). You must uncomment no less and no more than ONE definition here
- step 6 - Backlight Mode: see [BACKLIGHT page](../BACKLIGHT)
- step 7 - TouchGFX Time base timer: you have to specify here the timer you set with a loop of 1/60 s handling touchgfxSignalVsync()
- step 8 - Buffer size: Used converting from RGB565 to RGB666 with ILI9488 V1.0 displays. Follow instructions into .h file  

into "z_touch_XPT2046.h" file you have to setup this configuration:
- step 1 - Port Parameters: here you have to set two macro constant with the SPI port name connecting touch sensor
- step 2 - Delay to Key Repeat: that must be set to -1

<br><br>

## TouchGFX package setup

- enter in uC configuration (CubeMX) doubleclicking the ".ioc" file<br>
- goto "Software Packs", then to "Select components" and install "TouchGFX" if not yet done in your CubeIDE<br>
- after installation click on TouchGFX details and select it<br>
- close the Software packs component selector<br>
- goto Computing->CRC and activate it<br>
- goto Software Packs->TouchGFX and activate it<br>

now in TouchGFX generator, set these parameters:
|Parameter|Value|
|---|---|
|Framebuffer PixelFormat|RGB565 <b>(*)</b>|
|Width|<set display size>|   
|Height|<set display size>|
|Framebuffer Strategy|Partial Buffer|
all other parameters as per default<br>

Save and generate (or update) software.<br>

> <br><em><b>(*)</b> <br>even if you are using a display needing RGB666 (e.g. ILI9488 board V1.0), set TouchGFX to RGB565.<br>
> TouchGFX doesn't work with RGB666 and will send frames in RGB565: library will make convertion from RBG565 to RGB666 needed by the display.<br></em><br> 


<br><br>

## Touch sensor integration

Open TouchGFX->target folder in your project.<br>
Open STM32TouchController.cpp inside this folder.<br>
add the include:<br>
<br>
  (STM32TouchController.cpp)
  ```sh
  ...
  #include "main.h"
  ...
  ```

<br>
In the same file change sampleTouch() as shown here:<br>
<br>

  (STM32TouchController.cpp)
  ```sh
  ...
bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
	return ((bool) Touch_TouchGFXSampleTouch(&x, &y));
}
  ...
  ```

<br><br>




## main.c setup

in main.c you now need to initialize display before entering in the main loop, this way:<br>
<br>
  (main.c)
  ```sh
  ...
  /* USER CODE BEGIN 2 */			// "USER CODE BEGIN 2" is after all system initializations and before entering the main loop. Good place to initialize display
  Displ_Init(Displ_Orientat_90);		// initialize display controller - set orientation parameter as per TouchGFX setup
  Displ_BackLight('I');  			// initialize backlight
  HAL_TIM_Base_Start_IT(&TGFX_T);		// starting timer driving touchgfxSignalVSync()
  /* USER CODE END 2 */
  ...
  ```

<br>

> <br><em>PLEASE NOTE:<br> set parameter in Displ_Init() function as per orientation of your display graphics.<br></em><br>

<br>
In the main loop, you do not need ad anything related to the library:<br>
<br>

  ```sh
  ...
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) 			// into the main loop you can add the test functions
  {
  
    /* USER CODE END WHILE */

	  MX_TouchGFX_Process();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
...
  ```


<br><br>
	
## using TouchGFX  in your projects

That's all, start using TouchGFX designer to setup your GUI!

<br><br>

## compiler warnings

When you first compile your project, it may stop on these two errors: <br> 

  ```sh
...
#error "A user must call touchgfx::startNewTransfer(); once touchgfxDisplayDriverTransmitBlock() has succesfully sent a block."
#error "A user must implement C-methods touchgfxDisplayDriverTransmitActive() and touchgfxDisplayDriverTransmitBlock() used by the Partial Framebuffer Strategy."
...
  ```

It is just a reminder for you to develop the interface routines to TouchGFX.<br>
They are already available in the library.<br>
So, just delete the two "#error" lines, the two warnings, and re-compile the project.<br>
They will not appear anymore.<br>

<br><br>

## touch sensor calibration

if you see that TouchGFX graphics works fine over the display but there is no reply touching sensor there could be a problem of calibration.
Check that here: ["How to" calibrate touch sensor](../4-CALIBRATION)




---

# Dunque...
Ora che hai creato un nuovo progetto con CubeIDE project seguendo queste istruzioni:

- [Guida per configurare un progetto con CubeIDE](./1-HOWTO)

Ora che hai scelto la gestione della retroilluminazione, seguendo queste istruzioni:

- [Guida alla gestione della retroilluminazione](./2-BACKLIGHT)

Puoi finalmente configurare gli ultimi parametri ed eseguire il programma!<br>
Segui le istruzioni sotto se vuoi gestire direttamente il display attraverso le funzioni grafiche della libreria.<br>
Se invece vuoi usare il pacchetto STM TouchGFX, non procedere qui e passa a [(gestione diretta) Guida per aggiungere la libreria al progetto creato](./2B-DIRECT)
<br>

## Avvisi
Quando indico che tutti i widget TouchGFX sono disponibili, non intendo dire che tutti danno buoni risultati. Ripeto qui che occorre tenere in considerazione i limiti imposti dall'hardware in uso: display di dimensioni ridotte, sensore touch resistivo, assenza di vsync, comunicazione con una porta SPI semplice.<br>
TouchGFX mette a disposizione un ricco set di strumenti (ed altri possono essere sviluppati) che coprono un ampio spettro di display e uC. E tutti questi strumenti sono disponibili all'uso.<br>
Ma se nel mio progetto non ho a disposizione un OctoSPI ed almeno il supporto hw di Chrome-art, io non userei il widget per i video nei miei progetti. <br>
Ma non userei nemmeno il widget "keyboard" in progetti con i display che consideriamo qui: non sono il display di un iPhone. <br>
D'altraparte, fortunatamente, non e' obbligatorio disporre di Chrome-art per arrivare ad un buon progetto con una buona UI.<br>
<br>

## Configurazione Parametri
Il progetto è quasi pronto. Devi mettere a punti i parametri nel file "z_displ_ST7735.h":
- step 1 - TouchGFX: se stai leggendo qui devi togliere il commento alla la macro nella sezione 0, abilitando la gestione TouchGFX  
- step 2 - Definisci il display che stai usando (ILI9346 or ILI9488 V1.0 or V2.0) togliendo ail commento al #define relativo
- step 3 - Port Parameters: qui devi impostare due costanti macro con il nome della porta SPI a cui è connesso il display (vedi anche "z_touch_XPT2046.h", sotto)
- step 4 - Port Speed: qui devi assegnare il valore al bitrate SPI prescaler quando trasferisce dati al display o al/dal sensore touch. Considera che il sensore touch comunica sotto i 2MBps 
- step 5 - SPI communication mode: togli il commento alla definizione definition relativa al tipo di comunicazione attivato (Polling mode, Interrupt mode o DMA mode). Devi togliere il commento ad una riga (NON di più e NON di meno) 
- step 6 - Backlight Mode: vedi la [pagina BACKLIGHT](../BACKLIGHT)
- step 7 - TouchGFX Time base timer: qui devi indicare il timer che hai impostato con un ciclo di 1/60 s per gestire touchgfxSignalVsync()
- step 8 - Dimensione Buffer: Usati per convertire dal protocollo RGB565 ad RGB666 con ILI9488 V1.0: segui istruzioni nel file .ho 

in "z_touch_XPT2046.h" devi impostare i seguenti parametri:
- step 1 - Port Parameters: qui devi impostare due costanti macro con il nome della porta SPI a cui è connesso il sensore touch<br>
- step 2 - Delay to Key Repeat: DEVE essere impostato a -1
<br><br>



## Configurazione TouchGFX package

- entra nella configurazione del uC (CubeMX) con un doppio click sul file ".ioc"<br>
- vai in "Software Packs", poi in "Select components" e installa "TouchGFX" se non lo hai ancora fatto sul tuo CubeIDE<br>
- dopo l'installazione clicca sulla tendina TouchGFX e selezionalo<br>
- chiudi il Software packs component selector<br>
- vai in Computing->CRC e attivalo<br>
- vai in Software Packs->TouchGFX e attivalo<br>

ora, in TouchGFX generator, imposta questi parametri:
|Parametro|Valore|
|---|---|
|Framebuffer PixelFormat|RGB565 <b>(*)</b>|
|Width|<imposta la dimensione del display>|
|Height|<imposta la dimensione del display>|
|Framebuffer Strategy|Partial Buffer|
tutti gli altri parametri come default<br>

Salva e genera (o aggiorna) il software.<br>


> <br><em><b>(*)</b> <br>anche se stai usando un display che richiede RGB666 (e.g. ILI9488 board V1.0), imposta TouchGFX per RGB565.<br>
> TouchGFX non lavora in RGB666 e manderà i frame in RGB565: la libreria convertirà da RBG565 al formato RGB666 richiesto dal display.<br></em><br> 


<br><br>

## Integrazione del sensore touch 

Apri la cartella TouchGFX->target nel tuo progetto.<br>
Apri STM32TouchController.cpp all'interno di questa cartella.<br>
aggiungi l'include include:<br>
<br>
  (STM32TouchController.cpp)
  ```sh
  ...
  #include "main.h"
  ...
  ```

<br>
Nello stesso file modifica sampleTouch() come sotto:<br>
<br>

  (STM32TouchController.cpp)
  ```sh
  ...
bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
	return ((bool) Touch_TouchGFXSampleTouch(&x, &y));
}
  ...
  ```

<br><br>


## impostazione di main.c 

in main.c ora devi inizializzare il display prima di entrare nel main loop, in questo modo:

  (main.c)
  ```sh
  ...
  /* USER CODE BEGIN 2 */			// "USER CODE BEGIN 2" is after all system initializations and before entering the main loop. Good place to initialize display
  Displ_Init(Displ_Orientat_90);		// initialize display controller - set orientation parameter as per TouchGFX setup
  Displ_BackLight('I');  			// initialize backlight
  HAL_TIM_Base_Start_IT(&TGFX_T);		// starting timer driving touchgfxSignalVSync()
  /* USER CODE END 2 */
  ...
  ```

	
<br>

> <br><em>NOTA BENE: il parametro della funzione Displ_Init() deve corrispondere all'orientamento assegnato della grafica del display.<br></em><br>

<br>
Nel main loop, non è necessario aggiungere niente relativo alla gestione della libreria:<br><br>

  ```sh
  ...
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) 			// into the main loop you can add the test functions
  
  
    /* USER CODE END WHILE */

	  MX_TouchGFX_Process();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
...
  ```
<br>


## usare TouchGFX nel tup progetto

E' tutto. Inizia ad usare TouchGFX designer per impostare la tua GUI!<br>


<br><br>


## avvisi del compilatore

Quando compili il progetto per la prima volta, potrebbe fermarsi su questi due errori:<br> 

  ```sh
...
#error "A user must call touchgfx::startNewTransfer(); once touchgfxDisplayDriverTransmitBlock() has succesfully sent a block."
#error "A user must implement C-methods touchgfxDisplayDriverTransmitActive() and touchgfxDisplayDriverTransmitBlock() used by the Partial Framebuffer Strategy."
...
  ```

Si tratta semplicemente di un avviso per ricordarti di sviluppare le routine di interfaccia a TouchGFX.<br>
Queste sono già disponibili nella libreria.<br>
Quindi, semplicemente cancella/commenta le due righe "#error", i due avvisi, e ricompila il progetto.<br>
Non appariranno più.<br>

<br><br>


## calibrazione del sensore touch

Se verifichi the la parte grafica del progetto funziona correttamente ma non ottieni risposta dal sensore touchm ci potrebbe essere la necessità di calibrarlo.
Puoi seguire queste indicazioni: [Guida per la calibrazione del sensore touch](../4-CALIBRATION)

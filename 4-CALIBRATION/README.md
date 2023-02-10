
_**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**Below English text you'll find the Italian version</i>**_

<br>


# Needing to calibrate sensor

I received some requests to help in fixing up a project that doesn't reply (or have a bad reply) to the touch, 
so I'm adding this page trying to help to set the touch sensor calibration. 

The calibration procedure showed below can be seen also inside this YouTube video: https://youtu.be/KS5u2-6yruU  

## Why needing the touch sensor calibration

These type of displays is made of 2 parts:<br>
- the graphic display handled by an ILI driver (ILI9488, ILI9386 and so on)<br>
- the touch sensor handled by the XPT2084 driver<br>

The touch sensor is a transparent module simply "glued" over the graphic display.<br>
The touch driver reads value returned by the sensor and that values have to be converted into the corresponding graphic positition, matching touch with the display.<br>
The convertion is made by software and the converting parameters could need to change depending on the display/touch-sensor combination


> <br><em>by the way: <br>touch sensor is quite stiff and fragile and it is easy breaking it, especially while prototyping, working with displays simply inserted into the breadboard.<br>
> In that case it is very easy removing the broken sensor, cutting the flex connector and making it a working display wihout the touch sensor.<br></em><br> 


## Checking touch sensor calibration

Create a new project with a "Direct Handling" of the display (even if your final project is based on TouchGFX) following all instruction in: [(Direct handling) "How to" add this library to the created project](../3B-DIRECT), adding also the touch test files (z_touch_XPT2046_test.c and .h).<br> 
In the main loop just run function "TestDrawing" this way:<br>

  ```sh
  ...
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) 			
  {
	Touch_TestDrawing();		
  ...
  ```

Compile and run the project. Touch the sensor and move your finger while touching.<br>
If you don't see a white cross (over a blue background) following your finger, sensor must be calibrated.



## Calibrating touch sensor

To calibrate sensors parameters, change the main loop this way:<br>


  ```sh
  ...
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) 			
  {
	Touch_TestCalibration();
  ...
  ```

and run it.
Follow instruction on the display.
At the end of calibration display will show a calibrated set of parameters for the touch-to-display convertion.

Register the new values into "z_touch_XPT2046.h" file (in the "CALIBRATION PARAMETERS" section)

Test the new cailbration with the Touch_TestDrawing() function above indicated.
You could also try to directly change the calibration parameters for a final fine tuning.

When you are satisfied with the touch/graphics alignement, keep the z_touch_XPT2046.h file for the next projects with your sensor.<br>
<br>
<br>

---

<br>

# La necessità di calibrare il sensore

Ho ricevuto alcune richieste di aiuto nel risolvere il problema di progetti che hanno una cattiva (o nulla) risposta al tocco ed 
ho pensato di aggiungere questa pagina per aiutare a calibrare il sensore touch.

La procedura di calibrazione mostrata sotto può essere vista anche all'interno di questo video YouTube: https://youtu.be/KS5u2-6yruU  


## Perché è necessario calibrare il sensore

Questo tipo di display è composto di due parti:<br>
- il display grafico gestito da un driver ILI (ILI9488, ILI9386 e così via)<br>
- il sensore touch gestito dal driver XPT2084<br>

Il sensore touch e' un modulo trasparente semplicemente "incollato" sopra il display grafico.<br>
Il driver touch legge i valori restituiti dal sensore e quei valori devono essere convertiti nella corrispondente posizione grafica, allineando il sensore touch con il display.<br>
La conversione è svolta via software ed i parametri per la conversione possono richiedere di essere adattati in funzione della combinazione display/sensore-touch


> <br><em>tra l'altro: il sensore touch è piuttosto rigido e fragile ed è facile romperlo, soprattutto nei prototipi dove il display è semplicemente agganciato alla breadboard.<br>
>In questi casi è molto semplice rimuovere il sensore rotto e, tagliando il connettore flessibile, si ottiene un semplice e funzionante display grafico senza la funzione touch.<br></em><br> 


## Verificare la calibrazione del sensore touch

Crea un nuovo progetto con la "Gestione Diretta" del display (anche se il tuo progetto finale è basato su TouchGFX) seguendo tutte le istruzioni in: [(gestione diretta) Guida per aggiungere la libreria al progetto creato](../3B-DIRECT), aggiungendo anche il file con le funzioni touch di test  (z_touch_XPT2046_test.c and .h).<br> 
Nel main loop richiama semplicemente la funzione "TestDrawing", in questo modo:<br>

  ```sh
  ...
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) 			
  {
	Touch_TestDrawing();		
  ...
  ```

Complila ed esegui il progetto. Tocca il sensore e muovi il dito mentre tocchi.<br>
Se non vedi una croce bianca (su fondo blu) che segue il tuo dito, il sensore deve essere ricalibrato.



## Effettuare la calibrazione

Per effettuare la calibrazione, cambia il main loop in questo modo:<br>


  ```sh
  ...
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) 			
  {
	Touch_TestCalibration();
  ...
  ```

ed esegui il progetto.
Segui le istruzioni sul display.
Al termine della calibrazione, il display mostrerà il seti di parametri calibrato per la conversione touch-display.

Registra i nuovi valori nel file "z_touch_XPT2046.h" (nella sezione "CALIBRATION PARAMETERS")

Verifica la nuova calibrazione con la funzione Touch_TestDrawing().
Puoi anche provare a modificare direttamente i parametri di calibrazione, per l'allineamento di precisione finale.

Quando l'allineamento touch/grafica ti soddisfa, manitieni il file z_touch_XPT2046.h file per i tuoi prossimi progetti con il sensore.


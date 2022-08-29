_**Piu' sotto, al termine della lingua inglese trovi il testo in italiano. </i>**_
_**Below English text you'll find the Italian version</i>**_

<br>
<br>

# Source and header files

- Copy the .c files into the /Core/Src folder in your STM32CubeIDE project
- Copy the .h files into the /Core/Inc folder in your STM32CubeIDE project
- into the /Core/Inc folder, open the main.h file and add (better if into "USER CODE BEGIN Includes")include directive for every .h file copied in this order:<br>
<br>
(main.h)<br>
...<br>
/* USER CODE BEGIN Includes */<br>
#include "fonts.h"<br>
#include "z_displ_ILI9XXX.h"<br>
#include "z_displ_ILI9XXX_test.h"<br>
#include "z_touch_XPT2046.h"<br>
#include "z_touch_XPT2046_test.h"<br>
#include "z_touch_XPT2046_menu.h"<br>
/* USER CODE END Includes */<br>
...<br>
<br>
"z_displ_ILI9XXX_test" files (.c and .h) must be copied into the project only if you want to use graphics test/demo functions. You don't need to add it in the production project.<br>
"z_touch_XPT2046_test" files (.c and .h) must be copied into the project only if you want to use touch test/demo functions. You don't need to add it in the production project.<br>
adding "z_touch_XPT2046_test", compiler will ask you to enable printf library into project properties (follow warnings instructions) <br>
<br>
<br>
That should be enough, now you can move to these sections:
- ["How to" add it to a CubeIDE project](../HOWTO)
- ["How to" handle display backlight](../BACKLIGHT)



---

<br>
<br>

# File sorgente e di testata

- Copia i file .c nella cartella /Core/Src del progetto STM32CubeIDE
- Copia i file .h nella cartella /Core/Inc del progetto STM32CubeIDE
- Nella cartella /Core/Inc, apri il file main.h e aggiungi la direttiva include (nella sezione "USER CODE BEGIN Includes"?) per ogni file .h copiato in questo ordine:<br>

<br>(main.h)
<br>...
<br>/* USER CODE BEGIN Includes */
<br>#include "fonts.h"
<br>#include "z_displ_ILI9XXX.h"
<br>#include "z_displ_ILI9XXX_test.h"
<br>#include "z_touch_XPT2046.h"
<br>#include "z_touch_XPT2046_test.h"
<br>#include "z_touch_XPT2046_menu.h"
<br>/* USER CODE END Includes */
<br>...
<br>
<br>
I file "z_displ_ILI9XXX_test" (.c e .h) devono essere copiati nel progetto solo se vuoi usare le funzioni di demo grafiche e test. Non occorrono questi file nella versione definitiva del progetto.<br>
I file "z_touch_XPT2046_test" (.c e .h) devono essere copiati nel progetto solo se vuoi usare le funzioni touch di demo e test. Non occorrono questi file nella versione definitiva del progetto.<br>
Aggiungendo "z_touch_XPT2046_test", il compilatore chiederà di attivare la libreria printf nelle proprietà del progetto  (seguire le istruzioni dei warning)<br> 
<br>
<br>
Dovrebbe bastare, ora puoi andare alle sezioni:
- [Guida per integrare il software in un progetto con CubeIDE](../HOWTO)
- [Guida alla gestione della retroilluminazione](../BACKLIGHT)





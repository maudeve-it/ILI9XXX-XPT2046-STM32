# ILI9XXX +XPT2046 STM32

 /* USER CODE BEGIN 2 */
  Displ_Init(Displ_Orientat_0);
  Displ_CLS(BLACK);						// clear the screen
  Displ_BackLight('I');  					// initialize backlight
  Displ_BackLight('1');					// light-up display ad max light level
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  InitMenu();
  while (1)
  {
	  Displ_PerfTest();
//	  Touch_ShowData();
//	  Touch_TestDrawing();
//	  Touch_TestCalibration();
//	  RunMenu1();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }

/* USER CODE BEGIN Header */
/**
  ****************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ****************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ****************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <zmienna.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */
RTC_TimeTypeDef nowsTime = {0};
RTC_DateTypeDef nowsDate = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

typedef struct record
{
	int poziom;
	int wynik;
	char nazwa[4];
} record;

int przycisk()
{
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 10);
	int x = HAL_ADC_GetValue(&hadc1);

	float y = x*(3.3/(pow(2, 12) - 1));

	if(y > 2.8)
	{
		return 6; //Nic nie wcisnieto
	}

	/*if(y > 2.33 && y < 2.89)		return 1;//Select
	else if(y > 1.75 && y < 2.33)	return 2;//Left
	else if(y > 1.23 && y < 1.75)	return 3;//Down
	else if(y > 0.59 && y < 1.23)	return 4;//Up
	else if(y < 0.58)				return 5;//Right*/

	if(y > 2.1 && y < 2.5)
	{
		return 1;//Select
	}
	else if(y > 1.6 && y < 1.9)
	{
		return 2;//Left
	}
	else if(y > 1.1 && y < 1.4)
	{
		return 3;//Down
	}
	else if(y > 0.5 && y < 0.65)
	{
		return 4;//Up
	}
	else if(y < 0.3)
	{
		return 5;//Right
	}
	else
	{
		return 6;
	}
}

void leaderBoard(record* leaderboard)
{
	HAL_Delay(150);

	char opcje[12][15];
	strcpy(opcje[0],"Leaderboard:");
	strcpy(opcje[11],"Powrot");
	char bufer[15];
	char bufer2[6];
	for(int i=0;i<10;i++){

		if(leaderboard[i].wynik != -1)
		{
			sprintf(bufer, "%d", i+1);
			strcat(bufer,".");
			strcat(bufer,leaderboard[i].nazwa);
			strcat(bufer," ");
			sprintf(bufer2, "%d", leaderboard[i].wynik);
			strcat(bufer,bufer2);
			if(leaderboard[i].poziom==0)strcat(bufer,", LAT");
			else if(leaderboard[i].poziom==1)strcat(bufer,", SRE");
			else strcat(bufer,", TRU");
		}
		else
		{
			sprintf(bufer, "%d", i+1);
			strcat(bufer,".");
		}

		strcpy(opcje[i+1],bufer);
	}

	int opcja = 0;

	while(1){
		lcd_clear();
		lcd_print(1,1,opcje[opcja]);
		if(opcja!=11)lcd_print(2,1,opcje[opcja+1]);

		int wybor = 6;

		while(wybor == 6)
		{
		  wybor = przycisk();
		  HAL_Delay(5);
		}

		if(wybor == 1) //Select
		{
			if(opcja==11)break;
		}
		else if(wybor == 4) //Up
		{
			if(opcja > 0)opcja--;
		}
		else if(wybor == 3) //Down
		{
			if(opcja < 11)opcja++;
		}
		HAL_Delay(150);
	}
}
void trudnosc(int* poziom)
{
	HAL_Delay(150);

	char opcje[4][15] = {"Latwy", "Sredni", "Trudny","Powrot"};
	int opcja = 0;
	while(1){
		lcd_clear();
		lcd_print(1,5,"Trudnosc");
		if(opcja != 0)lcd_print(2,1,"<");
		if(opcja != 3)lcd_print(2,16,">");

		int pozycja_wyswietlania = (16 - strlen(opcje[opcja])) / 2 + 1;
		lcd_print(2,pozycja_wyswietlania,opcje[opcja]);

		int wybor = 6;

		while(wybor == 6)
		{
		  wybor = przycisk();
		  HAL_Delay(5);
		}

		if(wybor == 1) //Select
		{
			if(opcja==3)
			{
				break;
			}
			else
			{
				*poziom=opcja;

				lcd_clear();
				lcd_print(1,2, "Zmieniono na:");

				int pozycja_wyswietlania = (16 - strlen(opcje[opcja])) / 2 + 1;
				lcd_print(2,pozycja_wyswietlania,opcje[opcja]);

				HAL_Delay(1500);
				break;
			}
		}
		else if(wybor == 2) //Left
		{
			if(opcja > 0)opcja--;
		}
		else if(wybor == 5) //Right
		{
			if(opcja < 3)opcja++;
		}
		HAL_Delay(150);
	}
}

void pacmanPrawo(int linia, int kolumna)
{
	lcd_gotoxy(linia, kolumna);
	lcd_char_cp(0);
}

void pacmanLewo(int linia, int kolumna)
{
	lcd_gotoxy(linia, kolumna);
	lcd_char_cp(1);
}

void wisienka(int linia, int kolumna)
{
	lcd_gotoxy(linia, kolumna);
	lcd_char_cp(2);
}

void duch(int linia, int kolumna)
{
	lcd_gotoxy(linia, kolumna);
	lcd_char_cp(3);
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  lcd_init(_LCD_4BIT, _LCD_FONT_5x8, _LCD_2LINE);
  int poziom = 0; //0 - latwy, 1 - sredni, 2 - trudny
  int aktywnosc_gry = 0; //0 - gra nie jest aktywna
  record leaderboard[10];
  srand(time(NULL));

  for(int i=0;i<10;i++)
  {
	  leaderboard[i].wynik = -1;
	  leaderboard[i].poziom = -1;
	  strcpy(leaderboard[i].nazwa, "AAA");
  }
  char opcje[3][15] = {"Start", "Trudnosc", "Leaderboard"};
  int opcja = 0; //Start

  RTC_TimeTypeDef sTime = {0};

  sTime.DayLightSaving=RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation=RTC_STOREOPERATION_RESET;

	lcd_cmd(0x40); //Pacman Prawo
	lcd_char_cp(0x06);
	lcd_char_cp(0x09);
	lcd_char_cp(0x12);
	lcd_char_cp(0x14);
	lcd_char_cp(0x12);
	lcd_char_cp(0x09);
	lcd_char_cp(0x06);
	lcd_char_cp(0x00);

	//lcd_cmd(0x41); Pacman Lewo
	lcd_char_cp(0x0C);
	lcd_char_cp(0x12);
	lcd_char_cp(0x09);
	lcd_char_cp(0x05);
	lcd_char_cp(0x09);
	lcd_char_cp(0x12);
	lcd_char_cp(0x0C);
	lcd_char_cp(0x00);

	//lcd_cmd(0x42); Wisienka
	lcd_char_cp(0x06);
	lcd_char_cp(0x04);
	lcd_char_cp(0x0E);
	lcd_char_cp(0x11);
	lcd_char_cp(0x11);
	lcd_char_cp(0x11);
	lcd_char_cp(0x0E);
	lcd_char_cp(0x00);

	//lcd_cmd(0x42); Duszek
	lcd_char_cp(0x04);
	lcd_char_cp(0x0E);
	lcd_char_cp(0x1F);
	lcd_char_cp(0x15);
	lcd_char_cp(0x1B);
	lcd_char_cp(0x1F);
	lcd_char_cp(0x15);
	lcd_char_cp(0x00);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(aktywnosc_gry == 0)
	  {
		  //menu
		  lcd_clear();
		  lcd_print(1,3,"Pacman Menu");
		  if(opcja != 0)lcd_print(2,1,"<");
		  if(opcja != 2)lcd_print(2,16,">");

		  int pozycja_wyswietlania = (16 - strlen(opcje[opcja])) / 2 + 1;
		  lcd_print(2,pozycja_wyswietlania,opcje[opcja]);

		  int wybor = 6;

		  while(wybor == 6)
		  {
			  wybor = przycisk();
			  HAL_Delay(5);
		  }

		  if(wybor == 1) //Select
		  {
			  if(opcja==0)aktywnosc_gry=1;
			  else if(opcja==1)trudnosc(&poziom);
			  else leaderBoard(&(leaderboard[0]));
		  }
		  if(wybor == 2) //Left
		  {
			  if(opcja > 0)opcja--;
		  }
		  if(wybor == 5) //Right
		  {
			  if(opcja < 2)opcja++;
		  }
		  HAL_Delay(150);
	  }
	  else
	  {
		  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
		  {
			  Error_Handler();
		  }
		  int x=7,y=2;
		  int xw=1,yw=1;
		  int xd=3,yd=2;
		  int xdelta,ydelta;
		  int wynik=0;

		  int kierunek = 0; //0 - prawo, 1 - lewo

		  HAL_RTC_GetTime(&hrtc, &nowsTime, RTC_FORMAT_BIN);
		  HAL_RTC_GetDate(&hrtc, &nowsDate, RTC_FORMAT_BIN);
		  HAL_Delay(100);
		  int ghostTime = nowsTime.Seconds;
		  while(aktywnosc_gry)
		  {

			  lcd_clear();

			  if(kierunek == 0)
			  {
				  pacmanPrawo(y, x);
			  }
			  else
			  {
				  pacmanLewo(y, x);
			  }

			  wisienka(yw, xw);
			  if(poziom == 2)duch(yd,xd);

			  int wybor = 6;

			  while(wybor == 6)
			  {
				  wybor = przycisk();
				  if(nowsTime.Seconds>30)break;

				  if(poziom == 2 && nowsTime.Seconds-ghostTime>0)
				  {
					  lcd_clear();
					  if(kierunek == 0)
					  {
						  pacmanPrawo(y, x);
					  }
					  else
					  {
						  pacmanLewo(y, x);
					  }

					  wisienka(yw, xw);
					  xdelta = rand()%3-1;
					  if(xdelta==0) ydelta=rand()%3-1;

					  if(xd+xdelta<17&&xd+xdelta>0)xd+=xdelta;
					  if(yd+ydelta<3&&yd+ydelta>0)yd+=ydelta;
					  duch(yd,xd);
					  if(xd==xw&&yd==yw)break;
					  if(xd==x&&yd==y)break;
					  ghostTime = nowsTime.Seconds;
				  }

				  HAL_Delay(5);
			  }

			  /*if(poziom == 2 && (nowsTime.Seconds*1000+nowsTime.SubSeconds)-(ghostTime.Seconds*1000+ghostTime.SubSeconds)>400)
			  {
				  xdelta = rand()%3-1;
				  if(xdelta==0) ydelta=rand()%3-1;

				  if(xd+xdelta<17&&xd+xdelta>0)xd+=xdelta;
				  if(yd+ydelta<3&&yd+ydelta>0)yd+=ydelta;
				  duch(yd,xd);
				  ghostTime = nowsTime;
			  }*/

			  if(wybor == 1) //Select
			  {

			  }
			  else if(wybor == 2) //Left
			  {
				  if(x>1)x--;

				  kierunek = 1;
			  }
			  else if(wybor == 3) //Down
			  {
				  if(y==1)y++;
			  }
			  else if(wybor == 4) //Up
			  {
				  if(y==2)y--;
			  }
			  else if(wybor == 5) //Right
			  {
				  if(x<16)x++;

				  kierunek = 0;
			  }

			  if(x==xw&&y==yw)wynik++;
			  if(poziom == 2&&xd==xw&&yd==yw)wynik--;



			  while(x==xw&&y==yw||xd==xw&&yd==yw)
			  {
				  if(poziom == 0)
				  {
					  xw=rand()%15+1; //Zakres od 0 do 15
					  yw=rand()%2+1; //Zakres od 0 do 2
				  }
				  else if(poziom == 1)
				  {
					  xw = rand() % 5+1; // Zakres od 0 do 4
					  if (x < 7)
					  {
						  xw += 10; // Zakres od 12 do 15
					  }

					  yw=rand()%2+1; //Zakres od 0 do 2
				  }
				  else if(poziom == 2)
				  {
					  xw = rand() % 5+1; // Zakres od 0 do 4
					  if (x < 7)
					  {
						  xw += 10; // Zakres od 12 do 15
					  }

					  yw=rand()%2+1; //Zakres od 0 do 2
				  }
			  }

			  if(poziom == 2&&xd==x&&yd==y)break;

			  if(nowsTime.Seconds>30)break;

			  HAL_Delay(50);
		  }

		  if(poziom == 1)
		  {
			  wynik = wynik * 1.5;
		  }
		  else if(poziom == 2)
		  {
			  wynik = wynik * 2;
		  }

		  int setNewLeader=0;
		  if(wynik>leaderboard[9].wynik) setNewLeader=1;

		  lcd_clear();
		  lcd_print(1, 3, "Koniec gry!");
		  char b_wynik[15];
		  char b_wynik2[6];
		  strcpy(b_wynik, "Wynik: ");
		  sprintf(b_wynik2, "%d", wynik);
		  strcat(b_wynik, b_wynik2);
		  lcd_print(2, 1, b_wynik);
		  HAL_Delay(3000);
		  lcd_clear();

		  if(setNewLeader){
			  char alfabet[26][2] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
			  int nazwa[3] = {0, 0, 0};
			  int pozycja = 0;
			  char bufor[4];

			  while(1)
			  {
				  strcpy(bufor, alfabet[nazwa[0]]);
				  strcat(bufor, alfabet[nazwa[1]]);
				  strcat(bufor, alfabet[nazwa[2]]);

				  lcd_print(1, 1, "Wprowadz nazwe:");
				  lcd_print(2, 7, bufor);

				  int w = 6;

				  while(w == 6)
				  {
					  w = przycisk();
					  HAL_Delay(5);
				  }
				  HAL_Delay(100);

				  if(w == 1) break;
				  else if(w == 2 && pozycja > 0) pozycja--;
				  else if(w == 3 && nazwa[pozycja] > 0) nazwa[pozycja]--;
				  else if(w == 4 && nazwa[pozycja] < 25) nazwa[pozycja]++;
				  else if(w == 5 && pozycja < 2) pozycja++;
			  }

			  strcpy(leaderboard[9].nazwa, bufor);
			  leaderboard[9].poziom = poziom;
			  leaderboard[9].wynik = wynik;
			  int i, j;
			  int swapped;
			  for (i = 0; i < 9; i++) {
				  swapped = 0;
				  for (j = 0; j < 10 - i - 1; j++) {
					  if (leaderboard[j].wynik < leaderboard[j + 1].wynik) {
						  record leader = leaderboard[j];
						  leaderboard[j] = leaderboard[j + 1];
						  leaderboard[j + 1] = leader;
						  swapped = 1;
					  }
				  }
				  if (swapped == 0) break;
			  }
		  }
		  wynik=0;
		  aktywnosc_gry = 0;

	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_D6_Pin|LCD_D5_Pin|LCD_D4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_D7_Pin|LCD_RS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_D6_Pin LCD_D5_Pin LCD_D4_Pin */
  GPIO_InitStruct.Pin = LCD_D6_Pin|LCD_D5_Pin|LCD_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : LCD_EN_Pin */
  GPIO_InitStruct.Pin = LCD_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_D7_Pin LCD_RS_Pin */
  GPIO_InitStruct.Pin = LCD_D7_Pin|LCD_RS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

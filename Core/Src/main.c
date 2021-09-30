/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

#include "threadGyro.h"
#include "threadDebug.h"
#include "threadEthernet.h"

/**
 * @brief Git libs include
 */
#include "debug.h"
#include "crg20.h"
#include "net_prots.h"

/**
 * @brief threads init
 * Ethernet thread for UDP
 * Debug thread for UART debugging
 * Gyro thread for gyroscope computing
 */
osThreadId_t EthernetHandle;
const osThreadAttr_t EthernetAttr = {
  .name = "Ethernet",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 512 * 4
};

osThreadId_t DebugHandle;
osThreadAttr_t DebugAttr = {
    .name = "Debug",
    .priority = (osPriority_t)osPriorityBelowNormal,
    .stack_size = 512 * 4
};

osThreadId_t GyroHandle;
osThreadAttr_t GyroAttr = {
    .name = "Gyroscope",
    .priority = (osPriority_t)osPriorityNormal,
    .stack_size = 512 * 4
};

void SystemClock_Config(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();

  HAL_GPIO_WritePin(GPIOB, SPI_SS_G2_Pin | SPI_SS_G1_Pin, 1);

  print_in("Start\r\n");


  if (ENC28J60_Init() != ENC28J60_OK) SystemError(SYSTEM_ERROR_ETHERNET);
  print_in("ENC28J60 Init: Success\r\n");

  crg20_module_t gyro1 = CRG20_Init(SPI_SS_G1_GPIO_Port, SPI_SS_G1_Pin);
  while(gyro1.status != CRG20_OK) gyro1 = CRG20_Init(SPI_SS_G1_GPIO_Port, SPI_SS_G1_Pin);
  print_in("CRG20 (1) Init: Success\r\n");

  crg20_module_t gyro2 = CRG20_Init(SPI_SS_G2_GPIO_Port, SPI_SS_G2_Pin);
  while(gyro2.status != CRG20_OK) gyro2 = CRG20_Init(SPI_SS_G2_GPIO_Port, SPI_SS_G2_Pin);
  print_in("CRG20 (2) Init: Success\r\n");
  
  // while(1){
  //   UDP_PacketSend(ip_set(192, 168, 0, 25), 5555, "yeppp", 6);
  //   print_db("yeppie\r\n");
  //   HAL_Delay(500);
  // }

  print_in("RTOS Initialization...\r\n");

  osKernelInitialize();  

  EthernetHandle = osThreadNew(threadEthernet, NULL, &EthernetAttr);
  GyroHandle =     osThreadNew(threadGyro,     NULL, &GyroAttr);
  DebugHandle =    osThreadNew(threadDebug,    NULL, &DebugAttr);

  print_in("RTOS Ready...\r\n");

  osKernelStart();
 
  while (1){

  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
 
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
 
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

void SystemError(uint8_t arg){
  if (arg == SYSTEM_ERROR_ETHERNET){
    print_er("Ethernet: INITIALIZATION ERROR, Rebooting...\r\n"); // TODO: add fixing the ethernet
    HAL_NVIC_SystemReset();
  }
  else if (arg == SYSTEM_ERROR_GYRO_OVERHEAT){
    print_er("CRG20: OVERHEAT, Stop Gyroscope Thread\r\n"); // TODO: add thermocontrol for gyroscope
  }
  else if (arg == SYSTEM_ERROR_GYRO_NOT_FOUND){
    print_er("CRG20: NOT FOUND, Rebooting...\r\n"); // TODO: add fixing the gyro init
    HAL_NVIC_SystemReset();
  }
}


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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

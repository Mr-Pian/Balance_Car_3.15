/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD_ST7789S.h"
#include "Music.h"
#include "tb6612.h"
#include "control.h"
#include "pid.h"
#include "uart_unpack.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_SPI3_Init();
  MX_I2C3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM3_Init();
  MX_TIM12_Init();
  MX_TIM14_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	delay_init(168);
	//BSP_Hardware Init
	
	
	TFTSPI_Init();  //屏幕初始化lcd
	
	//Play_Music(&Open);  //开机音效
	
	//delay_ms(1000);

	
  
	Control_Init(&the_car);
	pid_init(the_car.the_pid->pid_speed_L);
	the_car.the_pid->pid_speed_L->f_param_init(the_car.the_pid->pid_speed_L,PID_Speed,MAX_MOTOR_DUTY,MAX_MOTOR_DUTY,0,1,0.0,10000.0f,100.0f,0.0f);
	the_car.the_pid->pid_speed_L->enable = 1;
	pid_init(the_car.the_pid->pid_speed_R);
	the_car.the_pid->pid_speed_R->f_param_init(the_car.the_pid->pid_speed_R,PID_Speed,MAX_MOTOR_DUTY,MAX_MOTOR_DUTY,0,1,0.0,10000.0f,100.0f,0.0f);  //150.0f,6.5f,0.0f);
	the_car.the_pid->pid_speed_R->enable = 1;
	pid_init(the_car.the_pid->pid_stand_angle);
	the_car.the_pid->pid_stand_angle->f_param_init(the_car.the_pid->pid_stand_angle,PID_Position,0,1000,0,10,0.0, 80.0f,0.0f,100.0f);  //80.0f,0.0f,0.0f);
	pid_init(the_car.the_pid->pid_stand_angle_speed);
	the_car.the_pid->pid_stand_angle_speed->f_param_init(the_car.the_pid->pid_stand_angle_speed,PID_Position,0,1000,0,2,0.0,3.0f,0.03f,10.0f);
	pid_init(the_car.the_pid->pid_target_speed);
	the_car.the_pid->pid_target_speed->f_param_init(the_car.the_pid->pid_target_speed,PID_Position,0,10,0,10,0.0f,8.0f,0.1f,2.0f);
	
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
	HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim12);
	HAL_TIM_Base_Start_IT(&htim14);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart3, Received_Buffer_1, 128); //开启imu数据接收DMA空闲中断
	
	the_car.Motor_L->motor_status = M_On;
	the_car.Motor_R->motor_status = M_On;
	
	Motor_Start(Both);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	while (1)
  {
		TFT_Printf(0,0,COLOR_BLACK, COLOR_WHITE, fsize_12X24, "LSPEED: %f ", the_car.Motor_L->real_speed);
		TFT_Printf(0,24,COLOR_BLACK, COLOR_WHITE, fsize_12X24, "RSPEED: %f ", the_car.Motor_R->real_speed);
		TFT_Printf(0,48,COLOR_BLACK, COLOR_WHITE, fsize_12X24, "AOUT: %f ", the_car.the_pid->pid_stand_angle->output);
		TFT_Printf(0,72,COLOR_BLACK, COLOR_WHITE, fsize_12X24, "ASOUT: %f ", the_car.the_pid->pid_stand_angle_speed->output);
		TFT_Printf(0,96,COLOR_BLACK, COLOR_WHITE, fsize_12X24, "OUT: %f ", the_car.the_pid->pid_target_speed->output);		
//    delay_ms(2000);
//		the_car.the_pid->pid_speed_L->target = 0.7;
//		the_car.the_pid->pid_speed_R->target = 0.7;
//		delay_ms(2000);
//		the_car.the_pid->pid_speed_L->target = 0.1;
//		the_car.the_pid->pid_speed_R->target = 0.1;
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
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

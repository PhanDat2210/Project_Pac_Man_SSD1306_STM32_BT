/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f1xx_it.h"
#include "fonts.h"
#include "ssd1306_oled.h"
#define MAP_WIDTH 18
#define MAP_HEIGHT 32
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */
extern int pacman_x;
extern int pacman_y;
extern int map[MAP_HEIGHT][MAP_WIDTH];

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line0 interrupt.
  */
volatile uint32_t last_interrupt_time = 0;  // Lưu trữ thời gian ngắt cuối cùng
#define DEBOUNCE_DELAY 200  // Thời gian debounce (200 ms)

uint32_t last_interrupt_time_buttons[4] = {0}; // Dùng cho 4 nút điều hướng (GPIOA: PA0, PA1, PA2, PA3)

void handle_pacman_interrupt(uint8_t button_index, uint16_t pin, int dx, int dy)
{
    uint32_t current_time = HAL_GetTick();

    if (__HAL_GPIO_EXTI_GET_IT(pin) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(pin);  // Xóa cờ ngắt

        // Kiểm tra debounce (phải đủ thời gian trễ)
        if (current_time - last_interrupt_time_buttons[button_index] > DEBOUNCE_DELAY) {
        	last_interrupt_time_buttons[button_index] = current_time;  // Cập nhật thời gian ngắt cuối cùng

            // Tính vị trí mới của pacman
            int new_x = pacman_x + dx;
            int new_y = pacman_y + dy;

            // Kiểm tra nếu vị trí mới không nằm ngoài biên và không phải khối cản
            if (new_x >= 0 && new_x < MAP_WIDTH &&
                new_y >= 0 && new_y < MAP_HEIGHT &&
                map[new_y][new_x] == 0)
            {
                pacman_x = new_x;
                pacman_y = new_y;
            }
        }
    }
}

void EXTI0_IRQHandler(void)
{
    handle_pacman_interrupt(0, GPIO_PIN_0, 0, -1); // Di chuyển lên (dx = 0, dy = -1)
}

void EXTI1_IRQHandler(void)
{
    handle_pacman_interrupt(1, GPIO_PIN_1, 0, 1); // Di chuyển xuống (dx = 0, dy = +1)
}

void EXTI2_IRQHandler(void)
{
    handle_pacman_interrupt(2, GPIO_PIN_2, -1, 0); // Di chuyển trái (dx = -1, dy = 0)
}

void EXTI3_IRQHandler(void)
{
    handle_pacman_interrupt(3, GPIO_PIN_3, 1, 0); // Di chuyển phải (dx = +1, dy = 0)
}

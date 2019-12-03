/**
  ******************************************************************************
  * @file    main.c
  * @author  Arjav Naik
  * @version V1.0
  * @date    01-November-2019
  * @brief   Example code for FreeRTOS with 2 Task creation on STM32F446re Nucleo Board
  ******************************************************************************
*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "stm32f4xx.h"
#include "FreeRTOS.h" // Always include this when working with FreeRTOS
#include "task.h"

TaskHandle_t xTaskHandle1 = NULL;
TaskHandle_t xTaskHandle2 = NULL;

void vTask1_handler( void *params);
void vTask2_handler( void *params);


#ifdef USE_SEMIHOSTING
// used for semi-hosting
extern void initialise_monitor_handles();
#endif

static void prvSetupHardware(void);
static void prvSetupUart(void);

void printmsg(char *msg);

// buffer for UART
char usr_msg[250]={0};

// Macros for spin lock

#define TRUE 1
#define FALSE 0
#define AVAILABLE TRUE
#define NOT_AVAILABLE FALSE

uint8_t UART_ACCESS_KEY = AVAILABLE;

int main(void)
{
#ifdef USE_SEMIHOSTING
	initialise_monitor_handles();
	printf("This is Hello World Example \n");
#endif

	DWT->CTRL |= (1 << 0);     //Enable CYCCNT in DWT_CTRL.

	// 1.Reset the RCC clock configuration to the default reset state
	// This function makes HSI ON, HSE OFF, system clock = 16 MHz, CPU clock = 16 MHz
	RCC_DeInit();

	// 2. Update the SystemCoreClock variable
	SystemCoreClockUpdate();

	prvSetupHardware();

	sprintf(usr_msg,"This is hello world application starting\r\n");
	printmsg(usr_msg);

	//Start Recording
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	// 3. Creating tasks : Task 1 & Task 2
	xTaskCreate(vTask1_handler, "Task-1", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle1);
	xTaskCreate(vTask2_handler, "Task-2", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle2);

	/* Start the scheduler */

	vTaskStartScheduler();

	for(;;);

}

void vTask1_handler( void *params){

	while(1)				// task handler should never return in freeRTOS that's why while(1)
	{
		if( UART_ACCESS_KEY == AVAILABLE ){    // spin lock implementation to avoid racing condition at uart TX buffer

			UART_ACCESS_KEY = NOT_AVAILABLE;
			printmsg("Hello world from Task-1\r\n");
			UART_ACCESS_KEY = AVAILABLE;
			traceISR_EXIT_TO_SCHEDULER();
			taskYIELD();
		}
	}
}

void vTask2_handler( void *params){

	while(1)
	{
			if( UART_ACCESS_KEY == AVAILABLE ){    // spin lock implementation to avoid racing condition at uart TX buffer

					UART_ACCESS_KEY = NOT_AVAILABLE;
					printmsg("Hello world from Task-2\r\n");
					UART_ACCESS_KEY = AVAILABLE;
					traceISR_EXIT_TO_SCHEDULER();
					taskYIELD();
			}
	}
}

static void prvSetupUart(void){
	GPIO_InitTypeDef gpio_uart_pins;

		USART_InitTypeDef uart2_init;

		// 1. Enable the UART2 and GPIOA  Peripheral clock
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);


		//PA 2 is USART2_TX, PA 3 is USART2_RX

		//2. Alternate Pin configuration to function as UART

		//set the value of all member element of struct gpio_uart_pins as 0 because they may contain garbage
		memset(&gpio_uart_pins,0,sizeof(gpio_uart_pins));

		gpio_uart_pins.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF;
		gpio_uart_pins.GPIO_PuPd = GPIO_PuPd_UP; // because when the uart line is idle it is 1

		GPIO_Init(GPIOA, &gpio_uart_pins);

		// 3. Alternate Function (AF) Mode setting for the PINs
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); // For PA2 which is config as UART TX (AF7 mode)
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); // For PA3 which is config as UART RX (AF7 mode)

		// 4. UART parameter Config

		//set the value of all member element of struct uart2_init as 0 because they may contain garbage
		memset(&uart2_init,0,sizeof(uart2_init));

		uart2_init.USART_BaudRate = 115200;
		uart2_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		uart2_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
		uart2_init.USART_Parity = USART_Parity_No;
		uart2_init.USART_StopBits = USART_StopBits_1;
		uart2_init.USART_WordLength = USART_WordLength_8b;

		USART_Init(USART2, &uart2_init);

		//5. enable the UART2 peripheral
		USART_Cmd(USART2, ENABLE);

}


static void prvSetupHardware(void){

	//setup UART2
	prvSetupUart();

}

void printmsg(char *msg){

	for(uint32_t i=0; i<strlen(msg); i++)
	{
		// wait till the transmit data register is empty
		while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) != SET);
		USART_SendData(USART2, msg[i]);
	}

}

/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "stm32f4xx.h"
#include "FreeRTOS.h" // Always include this when working with FreeRTOS
#include "task.h"

//macros
#define FALSE 0
#define TRUE 1
#define NOT_PRESSED FALSE
#define PRESSED TRUE


// function prototypes
static void prvSetupHardware(void);
static void prvSetupUart(void);
void prvSetupGpio(void);
void printmsg(char *msg);

// buffer for UART
char usr_msg[250]={0};

// task prototypes
void led_task_handler(void *param);
void button_handler(void *param);

// Global variable
uint8_t button_status_flag = NOT_PRESSED;


int main(void)
{

	DWT->CTRL |= (1 << 0);     //Enable CYCCNT in DWT_CTRL.

	// 1.Reset the RCC clock configuration to the default reset state
	// This function makes HSI ON, HSE OFF, system clock = 16 MHz, CPU clock = 16 MHz
	RCC_DeInit();

	// 2. Update the SystemCoreClock variable
	SystemCoreClockUpdate();

	prvSetupHardware();

	//Start Recording
		SEGGER_SYSVIEW_Conf();
		SEGGER_SYSVIEW_Start();

	// 3. Creating tasks
	xTaskCreate(led_task_handler, "LED_TASK", configMINIMAL_STACK_SIZE, NULL, 1,NULL);

	/* Start the scheduler */

	vTaskStartScheduler();

	for(;;);
}

void led_task_handler(void *param){
	while(1){
		if(button_status_flag == PRESSED){
			//turn on the led
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
		}
		else{
			//turn off the led
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
		}
	}

}

void button_handler(void *param){

	button_status_flag ^= 1; // toggle the flag

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

	// setup GPIO
	prvSetupGpio();

}

void printmsg(char *msg){

	for(uint32_t i=0; i<strlen(msg); i++)
	{
		// wait till the transmit data register is empty
		while( USART_GetFlagStatus(USART2,USART_FLAG_TXE) != SET);
		USART_SendData(USART2, msg[i]);
	}

}

void prvSetupGpio(void){
	// board specific function
	// 1. Enable GPIO clocks for  A and C
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	// SYSCONFIG block is internal peripheral so we need to enable clock
	// in order to use interrupt
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	// 2. Configure GPIO Pins
	GPIO_InitTypeDef led_init, button_init;

	//set the value of all member element of struct gpio_uart_pins as 0 because they may contain garbage
	memset(&led_init,0,sizeof(led_init));
	memset(&button_init,0,sizeof(button_init));

	led_init.GPIO_Pin = GPIO_Pin_5;
	led_init.GPIO_Mode = GPIO_Mode_OUT;
	led_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	led_init.GPIO_OType = GPIO_OType_PP;
	led_init.GPIO_Speed = GPIO_Low_Speed;

	GPIO_Init(GPIOA, &led_init);


	button_init.GPIO_Pin = GPIO_Pin_13;
	button_init.GPIO_Mode = GPIO_Mode_IN;
	button_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	button_init.GPIO_OType = GPIO_OType_PP; // not matters as it is input mode
	button_init.GPIO_Speed = GPIO_Low_Speed;

	GPIO_Init(GPIOC, &button_init);

	// interrupt configuration for the button (PC13)
	// 1. system configuration for exti line (PC13 selection) (SYSCFG setting)
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);

	//2. configure the exti line 13, falling edge and interrupt mode
	EXTI_InitTypeDef exti_init;

	exti_init.EXTI_Line = EXTI_Line13;
	exti_init.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_init.EXTI_Trigger = EXTI_Trigger_Falling;
	exti_init.EXTI_LineCmd = ENABLE;

	EXTI_Init(&exti_init);

	// 3. NVIC setting (IRQ setting for line 13)
	NVIC_SetPriority(EXTI15_10_IRQn, 5);
	NVIC_EnableIRQ(EXTI15_10_IRQn);

}

// interrupt handler (they do not take any parameters and do not return any value)
void EXTI15_10_IRQHandler(void){
	// segger interrupt entry trace
	traceISR_ENTER();

	// 1. clear the interrupt pending bit of the EXTI line (13)
	EXTI_ClearITPendingBit(EXTI_Line13);

	// call the handler function
	button_handler(NULL);

	// segger interrupt exit trace
	traceISR_EXIT();
}
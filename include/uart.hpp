#ifndef __UART_HPP
#define __UART_HPP

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __USART2_CLK_ENABLE();
#define DMAx_CLK_ENABLE()                __DMA1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART2
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART2

/* Definition for USARTx's DMA */
#define USARTx_TX_DMA_CHANNEL             DMA_CHANNEL_4
#define USARTx_TX_DMA_STREAM              DMA1_Stream6
#define USARTx_RX_DMA_CHANNEL             DMA_CHANNEL_4
#define USARTx_RX_DMA_STREAM              DMA1_Stream5

/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_IRQn                DMA1_Stream6_IRQn
#define USARTx_DMA_RX_IRQn                DMA1_Stream5_IRQn
#define USARTx_DMA_TX_IRQHandler          DMA1_Stream6_IRQHandler
#define USARTx_DMA_RX_IRQHandler          DMA1_Stream5_IRQHandler

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))


/* Class definition ----------------------------------------------------------*/
class UART_COM {
public:
  static UART_COM& Instance();
  ~UART_COM();

  void InitHardware( void );

  void WriteBytes( uint8_t *pData, uint16_t Size );

  void Error_Handler(void);

  UART_HandleTypeDef uart_handle;

  __IO ITStatus UartReady;

private:
  UART_COM();
};

extern "C" {
  void USARTx_DMA_RX_IRQHandler(void);
  void USARTx_DMA_TX_IRQHandler(void);
}

#endif	/* __UART_HPP */

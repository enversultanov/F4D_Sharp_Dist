/*
 * i2c.h
 *
 *  Created on: 21 okt 2014
 *      Author: ensul1
 */

#ifndef I2C_HPP_
#define I2C_HPP_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor I2Cx/I2Cx instance used and associated
   resources */
/* Definition for I2Cx clock resources */
#define I2C_ADDRESS        0x00

#define I2Cx                             I2C1
#define I2Cx_CLK_ENABLE()                __I2C1_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                __DMA1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()       __GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()       __GPIOB_CLK_ENABLE()

#define I2Cx_FORCE_RESET()               __I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()             __I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_6
#define I2Cx_SCL_GPIO_PORT              GPIOB
#define I2Cx_SCL_AF                     GPIO_AF4_I2C1
#define I2Cx_SDA_PIN                    GPIO_PIN_9
#define I2Cx_SDA_GPIO_PORT              GPIOB
#define I2Cx_SDA_AF                     GPIO_AF4_I2C1

/* Definition for I2Cx's DMA */
#define I2Cx_TX_DMA_CHANNEL             DMA_CHANNEL_1
#define I2Cx_TX_DMA_STREAM              DMA1_Stream6
#define I2Cx_RX_DMA_CHANNEL             DMA_CHANNEL_1
#define I2Cx_RX_DMA_STREAM              DMA1_Stream5

/* Definition for I2Cx's NVIC */
#define I2Cx_DMA_TX_IRQn                DMA1_Stream6_IRQn
#define I2Cx_DMA_RX_IRQn                DMA1_Stream5_IRQn
#define I2Cx_DMA_TX_IRQHandler          DMA1_Stream6_IRQHandler
#define I2Cx_DMA_RX_IRQHandler          DMA1_Stream5_IRQHandler


static void Error_Handler(void);

/* Memory adress size define ------------------------------------------------ */
#define I2C_MEMADD_SIZE_8BIT            ((uint32_t)0x00000001)
#define I2C_MEMADD_SIZE_16BIT           ((uint32_t)0x00000010)

extern "C" {
  void I2Cx_DMA_RX_IRQHandler(void);
  void I2Cx_DMA_TX_IRQHandler(void);
}

/* I2C handler declaration */
extern I2C_HandleTypeDef I2cHandle;

class I2Cx_COM {
public:
  static I2Cx_COM& Instance();

  ~I2Cx_COM();

  void I2Cx_COM_INIT(void);

  void ReadBytes( uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size );
  I2C_HandleTypeDef i2cx_point;

private:
  I2Cx_COM();
};

#endif /* I2C_HPP_ */

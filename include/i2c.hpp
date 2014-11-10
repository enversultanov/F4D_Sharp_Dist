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

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */
static void Error_Handler(void);

/* Memory adress size define ------------------------------------------------ */
#define I2C_MEMADD_SIZE_8BIT            ((uint32_t)0x00000001)
#define I2C_MEMADD_SIZE_16BIT           ((uint32_t)0x00000010)

// 1000ms default read timeout (modify with "I2Cdev::readTimeout = [ms];")
#define READ_TIMEOUT     1000
#define WRITE_TIMEOUT     1000

/* I2C handler declaration */
extern I2C_HandleTypeDef I2cHandle;

class I2Cdev {
    public:
        I2Cdev();

        void I2C_Init(void);

        static int8_t readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout=READ_TIMEOUT);
        static int8_t readBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data, uint16_t timeout=READ_TIMEOUT);
        static int8_t readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout=READ_TIMEOUT);
        static int8_t readBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data, uint16_t timeout=READ_TIMEOUT);
        static int8_t readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout=READ_TIMEOUT);
        static int8_t readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data, uint16_t timeout=READ_TIMEOUT);
        static int8_t readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout=READ_TIMEOUT);
        static int8_t readWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data, uint16_t timeout=READ_TIMEOUT);

        static bool writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
        static bool writeBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data);
        static bool writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
        static bool writeBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data);
        static bool writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
        static bool writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data);
        static bool writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
        static bool writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);

};

#endif /* I2C_HPP_ */

/*
 * i2c.cpp
 *
 *  Created on: 21 okt 2014
 *      Author: ensul1
 */

#include "i2c.hpp"

I2Cx_COM& I2Cx_COM::Instance(){
  static I2Cx_COM instance;
  return instance;
}

I2Cx_COM::I2Cx_COM()
{
}

I2Cx_COM::~I2Cx_COM(){

}

/**
  * @brief  This function handles DMA interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA stream
  *         used for I2C data transmission
  */
void I2Cx_DMA_RX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(I2Cx_COM::Instance().i2cx_point.hdmarx);
}

/**
  * @brief  This function handles DMA interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA stream
  *         used for I2C data reception
  */
void I2Cx_DMA_TX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(I2Cx_COM::Instance().i2cx_point.hdmatx);
}

void I2Cx_COM::Error_Handler(){
  /* Turn LED5 on */
  BSP_LED_On(LED4);
  while(1)
  {
  }
}

void I2Cx_COM::InitHardware(void){
  /*##-1- Configure the I2C peripheral ######################################*/
  i2cx_point.Instance             = I2Cx;

  i2cx_point.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  i2cx_point.Init.ClockSpeed      = 400000;
  i2cx_point.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  i2cx_point.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
  i2cx_point.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  i2cx_point.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLED;
  i2cx_point.Init.OwnAddress1     = I2C_ADDRESS;
  i2cx_point.Init.OwnAddress2     = 0xFE;

  if(HAL_I2C_Init(&i2cx_point) != HAL_OK)
  {
    /* Initialization Error */
    while(1){
	BSP_LED_On(LED5);
    }
  }
}

void I2Cx_COM::ReadBytes( uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size ){
  /*##-3- Wait for the end of the transfer ###################################*/
  /*  Before starting a new communication transfer, you need to check the current
      state of the peripheral; if it’s busy you need to wait for the end of current
      transfer before starting a new one. */
  if(HAL_I2C_GetState(&i2cx_point) == HAL_I2C_STATE_READY){
    while( HAL_I2C_Mem_Read_DMA(&i2cx_point, DevAddress, MemAddress, MemAddSize, pData, Size ) != HAL_OK ){
      /* Error_Handler() function is called when Timout error occurs.
	 When Acknowledge failure ocucurs (Slave don't acknowledge it's address)
	 Master restarts communication */
      if (HAL_I2C_GetError(&i2cx_point) != HAL_I2C_ERROR_AF)
      {
	Error_Handler();
      }
    }
  }
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED4 on: Transfer in transmission process is correct */
  BSP_LED_On(LED4);

}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED6 on: Transfer in reception process is correct */
  BSP_LED_On(LED6);
}

/**
  * @brief  I2C error callbacks.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
 void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Turn LED5 on: Transfer error in reception/transmission process */
  BSP_LED_On(LED5);
}


/** @defgroup HAL_MSP_Private_Functions
  * @{
  */
/**
  * @brief I2C MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for transmission request by peripheral
  *           - NVIC configuration for DMA interrupt request enable
  * @param hi2c: I2C handle pointer
  * @retval None
  */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  static DMA_HandleTypeDef hdma_tx;
  static DMA_HandleTypeDef hdma_rx;

  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Enable GPIO Clocks #################################################*/
  /* Enable GPIO TX/RX clock */
  I2Cx_SCL_GPIO_CLK_ENABLE();
  I2Cx_SDA_GPIO_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* I2C TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = I2Cx_SCL_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = I2Cx_SCL_AF;
  HAL_GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);

  /* I2C RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = I2Cx_SDA_PIN;
  GPIO_InitStruct.Alternate = I2Cx_SDA_AF;
  HAL_GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Enable DMA peripheral Clock ########################################*/
  /* Enable DMA2 clock */
  DMAx_CLK_ENABLE();

  /*##-4- Configure the DMA streams ##########################################*/
  /* Configure the DMA handler for Transmission process */
  hdma_tx.Instance                 = I2Cx_TX_DMA_STREAM;

  hdma_tx.Init.Channel             = I2Cx_TX_DMA_CHANNEL;
  hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_tx.Init.Mode                = DMA_NORMAL;
  hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
  hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
  hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
  hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

  HAL_DMA_Init(&hdma_tx);

  /* Associate the initialized DMA handle to the the I2C handle */
  __HAL_LINKDMA(hi2c, hdmatx, hdma_tx);

  /* Configure the DMA handler for Transmission process */
  hdma_rx.Instance                 = I2Cx_RX_DMA_STREAM;

  hdma_rx.Init.Channel             = I2Cx_RX_DMA_CHANNEL;
  hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_rx.Init.Mode                = DMA_NORMAL;
  hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
  hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
  hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

  HAL_DMA_Init(&hdma_rx);

  /* Associate the initialized DMA handle to the the I2C handle */
  __HAL_LINKDMA(hi2c, hdmarx, hdma_rx);

  /*##-5- Enable peripheral Clock ############################################*/
  /* Enable I2C1 clock */
  I2Cx_CLK_ENABLE();

  /*##-6- Configure the NVIC for DMA #########################################*/
  /* NVIC configuration for DMA transfer complete interrupt (I2C1_TX) */
  HAL_NVIC_SetPriority(I2Cx_DMA_TX_IRQn, 4, 3);
  HAL_NVIC_EnableIRQ(I2Cx_DMA_TX_IRQn);

  /* NVIC configuration for DMA transfer complete interrupt (I2C1_RX) */
  HAL_NVIC_SetPriority(I2Cx_DMA_RX_IRQn, 3, 3);
  HAL_NVIC_EnableIRQ(I2Cx_DMA_RX_IRQn);
}

/**
  * @brief I2C MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hi2c: I2C handle pointer
  * @retval None
  */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{

  static DMA_HandleTypeDef hdma_tx;
  static DMA_HandleTypeDef hdma_rx;

  /*##-1- Reset peripherals ##################################################*/
  I2Cx_FORCE_RESET();
  I2Cx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure I2C Tx as alternate function  */
  HAL_GPIO_DeInit(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_PIN);
  /* Configure I2C Rx as alternate function  */
  HAL_GPIO_DeInit(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_PIN);

  /*##-3- Disable the DMA Streams ############################################*/
  /* De-Initialize the DMA Stream associate to transmission process */
  HAL_DMA_DeInit(&hdma_tx);
  /* De-Initialize the DMA Stream associate to reception process */
  HAL_DMA_DeInit(&hdma_rx);

  /*##-4- Disable the NVIC for DMA ###########################################*/
  HAL_NVIC_DisableIRQ(I2Cx_DMA_TX_IRQn);
  HAL_NVIC_DisableIRQ(I2Cx_DMA_RX_IRQn);
}

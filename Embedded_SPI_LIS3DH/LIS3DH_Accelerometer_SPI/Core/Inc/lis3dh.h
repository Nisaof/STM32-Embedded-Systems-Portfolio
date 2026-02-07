/*
 * lis3dh.h
 * LIS3DH 3-axis accelerometer driver for STM32
 */

#ifndef LIS3DH_H_
#define LIS3DH_H_

#include "stm32f7xx_hal.h"
#include "main.h"

// LIS3DH Register Addresses
#define LIS3DH_WHO_AM_I         0x0F
#define LIS3DH_CTRL_REG1        0x20
#define LIS3DH_CTRL_REG4        0x23
#define LIS3DH_OUT_X_L          0x28
#define LIS3DH_OUT_X_H          0x29
#define LIS3DH_OUT_Y_L          0x2A
#define LIS3DH_OUT_Y_H          0x2B
#define LIS3DH_OUT_Z_L          0x2C
#define LIS3DH_OUT_Z_H          0x2D

// Device ID
#define LIS3DH_ID               0x33

// Read/Write bits
#define LIS3DH_READ_BIT         0x80
#define LIS3DH_MULTI_BIT        0x40

// CS Pin Control
#define LIS3DH_CS_LOW()         HAL_GPIO_WritePin(LIS3DH_CS_GPIO_Port, LIS3DH_CS_Pin, GPIO_PIN_RESET)
#define LIS3DH_CS_HIGH()        HAL_GPIO_WritePin(LIS3DH_CS_GPIO_Port, LIS3DH_CS_Pin, GPIO_PIN_SET)

// Acceleration data structure
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
    float x_g;
    float y_g;
    float z_g;
} LIS3DH_Data;

// Function prototypes
uint8_t LIS3DH_Init(SPI_HandleTypeDef *hspi);
uint8_t LIS3DH_ReadRegister(SPI_HandleTypeDef *hspi, uint8_t reg);
void LIS3DH_WriteRegister(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t value);
void LIS3DH_ReadAccel(SPI_HandleTypeDef *hspi, LIS3DH_Data *data);

#endif /* LIS3DH_H_ */

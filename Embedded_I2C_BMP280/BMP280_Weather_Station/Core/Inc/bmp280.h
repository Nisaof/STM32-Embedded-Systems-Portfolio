/* bmp280.h - BMP280 Sensor Driver */
#ifndef BMP280_H
#define BMP280_H

#include "stm32h5xx_hal.h"
#include <stdint.h>

#define BMP280_ADDR_DEFAULT    0x77
#define BMP280_ADDR_ALT        0x76
#define BMP280_REG_CHIP_ID     0xD0
#define BMP280_REG_RESET       0xE0
#define BMP280_REG_STATUS      0xF3
#define BMP280_REG_CTRL_MEAS   0xF4
#define BMP280_REG_CONFIG      0xF5
#define BMP280_REG_PRESS_MSB   0xF7
#define BMP280_REG_TEMP_MSB    0xFA //register adresleri
#define BMP280_REG_CALIB_00    0x88
#define BMP280_CHIP_ID         0x58
#define BMP280_TIMEOUT         100

typedef enum {
    BMP280_OVERSAMPLING_SKIP = 0x00,
    BMP280_OVERSAMPLING_1X   = 0x01,
    BMP280_OVERSAMPLING_16X  = 0x05
} BMP280_Oversampling;

typedef enum {
    BMP280_MODE_SLEEP  = 0x00,
    BMP280_MODE_NORMAL = 0x03
} BMP280_Mode;

typedef enum {
    BMP280_FILTER_OFF = 0x00,
    BMP280_FILTER_16  = 0x04
} BMP280_Filter;

typedef enum {
    BMP280_STANDBY_500MS = 0x04
} BMP280_Standby;

typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
    int32_t  t_fine;
} BMP280_CalibData;

typedef struct {
    I2C_HandleTypeDef *hi2c;
    uint8_t addr;
    BMP280_CalibData calib;
} BMP280_Handle;

HAL_StatusTypeDef BMP280_Init(BMP280_Handle *bmp, I2C_HandleTypeDef *hi2c, uint8_t addr);
HAL_StatusTypeDef BMP280_ReadID(BMP280_Handle *bmp, uint8_t *id);
HAL_StatusTypeDef BMP280_Reset(BMP280_Handle *bmp);
HAL_StatusTypeDef BMP280_SetConfig(BMP280_Handle *bmp, BMP280_Filter filter, BMP280_Standby standby);
HAL_StatusTypeDef BMP280_SetMode(BMP280_Handle *bmp, BMP280_Mode mode, BMP280_Oversampling temp_os, BMP280_Oversampling press_os);
HAL_StatusTypeDef BMP280_ReadAll(BMP280_Handle *bmp, float *temperature, float *pressure);

#endif

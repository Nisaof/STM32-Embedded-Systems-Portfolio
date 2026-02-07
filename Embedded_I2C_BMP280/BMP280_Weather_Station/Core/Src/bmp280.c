/* bmp280.c - BMP280 Sensör Sürücüsü (Stabil Versiyon) */
#include "bmp280.h"
#include <string.h>

#define BMP280_TIMEOUT 300  // Artırıldı (önceden 100)

static HAL_StatusTypeDef BMP280_WriteReg(BMP280_Handle *bmp, uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    return HAL_I2C_Master_Transmit(bmp->hi2c, bmp->addr << 1, data, 2, BMP280_TIMEOUT);
}

static HAL_StatusTypeDef BMP280_ReadReg(BMP280_Handle *bmp, uint8_t reg, uint8_t *data, uint16_t len) {
    return HAL_I2C_Mem_Read(bmp->hi2c, bmp->addr << 1, reg, 1, data, len, BMP280_TIMEOUT);
}

static HAL_StatusTypeDef BMP280_ReadCalibration(BMP280_Handle *bmp) {
    uint8_t calib[24];
    HAL_StatusTypeDef status = BMP280_ReadReg(bmp, BMP280_REG_CALIB_00, calib, 24);
    if (status != HAL_OK) return status;

    bmp->calib.dig_T1 = (calib[1] << 8) | calib[0];
    bmp->calib.dig_T2 = (calib[3] << 8) | calib[2];
    bmp->calib.dig_T3 = (calib[5] << 8) | calib[4];
    bmp->calib.dig_P1 = (calib[7] << 8) | calib[6];
    bmp->calib.dig_P2 = (calib[9] << 8) | calib[8];
    bmp->calib.dig_P3 = (calib[11] << 8) | calib[10];
    bmp->calib.dig_P4 = (calib[13] << 8) | calib[12];
    bmp->calib.dig_P5 = (calib[15] << 8) | calib[14];
    bmp->calib.dig_P6 = (calib[17] << 8) | calib[16];
    bmp->calib.dig_P7 = (calib[19] << 8) | calib[18];
    bmp->calib.dig_P8 = (calib[21] << 8) | calib[20];
    bmp->calib.dig_P9 = (calib[23] << 8) | calib[22];

    return HAL_OK;
}

static int32_t BMP280_CompensateTemp(BMP280_Handle *bmp, int32_t adc_T) {
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t)bmp->calib.dig_T1 << 1))) *
            ((int32_t)bmp->calib.dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)bmp->calib.dig_T1)) *
              ((adc_T >> 4) - ((int32_t)bmp->calib.dig_T1))) >> 12) *
            ((int32_t)bmp->calib.dig_T3)) >> 14;

    bmp->calib.t_fine = var1 + var2;
    T = (bmp->calib.t_fine * 5 + 128) >> 8;

    return T;
}

static uint32_t BMP280_CompensatePressure(BMP280_Handle *bmp, int32_t adc_P) {
    int64_t var1, var2, p;

    var1 = ((int64_t)bmp->calib.t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)bmp->calib.dig_P6;
    var2 = var2 + ((var1 * (int64_t)bmp->calib.dig_P5) << 17);
    var2 = var2 + (((int64_t)bmp->calib.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)bmp->calib.dig_P3) >> 8) +
           ((var1 * (int64_t)bmp->calib.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)bmp->calib.dig_P1) >> 33;

    if (var1 == 0) return 0;

    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)bmp->calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)bmp->calib.dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)bmp->calib.dig_P7) << 4);

    return (uint32_t)p;
}

/* ========================== Public Fonksiyonlar ========================== */

HAL_StatusTypeDef BMP280_Init(BMP280_Handle *bmp, I2C_HandleTypeDef *hi2c, uint8_t addr) {
    bmp->hi2c = hi2c;
    bmp->addr = addr;

    uint8_t id;
    if (BMP280_ReadID(bmp, &id) != HAL_OK || id != BMP280_CHIP_ID) {
        return HAL_ERROR;
    }

    if (BMP280_Reset(bmp) != HAL_OK) return HAL_ERROR;
    HAL_Delay(10);

    if (BMP280_ReadCalibration(bmp) != HAL_OK) return HAL_ERROR;

    BMP280_SetConfig(bmp, BMP280_FILTER_16, BMP280_STANDBY_500MS);
    BMP280_SetMode(bmp, BMP280_MODE_NORMAL,
                   BMP280_OVERSAMPLING_16X, BMP280_OVERSAMPLING_16X);

    // Sensörün stabilize olması için bekle
    HAL_Delay(100);

    // Reset sonrası mode tekrar aktif olsun
    BMP280_SetMode(bmp, BMP280_MODE_NORMAL,
                   BMP280_OVERSAMPLING_16X, BMP280_OVERSAMPLING_16X);
    HAL_Delay(25);

    return HAL_OK;
}

HAL_StatusTypeDef BMP280_ReadID(BMP280_Handle *bmp, uint8_t *id) {
    return BMP280_ReadReg(bmp, BMP280_REG_CHIP_ID, id, 1);
}

HAL_StatusTypeDef BMP280_Reset(BMP280_Handle *bmp) {
    return BMP280_WriteReg(bmp, BMP280_REG_RESET, 0xB6);
}

HAL_StatusTypeDef BMP280_SetConfig(BMP280_Handle *bmp, BMP280_Filter filter, BMP280_Standby standby) {
    uint8_t config = (standby << 5) | (filter << 2);
    return BMP280_WriteReg(bmp, BMP280_REG_CONFIG, config);
}

HAL_StatusTypeDef BMP280_SetMode(BMP280_Handle *bmp, BMP280_Mode mode,
                                 BMP280_Oversampling temp_os, BMP280_Oversampling press_os) {
    uint8_t ctrl = (temp_os << 5) | (press_os << 2) | mode;
    return BMP280_WriteReg(bmp, BMP280_REG_CTRL_MEAS, ctrl);
}

/* Ölçüm Fonksiyonları */

HAL_StatusTypeDef BMP280_ReadAll(BMP280_Handle *bmp, float *temperature, float *pressure) {
    uint8_t data[6];
    HAL_StatusTypeDef status = BMP280_ReadReg(bmp, BMP280_REG_PRESS_MSB, data, 6);

    if (status != HAL_OK) return status;

    int32_t adc_P = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    int32_t adc_T = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);

    int32_t temp = BMP280_CompensateTemp(bmp, adc_T);
    uint32_t press = BMP280_CompensatePressure(bmp, adc_P);

    *temperature = temp / 100.0f;
    *pressure = press / 256.0f / 100.0f;

    return HAL_OK;
}


/*
 * lis3dh.c
 * LIS3DH 3-axis accelerometer driver implementation
 */

#include "lis3dh.h"

/**
 * @brief Read a register from LIS3DH
 * @param hspi: Pointer to SPI handle
 * @param reg: Register address to read
 * @return Register value
 */
uint8_t LIS3DH_ReadRegister(SPI_HandleTypeDef *hspi, uint8_t reg) {
    uint8_t tx_data = reg | LIS3DH_READ_BIT;
    uint8_t rx_data = 0;

    LIS3DH_CS_LOW();
    HAL_SPI_Transmit(hspi, &tx_data, 1, 100);
    HAL_SPI_Receive(hspi, &rx_data, 1, 100);
    LIS3DH_CS_HIGH();

    return rx_data;
}

/**
 * @brief Write a register to LIS3DH
 * @param hspi: Pointer to SPI handle
 * @param reg: Register address to write
 * @param value: Value to write
 */
void LIS3DH_WriteRegister(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t value) {
    uint8_t tx_data[2];
    tx_data[0] = reg & ~LIS3DH_READ_BIT;  // Write bit = 0
    tx_data[1] = value;

    LIS3DH_CS_LOW();
    HAL_SPI_Transmit(hspi, tx_data, 2, 100);
    LIS3DH_CS_HIGH();
}

/**
 * @brief Initialize LIS3DH sensor
 * @param hspi: Pointer to SPI handle
 * @return Device ID (should be 0x33 if successful, 0 if failed)
 */
uint8_t LIS3DH_Init(SPI_HandleTypeDef *hspi) {
    HAL_Delay(10); // Power-up delay

    // Read WHO_AM_I register to verify device
    uint8_t who_am_i = LIS3DH_ReadRegister(hspi, LIS3DH_WHO_AM_I);

    if (who_am_i != LIS3DH_ID) {
        return 0; // Initialization failed
    }

    // Configure CTRL_REG1:
    // ODR = 100Hz (0101), Normal mode, X/Y/Z axes enabled
    // Binary: 0101 0111 = 0x57
    LIS3DH_WriteRegister(hspi, LIS3DH_CTRL_REG1, 0x57);

    // Configure CTRL_REG4:
    // ±2g full scale, High resolution mode, BDU enabled
    // Binary: 0000 1000 = 0x08
    LIS3DH_WriteRegister(hspi, LIS3DH_CTRL_REG4, 0x08);

    HAL_Delay(10); // Wait for configuration

    return who_am_i;
}

/**
 * @brief Read acceleration data from all three axes
 * @param hspi: Pointer to SPI handle
 * @param data: Pointer to LIS3DH_Data structure to store results
 */
void LIS3DH_ReadAccel(SPI_HandleTypeDef *hspi, LIS3DH_Data *data) {
    uint8_t tx_data = LIS3DH_OUT_X_L | LIS3DH_READ_BIT | LIS3DH_MULTI_BIT;
    uint8_t rx_data[6];

    // Read all 6 bytes (X_L, X_H, Y_L, Y_H, Z_L, Z_H) in one transaction
    LIS3DH_CS_LOW();
    HAL_SPI_Transmit(hspi, &tx_data, 1, 100);
    HAL_SPI_Receive(hspi, rx_data, 6, 100);
    LIS3DH_CS_HIGH();

    // Combine low and high bytes for each axis
    data->x = (int16_t)((rx_data[1] << 8) | rx_data[0]);
    data->y = (int16_t)((rx_data[3] << 8) | rx_data[2]);
    data->z = (int16_t)((rx_data[5] << 8) | rx_data[4]);

    // Convert raw values to g (gravity units)
    // For ±2g scale with 16-bit resolution: sensitivity = 4mg/LSB
    // Full scale: 32768 LSB = 2g, so 1g = 16384 LSB
    data->x_g = data->x / 16384.0f;
    data->y_g = data->y / 16384.0f;
    data->z_g = data->z / 16384.0f;
}

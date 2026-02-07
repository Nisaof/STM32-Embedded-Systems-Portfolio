# STM32 Embedded Systems Portfolio

This repository is a structured embedded systems portfolio demonstrating hands-on experience with STM32 microcontrollers over a focused development period.  
Each project targets a specific peripheral or communication protocol and is designed with clean architecture, modular drivers, and interrupt-driven logic.

---

## Project Overview

The portfolio covers both **high-performance** and **mainstream** STM32 families and focuses on low-level peripheral control using **STM32 HAL** and **CubeMX**.

---

## 📂 Project Structure
STM32-Embedded-Systems-Portfolio/
│
├── Embedded_GPIO_Interrupts/
│ └── Timer-based LED control using TIM interrupts (non-blocking design)
│
├── Embedded_UART_Interface/
│ └── Interrupt-driven UART communication with serial terminal interaction
│
├── Embedded_ADC_Temperature/
│ └── Internal temperature sensor monitoring using factory calibration values
│
├── Embedded_I2C_BMP280/
│ └── I2C communication with BMP280 sensor for pressure, temperature & altitude
│
├── Embedded_SPI_LIS3DH/
│ └── SPI-based 3-axis accelerometer interface with modular driver design
│
├── Embedded_DAC_Sine_Generator/
│ └── 12-bit DAC sine wave generation with ADC-based signal verification
│
└── README.md


---

## 🛠 Hardware & MCU Details

| Project | MCU | Core | Board |
|------|-----|------|------|
GPIO & Interrupts | STM32H563ZI | Cortex-M33 | NUCLEO-H563ZI |
UART Interface | STM32H563ZI | Cortex-M33 | NUCLEO-H563ZI |
ADC Temperature | STM32H563ZI | Cortex-M33 | NUCLEO-H563ZI |
I2C BMP280 | STM32F767ZITX | Cortex-M7 | NUCLEO-F767ZI |
SPI LIS3DH | STM32F767ZITX | Cortex-M7 | NUCLEO-F767ZI |
DAC Sine Generator | STM32F767ZITX | Cortex-M7 | NUCLEO-F767ZI |

---

## ⚙️ Development Environment

- **IDE:** STM32CubeIDE  
- **Framework:** STM32 HAL  
- **Configuration Tool:** STM32CubeMX  
- **Programming Interface:** ST-Link  

---

## 🚀 Build & Flash Instructions

1. Open the desired project folder inside **STM32CubeIDE**
2. If needed, open the `.ioc` file and click **Generate Code**
3. Build the project  
4. Connect the NUCLEO board via USB
5. Flash using **Run → Debug** or **Run → Run**

---

## 🧩 Design Principles

- Interrupt-driven architecture (no busy-wait loops)
- Modular peripheral drivers
- Clear separation of application logic and hardware abstraction
- Portable and reusable code structure

---
## 🏆 Capstone / Final Project

### Accelerometer-Based Digital Level (Digital Spirit Level)

This project serves as the standalone capstone of this STM32 embedded systems portfolio.

The system operates as a **digital spirit level**, measuring tilt angles in real time using a 3-axis accelerometer and transmitting processed data via UART.

#### 🔧 Technical Overview
- **Sensor:** LIS3DH 3-axis accelerometer  
- **Communication Protocol:** SPI  
- **MCU:** STM32F767ZITX (Cortex-M7)  
- **Board:** NUCLEO-F767ZI  
- **Output:** UART (real-time tilt data logging)

#### 🧠 Key Features
- Accurate tilt angle calculation using raw accelerometer data  
- Modular SPI driver implementation  
- Interrupt-safe and non-blocking firmware design  
- Clean separation between sensor drivers and application logic  

#### 📁 Repository
🔗 https://github.com/Nisaof/STM32Accelerometer-Based-Digital-Level

This project consolidates GPIO, SPI, UART, and sensor interfacing knowledge into a real-world embedded system application.

---

## 🏁 Notes

This repository is intended as a **technical portfolio** showcasing embedded systems fundamentals and practical STM32 experience.  
Each project is self-contained and documented within its own directory.

---



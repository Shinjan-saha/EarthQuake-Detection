# Earthquake Detection System (Arduino + MPU6050 + OLED + Buzzer)

This project implements a **low-cost earthquake detection system** using an **Arduino Nano**, **MPU6050 accelerometer + gyroscope**, **OLED display**, **buzzer**, and **LED**.  
The system uses an **RMS (Root Mean Square) based algorithm** to continuously monitor vibrations and trigger an alert when abnormal seismic activity is detected.  

---
## Circuit
<img src="https://i.postimg.cc/wMWjjH6J/temp-Image-JQD5f-N.avif">

---

## Working

![Demo](working.gif)


## 🛠️ Components Used
- **Arduino Nano / Uno / Pro Mini**  
- **MPU6050 (Accelerometer + Gyroscope)**  
- **0.96\" SPI OLED Display (SSD1306, 128x64 resolution)**  
- **Buzzer (Active or Passive)**  
- **LED**  
- **Resistors, Breadboard, and Jumper Wires**  

---

## 📚 Libraries Used
Make sure you install the following libraries from the Arduino IDE Library Manager:
- [`Wire.h`](https://www.arduino.cc/en/reference/wire) → I2C communication  
- [`I2Cdev.h`](https://github.com/jrowberg/i2cdevlib) → MPU6050 support  
- [`MPU6050.h`](https://github.com/jrowberg/i2cdevlib) → IMU driver  
- [`Adafruit_GFX.h`](https://github.com/adafruit/Adafruit-GFX-Library) → Graphics library for OLED  
- [`Adafruit_SSD1306.h`](https://github.com/adafruit/Adafruit_SSD1306) → OLED display driver  
- [`SPI.h`](https://www.arduino.cc/en/reference/SPI) → Communication with OLED (SPI mode)  

---

## ⚙️ RMS-Based Detection System

### 🔹 What is RMS?
RMS (**Root Mean Square**) is a statistical measure used to calculate the **average vibration energy** over a moving time window.  
Instead of relying on single-sample spikes (which may be noise), RMS provides a **smoothed measure of seismic activity**.

### 🔹 How it Works
1. The **MPU6050** measures acceleration in X, Y, Z directions.  
2. The **magnitude** of acceleration is calculated as:  

   ```
   mag = sqrt(Ax² + Ay² + Az²)
   ```

3. The system stores these values in a **sliding buffer** (window).  
4. RMS is computed over the buffer:  

   ```
   RMS = sqrt((Σ mag²) / N)
   ```

   where **N = RMS_WINDOW** (number of samples).  

5. If the RMS value **exceeds a set threshold**, an earthquake is detected.  
6. The **buzzer & LED** are activated, and the OLED displays `"EARTHQUAKE!"`.

---

## 🚨 Output
- **Normal Mode:**  
  Displays acceleration values (Ax, Ay, Az) and magnitude.  
  LED and buzzer remain off.  

- **Earthquake Detected:**  
  OLED shows **"EARTHQUAKE!"**, LED blinks, and buzzer sounds.  

---

## 📷 System Diagram
```
Arduino Nano
   |
   |-- MPU6050 (SDA, SCL)
   |-- OLED SSD1306 (SPI: CS, DC, RESET, MOSI, CLK)
   |-- Buzzer (D4)
   |-- LED (D7)
```

---

## ⚡ Parameters to Tune
- **`RMS_WINDOW`** → number of samples used in RMS calculation (default: 10)  
- **`rmsThreshold`** → threshold above which an earthquake is detected (default: 1.1 g)  
- **`SAMPLE_INTERVAL_MS`** → sampling rate in milliseconds (default: 100ms → 10Hz)  

---

## 🖥️ Serial Monitor Example
```
Ax: 0.02 | Ay: -0.01 | Az: 1.00 | Mag: 1.00
RMS: 1.02
Ax: 0.05 | Ay: -0.02 | Az: 0.98 | Mag: 1.01
RMS: 1.03
!!! EARTHQUAKE DETECTED (RMS) !!!
```

---



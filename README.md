# DIY Oscilloscope using ESP32 & OLED Display

A compact, low-cost digital oscilloscope built using the ESP32 microcontroller and dual 0.96-inch OLED displays. This project captures analog waveforms, displays them in real time, and measures key parameters like frequency, Vpp (peak-to-peak voltage), and DC voltage.

![image](https://github.com/user-attachments/assets/f2ff455c-cb78-483e-a6f4-0dd1d2335147)

## 🔧 Features

- 📉 Real-time waveform display on OLED
- ⏱ Frequency measurement (Hz) with high accuracy
- ⚡ Peak-to-peak voltage (Vpp) and DC voltage (VDC) measurement
- 🎛 Potentiometer-controlled scaling
- 🧲 Signal hold/capture button
- 📺 Dual OLED UI: Left for values, Right for waveform
- ✅ Verified against lab-grade oscilloscope (Scientech 801)

---

## 🧠 How It Works

The ESP32 reads the analog signal using its ADC, samples it in real time, and:
- Plots the waveform on one OLED screen.
- Calculates and displays frequency, Vpp, and VDC on the second OLED.
- Uses interrupts and sampling logic for accurate frequency measurement.

---

## 🧰 Hardware Used

- [x] ESP32 Dev Module
- [x] 2x 0.96" OLED Display (SSD1306 I2C)
- [x] Potentiometer (10kΩ)
- [x] Push Buttons (for HOLD / trigger options)
- [x] Voltage Divider Circuit
- [x] Function Generator (Scientech 4061)
- [x] Analog Oscilloscope (Scientech 801)
- [x] Breadboard + Jumper Wires
- [x] TLC084 or other op-amp (for signal conditioning)

---

## 🔌 Wiring Overview

| Component        | ESP32 Pin  |
|------------------|------------|
| OLED1 (Value)    | SDA, SCL   |
| OLED2 (Graph)    | SDA, SCL   |
| Potentiometer    | ADC (e.g., GPIO34) |
| Signal Input     | ADC (e.g., GPIO35) |
| Button (Capture) | GPIO (e.g., GPIO26) |

> Use a voltage divider if your input exceeds 3.3V. For higher voltage input (up to 30V), buffer and scale the signal.

---

## 📸 Screenshots

### OLED Display Output
![image](https://github.com/user-attachments/assets/7dea8325-d704-49e5-b97d-32a34969b034)



### Waveform Validation on Analog Oscilloscope
![image](https://github.com/user-attachments/assets/a58cdcf2-4a8c-4cf2-b1ab-bbf7b36e67d1)
![image](https://github.com/user-attachments/assets/666b0819-7a3f-4637-85f4-0da2d8f1640c)


---

## 🚀 Getting Started

### 📁 Clone this repo
```bash
git clone https://github.com/yourusername/esp32-oscilloscope.git

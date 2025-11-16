# POSTURE SMART — Wearable Posture, Focus, and Break Monitoring Device

A wearable wellness assistant designed using the **Adafruit Circuit Playground Classic**, built to help students develop healthy study habits. Posture Smart monitors your posture, tracks ambient noise, and reminds you to take breaks, creating an optimized environment for productivity and well-being.

---

## Overview

Poor posture, high noise levels, and lack of regular breaks are major barriers to study efficiency and long-term well-being.  
**Posture Smart** is a wearable device that addresses all three by using built-in sensors on the Adafruit Circuit Playground to:

- Detect poor posture  
- Monitor sound levels  
- Remind users to take breaks  
- Provide intuitive configuration through onboard buttons and LEDs  

This project demonstrates the practical use of **C++**, **embedded systems**, and **sensor integration** to improve daily health and productivity.

---

## Project Features

### 1. Posture Corrector
Detects significant deviations from the normal Z-axis gravitational value (9.8 m/s²).  
If poor posture is detected:

- All LEDs flash red  
- A 1000 Hz alert tone is played  
- Alerts are suppressed during “mute periods”

**Core Logic**
- Uses `CircuitPlayground.motionZ()`
- Converts raw G value → m/s²  
- Checks deviation against `zThreshold`

---

### 2. Sound Level Detector
Continuously reads ambient sound pressure levels and:

- Displays sound intensity on the LED ring  
- Calculates average sound over a fixed window  
- Triggers a double-beep alert with red LEDs for high noise levels  

**Real-life example**  
A student studying in a library receives an alert if the environment becomes too noisy to focus.

---

### 3. Break Reminder (Pomodoro)
Encourages healthy study habits using regular break reminders.

When the interval is reached:

- Triple beep  
- All LEDs flash red  
- Device enters a temporary **mute state** to avoid overlapping alerts

---

### 4. User Interaction
Users can intuitively configure features using onboard buttons:

- **Left button:** Toggle selected feature ON/OFF  
- **Right button:** Confirm selection and move to the next feature  
- **Hold both buttons for 2+ seconds:** Reset and return to configuration mode  

**Features & LEDs**
| LED | Feature              |
|-----|----------------------|
| 0   | Posture Corrector    |
| 1   | Sound Level Detector |
| 2   | Break Reminder       |

---

## System Architecture

The system uses three OOP classes:

- `PostureCorrector`
- `SoundLevelDetector`
- `BreakReminder`

Each class includes:

- Configuration parameters  
- A continuous `update()` loop  
- LED + audio feedback  
- Built-in suppression during mute periods  

Main loop handles reset logic, mute timing, and the sequential calling of feature updates.

---

## Hardware & Tools

### Adafruit Circuit Playground Classic
Used components:

- 3-axis accelerometer  
- Microphone  
- 10 RGB NeoPixel LEDs  
- Two push buttons  
- Onboard speaker  

### Software Tools
- **Arduino IDE**  
- **C++** (object-oriented approach)  

---

## Pseudocode Summary


Feature logic includes:  
- Z-axis threshold detection  
- Sound averaging and LED visualization  
- Pomodoro break timing  

---

## Testing & Evaluation

### Posture Detection
- Monitored Z-axis values with Serial Monitor  
- Alerts reliably triggered when user slouched beyond threshold  

### Sound Level Detection
- Verified accuracy of SPL readings  
- Debugged mapping of SPL → LED pattern  
- Fixed averaging inconsistencies  

### Break Reminder
- Tested short intervals to verify timing  
- Ensured mute period correctly suppresses alerts  

### User Interaction
- Verified button debounce logic  
- Ensured seamless configuration transitions  

**Conclusion:**  
All features function reliably, even when enabled simultaneously. The system is stable, responsive, and intuitive.

---

## Future Improvements

- Add rechargeable battery for portability  
- Add health sensors (SpO2, heart-rate, pressure)  
- Create protective and wearable casing  
- Develop mobile companion app for analytics  
- Branding and marketing strategy for student wellness product  

---

## Team
**Developed by:**  
- Ibrahim Bucktowar  
- Shadad Hossain  
- Sarfraz Binushkhan  

---

## References

- Adafruit Learning System – Circuit Playground Overview  
- Arduino Documentation – Debounce & Serial Monitor  
- Cirillo, F. – The Pomodoro Technique  
- Cleveland Clinic – Importance of Good Posture  
- Hongisto et al. – Noise and concentration  
- Peper et al. – Posture and mental state  
- Lompoc Medical Center – Health effects of posture  



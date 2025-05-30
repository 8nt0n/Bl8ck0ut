# ⚠️ Wiring Disclaimer

The following wiring instructions are provided **solely for educational and experimental purposes**. Please ensure you:

- Understand and comply with all local laws and regulations regarding RF devices before assembling or powering this project.
- Only build and test this hardware in a controlled environment where you have **explicit permission** to do so.
- Avoid causing interference or harm to any wireless devices, networks, or individuals.

**Use this information responsibly and ethically. If you're unsure about the legality or safety of your setup, please do not proceed.**

---


## 🧠 1st ESP32 - Signal Transmitter

### 📡 nRF24 Modules

Wire the two nRF24L01+ modules to the 1st ESP32 as follows:

#### ▶️ VSPI Bus (Module 1)

| **Signal** | **nRF24L01+ Pin** | **ESP32 Pin** |
| ---------- | ----------------- | ------------- |
| CE         | CE                | GPIO 22       |
| CSN (CS)   | CSN               | GPIO 21       |
| SCK        | SCK               | GPIO 18       |
| MOSI       | MOSI              | GPIO 23       |
| MISO       | MISO              | GPIO 19       |
| VCC        | +3.3V             | 3.3V          |
| GND        | GND               | GND           |


#### ▶️ HSPI Bus (Module 2)

| **Signal** | **nRF24L01+ Pin** | **ESP32 Pin** |
| ---------- | ----------------- | ------------- |
| CE         | CE                | GPIO 16       |
| CSN (CS)   | CSN               | GPIO 15       |
| SCK        | SCK               | GPIO 14       |
| MOSI       | MOSI              | GPIO 13       |
| MISO       | MISO              | GPIO 12       |
| VCC        | +3.3V             | 3.3V          |
| GND        | GND               | GND           |


---


### 🖥️ I²C OLED Display

Wire the display to the ESP32 like this:

| **Display Pin** | **ESP32 Pin** |
| --------------- | ------------- |
| GND             | GND           |
| VCC             | 5V            |
| SCL             | GPIO 5        |
| SDA             | GPIO 4        |


---


### ⚡ Power

- Connect the **5V** and **GND** pins of the 1st ESP32 to a power rail on the breadboard.
- **Note:** This power rail will later be **controlled by the relay** from the 2nd ESP32 (acting as a timed power switch).


---


### 🧩 Final Result (1st ESP32)

> The completed wiring for this board will look like:
> ***[In work]**


---


## 🔌 2nd ESP32 - Relay Controller

Wire the relay module to the second ESP32 like this:

| **Relay Module Pin** | **Connect To**               | **Description**                                                              |
|----------------------|------------------------------|------------------------------------------------------------------------------|
| VCC                  | 5V (ESP32 or external)       | Power for the relay (confirm voltage requirement)                           |
| GND                  | GND                          | Shared ground with the ESP32                                                |
| IN                   | GPIO (e.g., GPIO 4)          | Control signal from the ESP32                                               |
| COM                  | 5V (relay input)             | Common pin — connects to your voltage source                                |
| NO                   | +5V rail for 1st ESP32       | "Normally Open" — connects to COM when relay is active                      |


---


### 🔄 Shared Ground

- Connect the **GND of the 2nd ESP32** to the **GND rail of the 1st ESP32** using a jumper wire.


---


### 🧩 Final Result (2nd ESP32)

> The completed wiring for this board will look like:
> **[In work]**

---

/*
  bl8ck0ut - Educational RF Signal Experimentation Platform
  --------------------------------------------------------

  Author: 8nt0n
  GitHub: https://github.com/8nt0n/bl8ck0ut
  License: MIT License (See LICENSE file)

  ⚠️ LEGAL NOTICE ⚠️

  This code is strictly for educational, research, and demonstration purposes only.
  It must NOT be used to interfere with wireless communications, disrupt networks,
  or violate any local, national, or international laws.

  Unauthorized transmission on RF bands can result in serious legal consequences.

  Before using this code or hardware in any real RF transmission, ALWAYS:

    1. Obtain explicit permission to operate.
    2. Ensure compliance with all applicable laws and regulations in your jurisdiction.
    3. Confirm that no other devices, networks, or people will be affected.

  For full legal disclaimer and ethical guidelines, see the "Legal Notice" and "Ethics & Responsibility" guide on the top of the README

  If unsure about legality or ethics: DO NOT USE.

  --------------------------------------------------------
*/

#include <SPI.h>
#include "RF24.h"
#include "esp_bt.h"
#include "esp_wifi.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1  // No reset pin
#define I2C_SDA        4   // I2C SDA pin
#define I2C_SCL        5   // I2C SCL pin

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// SPI buses for NRF24L01+
SPIClass *spiHSPI = nullptr;
SPIClass *spiVSPI = nullptr;

// NRF24L01+ radios on HSPI and VSPI buses
RF24 radioHSPI(16, 15, 16000000);  // CE=16, CS=15
RF24 radioVSPI(22, 21, 16000000);  // CE=22, CS=21

// Channel tracking variables and direction flags
int channelHSPI = 45;
int channelVSPI = 45;
bool directionHSPIUp = true;
bool directionVSPIUp = true;

/**
 * @brief Incremental channel sweep function for both radios
 * 
 * This function sweeps the RF channels up and down within the valid range
 * to simulate a signal pattern without transmitting meaningful data.
 */
void channelSweep() {
  // Adjust VSPI channel
  if (directionVSPIUp) {
    channelVSPI += 4;
    if (channelVSPI > 79) directionVSPIUp = false;
  } else {
    channelVSPI -= 4;
    if (channelVSPI < 2) directionVSPIUp = true;
  }

  // Adjust HSPI channel
  if (directionHSPIUp) {
    channelHSPI += 2;
    if (channelHSPI > 79) directionHSPIUp = false;
  } else {
    channelHSPI -= 2;
    if (channelHSPI < 2) directionHSPIUp = true;
  }

  radioHSPI.setChannel(channelHSPI);
  radioVSPI.setChannel(channelVSPI);

  // Debug output
  /*
  Serial.print("VSPI channel: ");
  Serial.println(channelVSPI);
  Serial.print("HSPI channel: ");
  Serial.println(channelHSPI);
  */
}

/**
 * @brief Randomized channel function - Jammer behavior
 * 
 * WARNING: This function generates randomized radio transmissions
 * across all channels. It is **commented out by default** due to legal and ethical risks.
 * 
 * Uncomment ONLY if you have explicit permission and are fully compliant with local laws.
 */
void randomJammer() {
  radioHSPI.setChannel(random(80));
  radioVSPI.setChannel(random(80));
  delayMicroseconds(random(60));  // Optional delay to reduce transmission rate
}

void setup() {
  Serial.begin(115200);

  // Disable Bluetooth and WiFi to avoid interference and for regulatory safety
  esp_bt_controller_deinit();
  esp_wifi_stop();
  esp_wifi_deinit();
  esp_wifi_disconnect();

  // Initialize OLED display
  Wire.begin(I2C_SDA, I2C_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);  // Halt if OLED init fails
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.println(F("Bl8ck0ut"));
  display.setTextSize(1);
  display.println(F("HSPI: init"));
  display.println(F("VSPI: init"));
  display.display();

  // Initialize radios on SPI buses
  while (!initRadioHSPI()) delay(100);
  while (!initRadioVSPI()) delay(100);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.println(F("Bl8ck0ut"));
  display.setTextSize(1);
  display.println(F("HSPI: ready"));
  display.println(F("VSPI: ready"));
  display.display();
}

/**
 * @brief Initialize HSPI radio module
 */
bool initRadioHSPI() {
  spiHSPI = new SPIClass(HSPI);
  spiHSPI->begin();

  if (radioHSPI.begin(spiHSPI)) {
    Serial.println("HSPI radio started.");
    radioHSPI.setAutoAck(false);
    radioHSPI.stopListening();
    radioHSPI.setRetries(0, 0);
    radioHSPI.setPALevel(RF24_PA_MAX, true);
    radioHSPI.setDataRate(RF24_2MBPS);
    radioHSPI.setCRCLength(RF24_CRC_DISABLED);
    radioHSPI.printPrettyDetails();
    radioHSPI.startConstCarrier(RF24_PA_MAX, channelHSPI);
    return true;
  } else {
    Serial.println("Failed to start HSPI radio.");
    return false;
  }
}

/**
 * @brief Initialize VSPI radio module
 */
bool initRadioVSPI() {
  spiVSPI = new SPIClass(VSPI);
  spiVSPI->begin();

  if (radioVSPI.begin(spiVSPI)) {
    Serial.println("VSPI radio started.");
    radioVSPI.setAutoAck(false);
    radioVSPI.stopListening();
    radioVSPI.setRetries(0, 0);
    radioVSPI.setPALevel(RF24_PA_MAX, true);
    radioVSPI.setDataRate(RF24_2MBPS);
    radioVSPI.setCRCLength(RF24_CRC_DISABLED);
    radioVSPI.printPrettyDetails();
    radioVSPI.startConstCarrier(RF24_PA_MAX, channelVSPI);
    return true;
  } else {
    Serial.println("Failed to start VSPI radio.");
    return false;
  }
}

void loop() {
  // WARNING: By default, jammer function is DISABLED.
  // Uncomment the below line only if you have explicit legal permission.

  // randomJammer();

  // For safe default operation, perform harmless channel sweep instead
  channelSweep();
}

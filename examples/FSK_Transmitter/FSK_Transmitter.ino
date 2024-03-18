
// These examples are from the Electronics Cookbook by Simon Monk
//https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
// mod by Little_S@tan

#include <Arduino.h>
#include "ELECHOUSE_CC1101_SRC_DRV.h"

#define CC1101_GDO0 2
#define CC1101_GDO2 4

int gdo0;

const int n = 17;
byte buffer[n] = {0x51, 0x00, 0x6B, 0x58, 0x6E, 0x7F, 0x24, 0xF8, 0xD2, 0xFF, 0xFF, 0xFF, 0x3C, 0x28, 0x80, 0x00, 0x00};
byte len = 17;

void setup()
{

#ifdef ESP32
  gdo0 = CC1101_GDO0; // for esp32! GDO0 on GPIO pin 2.
#elif ESP8266
  gdo0 = 5; // for esp8266! GDO0 on pin 5 = D1.
#else
  gdo0 = 6; // for Arduino! GDO0 on pin 6.
#endif

  Serial.begin(921600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  if (ELECHOUSE_cc1101.getCC1101())
  { // Check the CC1101 Spi connection.
    Serial.println("Connection OK");
  }
  else
  {
    Serial.println("Connection Error");
  }

  ELECHOUSE_cc1101.Init(); // must be set to initialize the cc1101!

  Serial.println("Tx Mode");
}

void loop()
{
  ELECHOUSE_cc1101.SetTx();
  digitalWrite(LED_BUILTIN, HIGH);
  ELECHOUSE_cc1101.SendData(buffer, len, 100);
  Serial.print("Buffer: ");
  for (int i = 0; i < len; i++)
  {
    Serial.print(buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.print("len: ");
  Serial.println(len);
  ELECHOUSE_cc1101.SetRx();
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000); // 15 seconds
  /*   if (Serial.available())
  {
    int len = Serial.readBytesUntil('\n', buffer, n);
    buffer[len] = '\0';
    Serial.println((char *)buffer);
    ELECHOUSE_cc1101.SendData(buffer, len);

    Serial.print("Buffer: ");
    for (int i = 0; i < len; i++)
    {
      Serial.println(buffer[i]);
    }
    Serial.print("len: ");
    Serial.println(len);
  } */
}

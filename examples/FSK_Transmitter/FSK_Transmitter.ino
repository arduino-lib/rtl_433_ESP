
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

/* #include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <RFControl.h>

#define RECEIVER_PIN 4  // any intterupt able pin


void setup() {

  Serial.begin(115200);

  if (ELECHOUSE_cc1101.getCC1101())
  { // Check the CC1101 Spi connection.
    Serial.println("Connection OK");
  }
  else
  {
    Serial.println("Connection Error");
  }
  
  // Need to setup CC1101 for asyc receiver
  // define input pin
  
  ELECHOUSE_cc1101.Init();                // must be set to initialize the cc1101!
  ELECHOUSE_cc1101.setGDO(RECEIVER_PIN,0);
  ELECHOUSE_cc1101.setCCMode(0);          // set config for async receiver
  ELECHOUSE_cc1101.setMHZ(433.92);        // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
  ELECHOUSE_cc1101.setChannel(0);         // Set the Channelnumber from 0 to 255. Default is cahnnel 0.

  // Do these work in Async MOde??
  ELECHOUSE_cc1101.setModulation(0);      // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
  ELECHOUSE_cc1101.setDeviation(5.15);   // Set the Frequency deviation in kHz. Value from 1.58 to 380.85. Default is 47.60 kHz.
  ELECHOUSE_cc1101.setChsp(199.95);       // The channel spacing is multiplied by the channel number CHAN and added to the base frequency in kHz. Value from 25.39 to 405.45. Default is 199.95 kHz.
  ELECHOUSE_cc1101.setRxBW(812.50);       // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
  ELECHOUSE_cc1101.setDRate(17.26);       // Set the Data Rate in kBaud. Value from 0.02 to 1621.83. Default is 99.97 kBaud!
  ELECHOUSE_cc1101.setPA(0);             // Set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max!
  ELECHOUSE_cc1101.setSyncMode(0);        // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
  //ELECHOUSE_cc1101.setSyncWord(170, 170); // AA AA Set sync word. Must be the same for the transmitter and receiver. (Syncword high, Syncword low)
  ELECHOUSE_cc1101.setAdrChk(0);          // Controls address check configuration of received packages. 0 = No address check. 1 = Address check, no broadcast. 2 = Address check and 0 (0x00) broadcast. 3 = Address check and 0 (0x00) and 255 (0xFF) broadcast.
  ELECHOUSE_cc1101.setAddr(0);            // Address used for packet filtration. Optional broadcast addresses are 0 (0x00) and 255 (0xFF).
  ELECHOUSE_cc1101.setWhiteData(0);       // Turn data whitening on / off. 0 = Whitening off. 1 = Whitening on.
  ELECHOUSE_cc1101.setPktFormat(0);       // Format of RX and TX data. 0 = Normal mode, use FIFOs for RX and TX. 1 = Synchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins. 2 = Random TX mode; sends random data using PN9 generator. Used for test. Works as normal mode, setting 0 (00), in RX. 3 = Asynchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins.
  ELECHOUSE_cc1101.setLengthConfig(0);    // 0 = Fixed packet length mode. 1 = Variable packet length mode. 2 = Infinite packet length mode. 3 = Reserved
  ELECHOUSE_cc1101.setPacketLength(0);   // Indicates the packet length when fixed packet length mode is enabled. If variable packet length mode is used, this value indicates the maximum packet length allowed.
  ELECHOUSE_cc1101.setCrc(0);             // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.
  ELECHOUSE_cc1101.setCRC_AF(0);          // Enable automatic flush of RX FIFO when CRC is not OK. This requires that only one packet is in the RXIFIFO and that packet length is limited to the RX FIFO size.
  ELECHOUSE_cc1101.setDcFilterOff(0);     // Disable digital DC blocking filter before demodulator. Only for data rates ≤ 250 kBaud The recommended IF frequency changes when the DC blocking is disabled. 1 = Disable (current optimized). 0 = Enable (better sensitivity).
  ELECHOUSE_cc1101.setManchester(0);      // Enables Manchester encoding/decoding. 0 = Disable. 1 = Enable.
  ELECHOUSE_cc1101.setFEC(0);             // Enable Forward Error Correction (FEC) with interleaving for packet payload (Only supported for fixed packet length mode. 0 = Disable. 1 = Enable.
  ELECHOUSE_cc1101.setPQT(0);             // Preamble quality estimator threshold. The preamble quality estimator increases an internal counter by one each time a bit is received that is different from the previous bit, and decreases the counter by 8 each time a bit is received that is the same as the last bit. A threshold of 4∙PQT for this counter is used to gate sync word detection. When PQT=0 a sync word is always accepted.
  ELECHOUSE_cc1101.setAppendStatus(0);    // When enabled, two status bytes will be appended to the payload of the packet. The status bytes contain RSSI and LQI values, as well as CRC OK.

  Serial.println("Rx Mode");
  ELECHOUSE_cc1101.SetRx();

  RFControl::startReceiving(RECEIVER_PIN);
}

void loop() {
  if(RFControl::hasData()) {
    unsigned int *timings;
    unsigned int timings_size;
    unsigned int pulse_length_divider = RFControl::getPulseLengthDivider();
    RFControl::getRaw(&timings, &timings_size);
    for(int i=0; i < timings_size; i++) {
      unsigned long timing = timings[i] * pulse_length_divider;
      Serial.print(timing);
      Serial.write('\t');
      if((i+1)%16 == 0) {
        Serial.write('\n');
      }
    }
    Serial.write('\n');
    Serial.write('\n');
    RFControl::continueReceiving();
  }
} */
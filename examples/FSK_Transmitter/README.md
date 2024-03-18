Sample program to create a FSK signal to test decoder logic and settings.  Based on WH51 Soil Moisture Sensor Signal

Sends a FSK packet at the rate of about 1 every second or so

This is what it looks like in the receiver

```
N: Received message : {"model":"Fineoffset-WH51","id":"006b58","battery_ok":0.77778,"battery_mV":1400,"moisture":36,"boost":3,"ad_raw":210,"mic":"CRC","protocol":"Fine Offset Electronics/ECOWITT WH51, SwitchDoc Labs SM23 Soil Moisture Sensor","rssi":-74,"duration":11000}
```

Original WH51 signal via rtl_433

time : 2021-04-06 13:21:09
model : Fineoffset-WH51 ID : 00ca1d
Battery level: 1.111 Battery : 1700 mV Moisture : 0 % Transmission boost: 0 AD raw : 20 Integrity : CRC
Analyzing pulses...
Total count: 48, width: 10.26 ms ( 2564 S)
Pulse width distribution:
[ 0] count: 1, width: 0 us [0;0] ( 0 S)
[ 1] count: 38, width: 56 us [56;68] ( 14 S)
[ 2] count: 3, width: 116 us [116;120] ( 29 S)
[ 3] count: 3, width: 172 us [172;176] ( 43 S)
[ 4] count: 1, width: 404 us [404;404] ( 101 S)
[ 5] count: 1, width: 288 us [288;288] ( 72 S)
[ 6] count: 1, width: 1392 us [1392;1392] ( 348 S)
Gap width distribution:
[ 0] count: 33, width: 56 us [48;60] ( 14 S)
[ 1] count: 6, width: 172 us [172;176] ( 43 S)
[ 2] count: 2, width: 464 us [464;468] ( 116 S)
[ 3] count: 3, width: 112 us [112;116] ( 28 S)
[ 4] count: 2, width: 256 us [232;280] ( 64 S)
[ 5] count: 1, width: 348 us [348;348] ( 87 S)
Pulse period distribution:
[ 0] count: 1, width: 48 us [48;48] ( 12 S)
[ 1] count: 27, width: 116 us [116;120] ( 29 S)
[ 2] count: 9, width: 232 us [232;232] ( 58 S)
[ 3] count: 3, width: 172 us [172;176] ( 43 S)
[ 4] count: 2, width: 576 us [520;636] ( 144 S)
[ 5] count: 3, width: 308 us [292;340] ( 77 S)
[ 6] count: 1, width: 872 us [872;872] ( 218 S)
[ 7] count: 1, width: 1448 us [1448;1448] ( 362 S)
Pulse timing distribution:
[ 0] count: 1, width: 0 us [0;0] ( 0 S)
[ 1] count: 71, width: 56 us [48;68] ( 14 S)
[ 2] count: 6, width: 116 us [112;120] ( 29 S)
[ 3] count: 9, width: 172 us [172;176] ( 43 S)
[ 4] count: 3, width: 444 us [404;468] ( 111 S)
[ 5] count: 3, width: 264 us [232;288] ( 66 S)
[ 6] count: 1, width: 1392 us [1392;1392] ( 348 S)
[ 7] count: 1, width: 348 us [348;348] ( 87 S)
[ 8] count: 1, width: 0 us [0;0] ( 0 S)
Level estimates [high, low]: 15873, 7
RSSI: -0.1 dB SNR: 33.6 dB Noise: -33.7 dB
Frequency offsets [F1, F2]: 7357, -7395 (+28.1 kHz, -28.2 kHz)
Guessing modulation: Pulse Code Modulation (Not Return to Zero)
Attempting demodulation... short_width: 56, long_width: 56, reset_limit: 57344, sync_width: 0
Use a flex decoder with -X 'n=name,m=FSK_PCM,s=56,l=56,r=57344'
pulse_demod_pcm(): Analyzer Device
bitbuffer:: Number of rows: 1
[00] {177} 55 55 55 55 55 51 6e a2 88 06 50 e8 8b f8 07 c0 a7 ff ff fb 9a 20 80

Noting that the bit buffer seems to be out of sync by 1 bit. 55 <<1 = AA (Sync),
55 55 55 55 5 = AA AA AA AA A - sync
5 51 6e a = AA 2D D4 - pre
etc

Given that this actually has a well defined packet
Data format:
00 01 02 03 04 05 06 07 08 09 10 11 12 13
aa aa aa 2d d4 51 00 6b 58 6e 7f 24 f8 d2 ff ff ff 3c 28 8
FF II II II TB YY MM ZA AA XX XX XX CC SS
Sync: aa aa aa ...
Preamble: 2d d4 - actually preamble[] = {0xAA, 0x2D, 0xD4};
FF: Family code 0x51 (ECOWITT/FineOffset WH51)
IIIIII: ID (3 bytes)
T: Transmission period boost: highest 3 bits set to 111 on moisture change and decremented each transmission;
if T = 0 period is 70 sec, if T > 0 period is 10 sec
B: Battery voltage: lowest 5 bits are battery voltage * 10 (e.g. 0x0c = 12 = 1.2V). Transmitter works down to 0.7V (0x07)
YY: ? Fixed: 0x7f
MM: Moisture percentage 0%-100% (0x00-0x64) MM = (AD - 70) / (450 - 70)
Z: ? Fixed: leftmost 7 bit 1111 100
AAA: 9 bit AD value MSB byte[07] & 0x01, LSB byte[08]
XXXXXX: ? Fixed: 0xff 0xff 0xff
CC: CRC of the preceding 12 bytes (Polynomial 0x31, Initial value 0x00, Input not reflected, Result not reflected)
SS: Sum of the preceding 13 bytes % 256

It should be simple enough to set up the parameters in the normal packet TX code. I will do it tonight - I can verify against RTL_433 using the SDR, the Misol receiver and ESP.
Hopefully, the transmitter (WH51) will be stable enough clock wise to use Packet Rx mode on the CC101. I have not tried to date but wanted to start with the RTL_433 approach.

## ELECHOUSE_CC1101_SRC_DRV

```
    ELECHOUSE_cc1101.setGDO(gdo0, 0);      // set lib internal gdo pins (gdo0,gdo2). Gdo2 not use for this example.
    ELECHOUSE_cc1101.setCCMode(1);         // set config for internal transmission mode.
    ELECHOUSE_cc1101.setModulation(0);     // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setMHZ(433.92);       // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    ELECHOUSE_cc1101.setDeviation(30);     // Set the Frequency deviation in kHz. Value from 1.58 to 380.85. Default is 47.60 kHz.
    ELECHOUSE_cc1101.setChannel(0);        // Set the Channelnumber from 0 to 255. Default is cahnnel 0.
    ELECHOUSE_cc1101.setChsp(199.95);      // The channel spacing is multiplied by the channel number CHAN and added to the base frequency in kHz. Value from 25.39 to 405.45. Default is 199.95 kHz.
    ELECHOUSE_cc1101.setRxBW(81.50);       // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
    ELECHOUSE_cc1101.setDRate(17.241);     // Set the Data Rate in kBaud. Value from 0.02 to 1621.83. Default is 99.97 kBaud!
    ELECHOUSE_cc1101.setPA(10);            // Set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max!
    ELECHOUSE_cc1101.setSyncMode(2);       // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
    ELECHOUSE_cc1101.setSyncWord(45, 212); // Set sync word. Must be the same for the transmitter and receiver. (Syncword high, Syncword low)
    ELECHOUSE_cc1101.setAdrChk(0);         // Controls address check configuration of received packages. 0 = No address check. 1 = Address check, no broadcast. 2 = Address check and 0 (0x00) broadcast. 3 = Address check and 0 (0x00) and 255 (0xFF) broadcast.
    ELECHOUSE_cc1101.setAddr(0);           // Address used for packet filtration. Optional broadcast addresses are 0 (0x00) and 255 (0xFF).
    ELECHOUSE_cc1101.setWhiteData(0);      // Turn data whitening on / off. 0 = Whitening off. 1 = Whitening on.
    ELECHOUSE_cc1101.setPktFormat(0);      // Format of RX and TX data. 0 = Normal mode, use FIFOs for RX and TX. 1 = Synchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins. 2 = Random TX mode; sends random data using PN9 generator. Used for test. Works as normal mode, setting 0 (00), in RX. 3 = Asynchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins.
    ELECHOUSE_cc1101.setLengthConfig(1);   // 0 = Fixed packet length mode. 1 = Variable packet length mode. 2 = Infinite packet length mode. 3 = Reserved
    ELECHOUSE_cc1101.setPacketLength(0);   // Indicates the packet length when fixed packet length mode is enabled. If variable packet length mode is used, this value indicates the maximum packet length allowed.
    ELECHOUSE_cc1101.setCrc(0);            // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.
    ELECHOUSE_cc1101.setCRC_AF(0);         // Enable automatic flush of RX FIFO when CRC is not OK. This requires that only one packet is in the RXIFIFO and that packet length is limited to the RX FIFO size.
    ELECHOUSE_cc1101.setDcFilterOff(0);    // Disable digital DC blocking filter before demodulator. Only for data rates ≤ 250 kBaud The recommended IF frequency changes when the DC blocking is disabled. 1 = Disable (current optimized). 0 = Enable (better sensitivity).
    ELECHOUSE_cc1101.setManchester(0);     // Enables Manchester encoding/decoding. 0 = Disable. 1 = Enable.
    ELECHOUSE_cc1101.setFEC(0);            // Enable Forward Error Correction (FEC) with interleaving for packet payload (Only supported for fixed packet length mode. 0 = Disable. 1 = Enable.
    ELECHOUSE_cc1101.setPQT(0);            // Preamble quality estimator threshold. The preamble quality estimator increases an internal counter by one each time a bit is received that is different from the previous bit, and decreases the counter by 8 each time a bit is received that is the same as the last bit. A threshold of 4∙PQT for this counter is used to gate sync word detection. When PQT=0 a sync word is always accepted.
    ELECHOUSE_cc1101.setAppendStatus(0);   // When enabled, two status bytes will be appended to the payload of the packet. The status bytes contain RSSI and LQI values, as well as CRC OK.
```

## Tks to @AusGunno

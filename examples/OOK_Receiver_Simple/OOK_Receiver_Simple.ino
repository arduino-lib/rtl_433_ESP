/*
	Simplest rtl_433_ESP example for OOK/ASK Devices
*/

#define RF_MODULE_FREQUENCY 433.92
#define buf_size 512

#include <rtl_433_ESP.h>
rtl_433_ESP rf;

char buffer[buf_size];

void rtl_433_callback(char* message) {
	Serial.printf("Received message: %s\n", message);
}

void setup() {
	Serial.begin(115200);
	rf.initReceiver(RF_MODULE_RECEIVER_GPIO, RF_MODULE_FREQUENCY);
	rf.setCallback(rtl_433_callback, buffer, buf_size);
	rf.enableReceiver();
	rf.getModuleStatus();
}

void loop() {
	rf.loop();
}

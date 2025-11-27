#ifndef LIBRARIES_FURBY_FURBYWIFI_H_
#define LIBRARIES_FURBY_FURBYWIFI_H_
#include "../FurbyConstants.h"

#include <WiFi.h>
const char* ssid = "MYA-L113";//"CG WiFi Bandablu";//"ZyXEL";//
const char* password = "mammaangela59";//"galassi2020";//"wferinformatica";//

class FurbyWifi {
public:

	FurbyWifi(){};
	static void init(){

		WiFi.begin(ssid, password);
		// Wait for connection
		while (WiFi.status() != WL_CONNECTED) {
			delay(500);
			Serial.print(".");
		}
		Serial.println("");
		Serial.print("Connected to ");
		Serial.println(ssid);
		Serial.print("IP address: ");
		Serial.println(WiFi.localIP());

	};
};
#endif

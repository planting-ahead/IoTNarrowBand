#include <Seeed_ws2812.h>

#include <BreakoutSDK.h>
#include <board.h>
#include<stdio.h>
#include "DHT.h"

static const char *device_purpose = "monitor plants temp and c";

static const char *psk_key ="key goes here";

Breakout *breakout = &Breakout::getInstance();

#define SENSOR_PIN (D38)
#define LOOP_INTERVAL (1 * 1000)
#define SEND_INTERVAL (1 * 60 * 1000)
#define DHTTYPE DHT11   // DHT 11 

DHT dht(SENSOR_PIN, DHTTYPE);

/**
 * light turning yellow to green
 */
WS2812 strip = WS2812(1, RGB_LED_PIN);

void enableLed() {
  pinMode(RGB_LED_PWR_PIN, OUTPUT);
  digitalWrite(RGB_LED_PWR_PIN, HIGH);
  strip.begin();
  strip.brightness = 5;
}

void setup() {
  // put your setup code here, to run once:
  owl_log_set_level(L_INFO);
  LOG(L_WARN, "Arduino setup() starting up\r\n");
  
  enableLed();
  //changes colors
  strip.WS2812SetRGB(0, 0x20, 0x20, 0x00);
  strip.WS2812Send();
  
  breakout->setPurpose(device_purpose);
  breakout->setPSKKey(psk_key);
  breakout->setPollingInterval(1 * 60);  // Optional, by default set to 10 minutes

  // Powering the modem and starting up the SDK
  LOG(L_WARN, "Powering on module and registering...");
  breakout->powerModuleOn();
  
  LOG(L_WARN, "... done powering on and registering.\r\n");
  LOG(L_WARN, "Arduino loop() starting up\r\n");
  
}

void sendCommand(const char * command) {
  if (breakout->sendTextCommand(command) == COMMAND_STATUS_OK) {
    LOG(L_INFO, "Tx-Command [%s]\r\n", command);
    } else {
      LOG(L_INFO, "Tx-Command ERROR\r\n");
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  static unsigned long last_send = 0;

  if ((last_send == 0) || (millis() - last_send >= SEND_INTERVAL)) {
    last_send = millis();

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    
  
    LOG(L_INFO, "Current temperature [%f] degrees celcius\r\n", temperature);
    LOG(L_INFO, "Current humidity [%f]\r\n", humidity);
    char commandText[512];
    snprintf(commandText, 512, "%4.2f, %4.2f", humidity, temperature);
    sendCommand(commandText);
  }

  breakout->spin();

  delay(LOOP_INTERVAL);
}

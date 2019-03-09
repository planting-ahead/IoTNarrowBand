#include <Seeed_ws2812.h>

#include <BreakoutSDK.h>
#include <board.h>
#include<stdio.h>
#include <DHT.h>

static const char *device_purpose = "monitor plants";

static const char *psk_key ="your key here";

Breakout *breakout = &Breakout::getInstance();

#define SENSOR_PIN (D38)
#define LOOP_INTERVAL (1 * 1000)
#define SEND_INTERVAL (10 * 60 * 1000)
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
  
}

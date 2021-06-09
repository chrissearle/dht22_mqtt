#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include "Board.h"
#include "Sensor.h"

#define SENDMQ 1

#define DHTTYPE DHT22

#define LOUNGE 0
#define BEDROOM 1
#define DINING 2
#define CELLAR 3

#define BOARD LOUNGE

Board *board = NULL;

WiFiClient wifiClient;

long lastMsg = 0;

void setup()
{
  Serial.begin(115200);

  WiFiManager wifiManager;

  wifiManager.autoConnect("AutoConnectAP");

  switch (BOARD)
  {
  case LOUNGE:
    board = new Board("lounge", wifiClient);
    board->addSensor(new Sensor("lounge", D1));
    break;

  case BEDROOM:
    board = new Board("bedroom", wifiClient);
    board->addSensor(new Sensor("bedroom", D1));
    break;

  case DINING:
    board = new Board("dining_room", wifiClient);
    board->addSensor(new Sensor("dining_room", D1));
    break;

  case CELLAR:
    board = new Board("cellar", wifiClient);
    board->addSensor(new Sensor("cellar", D1));
    break;
  }
}

void loop()
{
  board->keepAlive();

  long now = millis();

  if (now - lastMsg > (60 * 1000))
  {
    lastMsg = now;
    board->publish();
  }
}

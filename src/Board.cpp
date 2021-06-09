#include <PubSubClient.h>

#include "Configuration.h"
#include "Board.h"
#include "Sensor.h"

void Board::reconnect()
{
    while (!pubSubClient->connected())
    {
        Serial.print("Attempting MQTT connection...");

        String clientId = String("DHT-" + String(clientName));

        if (pubSubClient->connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(pubSubClient->state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void Board::keepAlive()
{
    if (!pubSubClient->connected())
    {
        reconnect();
    }

    pubSubClient->loop();
}

void Board::addSensor(Sensor *sensor)
{
    sensorList[sensorCount] = sensor;
    sensorCount = sensorCount + 1;
}

void Board::publish()
{
    for (int i = 0; i < sensorCount; i++)
    {
        Sensor *sensor = sensorList[i];

        const char *sensorName = sensor->getName();

        publishToTopic(sensorName, "temperature", sensor->getTemperature());
        publishToTopic(sensorName, "humidity", sensor->getHumidity());
    }
}

void Board::publishToTopic(const char *topicName, const char *topicType, float value)
{
  char *publishTopic = (char *)malloc(50);
  
  memset(publishTopic, 0, 50);

  sprintf(publishTopic, "%s/%s/%s", MQTT_TOPIC, topicName, topicType);

#if SENDMQ
    Serial.println("Posting");
    char charVal[10];
    dtostrf(value, 4, 4, charVal);
    pubSubClient->publish(publishTopic, charVal);
#endif

    Serial.print(publishTopic);
    Serial.print(" ");
    Serial.println(value);
}

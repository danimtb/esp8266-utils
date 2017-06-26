#ifndef MQTTDISCOVERYCOMPONENT_H
#define MQTTDISCOVERYCOMPONENT_H

#include <string>

#include "ArduinoJson.h"


class MqttDiscoveryComponent
{
protected:
    StaticJsonBuffer<500> m_jsonBuffer;
    JsonObject& m_jsonObject;

public:
    std::string discovery_prefix{"homeassistant"};
    std::string component;
    std::string entity_id;
    std::string discovery_suffix{"config"};

	std::string getConfigTopic();
    virtual std::string getConfigPayload() = 0;
    void addToPayload(std::string paramKey, std::string paramValue);
};

#endif

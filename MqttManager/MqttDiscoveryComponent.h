#ifndef MQTTDISCOVERYCOMPONENT_H
#define MQTTDISCOVERYCOMPONENT_H

#include <string>

#include "ArduinoJson.h"


class MqttDiscoveryComponent
{
protected:
    std::string m_nameKey{"name"};
    std::string m_payloadOffKey{"payload_off"};
    std::string m_payloadOnKey{"payload_on"};
    std::string m_qosKey{"qos"};
    std::string m_stateTopicKey{"state_topic"};

    StaticJsonBuffer<500> m_jsonBuffer;
    JsonObject& m_jsonObject;

    void addToPayload(std::string paramKey, std::string paramValue);

public:
    std::string discovery_prefix{"homeassistant"};
    std::string component;
    std::string entity_id;
    std::string discovery_suffix{"config"};
    std::string state_suffix{"state"};

    std::string name;
    std::string payload_off;
    std::string payload_on;
    std::string qos;
    std::string state_topic;

	std::string getConfigTopic();
    virtual std::string getConfigPayload() = 0;
    void setDefaultStateTopic();
};

#endif

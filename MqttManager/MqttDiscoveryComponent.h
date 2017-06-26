#ifndef MQTTDISCOVERYCOMPONENT_H
#define MQTTDISCOVERYCOMPONENT_H

#include <string>

#include "ArduinoJson.h"


class MqttDiscoveryComponent
{
private:
    std::string m_nameKey{"name"};
    std::string m_payloadOffKey{"payload_off"};
    std::string m_payloadOnKey{"payload_on"};
    std::string m_qosKey{"qos"};
    std::string m_stateTopicKey{"state_topic"};
    std::string m_stateValueTemplateKey{"state_value_template"};

protected:
    StaticJsonBuffer<500> m_jsonBuffer;
    JsonObject& m_jsonObject;

public:
    std::string discovery_prefix{"homeassistant"};
    std::string component;
    std::string entity_id;
    std::string discovery_suffix{"config"};

    std::string name;
    std::string payload_off;
    std::string payload_on;
    std::string qos;
    std::string state_topic;
    std::string state_value_template;

	std::string getConfigTopic();
    virtual std::string getConfigPayload() = 0;
    void addToPayload(std::string paramKey, std::string paramValue);
};

#endif

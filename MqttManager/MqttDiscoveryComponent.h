#ifndef MQTTDISCOVERYCOMPONENT_H
#define MQTTDISCOVERYCOMPONENT_H

#include <string>
#include <map>

#include "ArduinoJson.h"


class MqttDiscoveryComponent
{
protected:
    std::string m_commandTopicKey{"command_topic"};
    std::string m_nameKey{"name"};
    std::string m_payloadOffKey{"payload_off"};
    std::string m_payloadOnKey{"payload_on"};
    std::string m_qosKey{"qos"};
    std::string m_stateTopicKey{"state_topic"};

    std::map<std::string, std::string> m_fields;

    void addToPayload(std::string paramKey, std::string paramValue);

public:
    std::string discovery_prefix{"homeassistant"};
    std::string component;
    std::string entity_id;
    std::string discovery_suffix{"config"};
    std::string state_suffix{"state"};

    std::string command_topic;
    std::string name;
    std::string payload_off;
    std::string payload_on;
    std::string qos;
    std::string state_topic;

    std::string getConfigTopic();
    void setDefaultStateTopic();
    virtual std::string getConfigPayload();
};

#endif

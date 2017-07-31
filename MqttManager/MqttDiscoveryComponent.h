#ifndef MQTTDISCOVERYCOMPONENT_H
#define MQTTDISCOVERYCOMPONENT_H

#include <string>
#include <map>

#include "ArduinoJson.h"


class MqttDiscoveryComponent
{
private:
    std::map<std::string, std::string> m_fields;

public:
    MqttDiscoveryComponent(std::string component, std::string entity_id);
    void setConfigurtionVariables(std::string configKey, std::string configValue);

    std::string discovery_prefix{"homeassistant"};
    std::string component;
    std::string entity_id;
    std::string discovery_suffix{"config"};
    std::string state_suffix{"state"};
    std::string command_suffix{"command"};


    std::string getConfigTopic();
    std::string getStateTopic();
    std::string getCommandTopic();
    std::string getConfigPayload();
};

#endif

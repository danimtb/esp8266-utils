#ifndef MQTTDISCOVERYCOMPONENT_H
#define MQTTDISCOVERYCOMPONENT_H

#include <map>

#include "ArduinoJson.h"


class MqttDiscoveryComponent
{
private:
    std::map<String, String> m_fields;

public:
    MqttDiscoveryComponent();
    MqttDiscoveryComponent(String component, String name, String discovery_prefix="homeassistant");
    void setConfigurtionVariable(String configKey, String configValue);

    String discovery_prefix;
    String component;
    String entity_id;
    String discovery_suffix{"config"};
    String state_suffix{"state"};
    String command_suffix{"command"};
    String m_configTopic;


    String const getConfigTopic();
    String const getStateTopic();
    String const getCommandTopic();
    String const getConfigPayload();
};

#endif

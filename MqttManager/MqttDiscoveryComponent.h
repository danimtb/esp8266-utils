#ifndef MQTTDISCOVERYCOMPONENT_H
#define MQTTDISCOVERYCOMPONENT_H

#include <map>

#include "ArduinoJson.h"


class MqttDiscoveryComponent
{
private:
    std::map<String, String> m_fields;

public:
    MqttDiscoveryComponent(String component, String name, String discovery_prefix="homeassistant");
    void setConfigurtionVariable(String configKey, String configValue);

    String discovery_prefix;
    String component;
    String entity_id;
    String discovery_suffix{"config"};
    String state_suffix{"state"};
    String command_suffix{"command"};
    String m_configTopic;


    String& getConfigTopic();
    String& getStateTopic();
    String& getCommandTopic();
    String getConfigPayload();
};

#endif

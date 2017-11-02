#include "MqttDiscoveryComponent.h"

MqttDiscoveryComponent::MqttDiscoveryComponent(String component, String name)
{
    this->component = component;
    this->entity_id = name;

    this->entity_id.replace(" ", "_");

    this->setConfigurtionVariable("name", name);
    this->setConfigurtionVariable("state_topic", discovery_prefix + "/" + component + "/" + entity_id + "/" + state_suffix);

    if (component == "light" || component == "switch")
    {
        this->setConfigurtionVariable("command_topic", discovery_prefix + "/" + component + "/" + entity_id + "/" + command_suffix);
    }
    else
    {
        this->setConfigurtionVariable("command_topic", "");
    }
}

void MqttDiscoveryComponent::setConfigurtionVariable(String configKey, String configValue)
{
    if (configValue.length() != 0)
    {
        m_fields[configKey] = configValue;
    }
}

String& MqttDiscoveryComponent::getConfigTopic()
{
    m_configTopic = discovery_prefix + "/" + component + "/" + entity_id + "/" + discovery_suffix;
    return m_configTopic;
}

String& MqttDiscoveryComponent::getStateTopic()
{
    if (m_fields.find("state_topic") != m_fields.end())
    {
        return m_fields["state_topic"];
    }
}

String& MqttDiscoveryComponent::getCommandTopic()
{
    return m_fields["command_topic"];
}

String MqttDiscoveryComponent::getConfigPayload()
{
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& jsonObject = jsonBuffer.createObject();
    String jsonString;

    for(auto i : m_fields)
    {
        jsonObject[i.first] = i.second;
    }

    jsonObject.printTo(jsonString);
    return jsonString;
}

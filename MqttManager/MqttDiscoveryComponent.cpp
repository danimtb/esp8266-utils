#include "MqttDiscoveryComponent.h"


MqttDiscoveryComponent::MqttDiscoveryComponent()
{
}

MqttDiscoveryComponent::MqttDiscoveryComponent(String component, String name, String discovery_prefix)
{
    this->component = component;
    this->entity_id = name;
    this->discovery_prefix = discovery_prefix;

    this->entity_id.replace(" ", "_");

    this->setConfigurtionVariable("name", name);
    this->setConfigurtionVariable("state_topic", this->discovery_prefix + "/" + this->component + "/" + this->entity_id + "/" + this->state_suffix);

    if (component == "light" || component == "switch")
    {
        this->setConfigurtionVariable("command_topic", this->discovery_prefix + "/" + this->component + "/" + this->entity_id + "/" + this->command_suffix);
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

String const MqttDiscoveryComponent::getConfigTopic()
{
    m_configTopic = discovery_prefix + "/" + component + "/" + entity_id + "/" + discovery_suffix;
    return m_configTopic;
}

String const MqttDiscoveryComponent::getStateTopic()
{
    if (m_fields.find("state_topic") != m_fields.end())
    {
        return m_fields["state_topic"];
    }
}

String const MqttDiscoveryComponent::getCommandTopic()
{
    return m_fields["command_topic"];
}

String const MqttDiscoveryComponent::getConfigPayload()
{
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& jsonObject = jsonBuffer.createObject();
    String jsonString;

    for(auto const &i : m_fields)
    {
        jsonObject[i.first] = i.second;
    }

    jsonObject.printTo(jsonString);
    return jsonString;
}

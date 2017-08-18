#include "MqttDiscoveryComponent.h"

MqttDiscoveryComponent::MqttDiscoveryComponent(String component, String entity_id)
{
    this->component = component;
    this->entity_id = entity_id;

    String name = entity_id;
    name.replace("_", " ");

    this->setConfigurtionVariables("name", name);

    if (component == "light" || component == "switch")
    {
        this->setConfigurtionVariables("command_topic", discovery_prefix + "/" + component + "/" + entity_id + "/" + command_suffix);
    }
    else
    {
        this->setConfigurtionVariables("command_topic", "");
    }
}

void MqttDiscoveryComponent::setConfigurtionVariables(String configKey, String configValue)
{
    if (configValue.length() != 0)
    {
        m_fields[configKey] = configValue;
    }
}

String MqttDiscoveryComponent::getConfigTopic()
{
    return discovery_prefix + "/" + component + "/" + entity_id + "/" + discovery_suffix;
}

String MqttDiscoveryComponent::getStateTopic()
{
    if (m_fields.find("state_topic") != m_fields.end())
    {
        return m_fields["state_topic"];
    }
    else
    {
        return (discovery_prefix + "/" + component + "/" + entity_id + "/" + state_suffix);
    }
}

String MqttDiscoveryComponent::getCommandTopic()
{
    return m_fields["command_topic"];
}

String MqttDiscoveryComponent::getConfigPayload()
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& jsonObject = jsonBuffer.createObject();
    String jsonString;

    for(auto i : m_fields)
    {
        jsonObject[i.first.c_str()] = i.second.c_str();
    }

    jsonObject.printTo(jsonString);
    return jsonString.c_str();
}

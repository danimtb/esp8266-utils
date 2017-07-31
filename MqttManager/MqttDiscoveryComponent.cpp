#include "MqttDiscoveryComponent.h"

MqttDiscoveryComponent::MqttDiscoveryComponent(std::string component, std::string entity_id)
{
    this->component = component;
    this->entity_id = entity_id;

    this->setConfigurtionVariables("name", entity_id.replace(entity_id.begin(), entity_id.end(), "_", " "));
}

void MqttDiscoveryComponent::setConfigurtionVariables(std::string configKey, std::string configValue);
{
    if (!configValue.empty())
    {
        m_fields[configKey] = configValue;
    }
}

std::string MqttDiscoveryComponent::getConfigTopic()
{
    return discovery_prefix + "/" + component + "/" + entity_id + "/" + discovery_suffix;
}

std::string MqttDiscoveryComponent::getStateTopic()
{
    state_topic = discovery_prefix + "/" + component + "/" + entity_id + "/" + state_suffix;
}

std::string MqttDiscoveryComponent::getCommandTopic()
{
    state_topic = discovery_prefix + "/" + component + "/" + entity_id + "/" + command_suffix;
}

std::string MqttDiscoveryComponent::getConfigPayload()
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

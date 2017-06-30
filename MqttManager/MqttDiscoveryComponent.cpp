#include "MqttDiscoveryComponent.h"

std::string MqttDiscoveryComponent::getConfigTopic()
{
    return discovery_prefix + "/" + component + "/" + entity_id + "/" + discovery_suffix;
}

void MqttDiscoveryComponent::setDefaultStateTopic()
{
    state_topic = discovery_prefix + "/" + component + "/" + entity_id + "/" + state_suffix;
}

void MqttDiscoveryComponent::addToPayload(std::string paramKey, std::string paramValue)
{
    if (!paramValue.empty())
    {
        m_fields[paramKey] = paramValue;
    }
}

std::string MqttDiscoveryComponent::getConfigPayload()
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& jsonObject = jsonBuffer.createObject();
    String jsonString;

    this->addToPayload(m_nameKey, name);

    for(auto i : m_fields)
    {
        jsonObject[i.first.c_str()] = i.second.c_str();
    }

    jsonObject.printTo(jsonString);
    return jsonString.c_str();
}

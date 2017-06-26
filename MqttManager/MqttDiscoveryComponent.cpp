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
        m_jsonObject[paramKey] = paramValue;
    }
}

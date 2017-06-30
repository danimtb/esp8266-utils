#include "MqttDiscoveryLight.h"

MqttDiscoveryLight::MqttDiscoveryLight(std::string entity_id, std::string command_topic)
{    
    this->entity_id = entity_id;
    this->name = entity_id.replace(entity_id.begin(), entity_id.end(), "_", " ");

    if (command_topic.empty())
    {
        this->setDefaultCommandTopic();
    }
    else
    {
        this->command_topic = command_topic;
    }
}

void MqttDiscoveryLight::setDefaultCommandTopic()
{
    command_topic = discovery_prefix + "/" + component + "/" + entity_id + "/" + command_suffix;
}

std::string MqttDiscoveryLight::getConfigPayload()
{
    DynamicJsonBuffer jsonBuffer;
    JsonObject& jsonObject = jsonBuffer.createObject();
    String jsonString;

    MqttDiscoveryComponent::getConfigPayload();

    this->addToPayload(m_commandTopicKey, command_topic);
    this->addToPayload(m_brightnessCommandTopicKey, brightness_command_topic);
    this->addToPayload(m_brightnessScaleKey, brightness_scale);
    this->addToPayload(m_brightnessStateTopicKey, brightness_state_topic);
    this->addToPayload(m_brightnessValueTemplateKey, brightness_value_template);
    this->addToPayload(m_colorTempCommandTopicKey, color_temp_command_topic);
    this->addToPayload(m_colorTempStateTopicKey, color_temp_state_topic);
    this->addToPayload(m_colorTempValueTemplateKey, color_temp_value_template);
    this->addToPayload(m_effectCommandTopicKey, effect_command_topic);
    this->addToPayload(m_effectStateTopicKey, effect_state_topic);
    this->addToPayload(m_effectValueTemplateKey, effect_value_template);
    this->addToPayload(m_effectListKey, effect_list);
    this->addToPayload(m_optimisticKey, optimistic);
    this->addToPayload(m_payloadOffKey, payload_off);
    this->addToPayload(m_payloadOnKey, payload_on);
    this->addToPayload(m_qosKey, qos);
    this->addToPayload(m_retainKey, retain);
    this->addToPayload(m_rgbCommandTopicKey, rgb_command_topic);
    this->addToPayload(m_rgbStateTopicKey, rgb_state_topic);
    this->addToPayload(m_rgbValueTemplateKey, rgb_value_template);
    this->addToPayload(m_stateTopicKey, state_topic);
    this->addToPayload(m_stateValueTemplateKey, state_value_template);
    this->addToPayload(m_whiteValueCommandTopicKey, white_value_command_topic);
    this->addToPayload(m_whiteValueStateTopicKey, white_value_state_topic);
    this->addToPayload(m_whiteValueValueTemplateKey, white_value_value_template);
    this->addToPayload(m_xyCommandTopicKey, xy_command_topic);
    this->addToPayload(m_xyStateTopicKey, xy_state_topic);
    this->addToPayload(m_xyValueTemplateKey, xy_value_template);

    for(auto i : m_fields)
    {
        jsonObject[i.first.c_str()] = i.second.c_str();
    }

    jsonObject.printTo(jsonString);
    return jsonString.c_str();
}

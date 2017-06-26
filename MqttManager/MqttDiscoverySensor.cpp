#include "MqttDiscoverySensor.h"

MqttDiscoverySensor::MqttDiscoverySensor(std::string entity_id, std::string state_topic)
{    
    this->entity_id = entity_id;
    this->name = entity_id.replace(entity_id.begin(), entity_id.end(), "_", " ");

    if (state_topic.empty())
    {
        this->setDefaultStateTopic();
    }
    else
    {
        this->state_topic = state_topic;
    }
}

std::string MqttDiscoverySensor::getConfigPayload()
{
    m_jsonObject = m_jsonBuffer.createObject();
    String jsonString;

    this->addToPayload(m_nameKey, name);
    this->addToPayload(m_payloadOffKey, payload_off);
    this->addToPayload(m_payloadOnKey, payload_on);
    this->addToPayload(m_qosKey, qos);
    this->addToPayload(m_stateTopicKey, state_topic);
    this->addToPayload(m_valueTemplateKey, value_template);

    m_jsonObject.printTo(jsonString);
    return jsonString.c_str();
}

#ifndef MQTTDISCOVERYSENSOR_H
#define MQTTDISCOVERYLIGHT_H

#include "MqttDiscoveryComponent.h"

#include "ArduinoJson.h"

#include <string>

class MqttDiscoverySensor : public MqttDiscoveryComponent
{
private:
    std::string m_unitOfMeasurementKey{"unit_of_measurement"};
    std::string m_expireAfterKey{"expire_after"};
    std::string m_valueTemplateKey{"value_template"};

public:
    MqttDiscoverySensor(std::string entity_id, std::string state_topic = "");

    std::string unit_of_measurement;
    std::string expire_after;
    std::string value_template;

    virtual std::string getConfigPayload();
};

#endif // MQTTDISCOVERYSENSOR_H

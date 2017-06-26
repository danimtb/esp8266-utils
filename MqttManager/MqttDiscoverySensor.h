#ifndef MQTTDISCOVERYSENSOR_H
#define MQTTDISCOVERYLIGHT_H

#include "MqttDiscoveryComponent.h"

#include <string>

class MqttDiscoverySensor : public MqttDiscoveryComponent
{
private:
    std::string m_unitOfMeasurementKey{"unit_of_measurement"};
    std::string m_expireAfterKey{"expire_after"};
    std::string m_valueTemplate{"value_template"};

public:
    MqttDiscoverySensor(std::string entity_id, std::string state_topic = "");

    std::string unit_of_measurement;
    std::string expire_after;
    std::string value_template;
};

#endif // MQTTDISCOVERYSENSOR_H

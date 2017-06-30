#ifndef MQTTDISCOVERYLIGHT_H
#define MQTTDISCOVERYLIGHT_H

#include "MqttDiscoveryComponent.h"

#include "ArduinoJson.h"

#include <string>

class MqttDiscoveryLight : public MqttDiscoveryComponent
{
private:
    std::string m_brightnessCommandTopicKey{"brightness_command_topic"};
    std::string m_brightnessScaleKey{"brightness_scale"};
    std::string m_brightnessStateTopicKey{"brightness_state_topic"};
    std::string m_brightnessValueTemplateKey{"brightness_value_template"};
    std::string m_colorTempCommandTopicKey{"color_temp_command_topic"};
    std::string m_colorTempStateTopicKey{"color_temp_state_topic"};
    std::string m_colorTempValueTemplateKey{"color_temp_value_template"};
    std::string m_effectCommandTopicKey{"effect_command_topic"};
    std::string m_effectStateTopicKey{"effect_state_topic"};
    std::string m_effectValueTemplateKey{"effect_value_template"};
    std::string m_effectListKey{"effect_list"};
    std::string m_optimisticKey{"optimistic"};
    std::string m_retainKey{"retain"};
    std::string m_rgbCommandTopicKey{"rgb_command_topic"};
    std::string m_rgbStateTopicKey{"rgb_state_topic"};
    std::string m_rgbValueTemplateKey{"rgb_value_template"};
    std::string m_whiteValueCommandTopicKey{"white_value_command_topic"};
    std::string m_stateValueTemplateKey{"state_value_template"};
    std::string m_whiteValueStateTopicKey{"white_value_state_topic"};
    std::string m_whiteValueValueTemplateKey{"white_value_value_template"};
    std::string m_xyCommandTopicKey{"xy_command_topic"};
    std::string m_xyStateTopicKey{"xy_state_topic"};
    std::string m_xyValueTemplateKey{"xy_value_template"};

public:
    MqttDiscoveryLight(std::string entity_id, std::string command_topic = "");
    void setDefaultCommandTopic();
    virtual std::string getConfigPayload();

    std::string command_suffix{"set"};
    std::string brightness_command_topic;
    std::string brightness_scale;
    std::string brightness_state_topic;
    std::string brightness_value_template;
    std::string color_temp_command_topic;
    std::string color_temp_state_topic;
    std::string color_temp_value_template;
    std::string effect_command_topic;
    std::string effect_state_topic;
    std::string effect_value_template;
    std::string effect_list;
    std::string optimistic;
    std::string retain;
    std::string rgb_command_topic;
    std::string rgb_state_topic;
    std::string rgb_value_template;
    std::string state_value_template;
    std::string white_value_command_topic;
    std::string white_value_state_topic;
    std::string white_value_value_template;
    std::string xy_command_topic;
    std::string xy_state_topic;
    std::string xy_value_template;
};

#endif // MQTTDISCOVERYLIGHT_H

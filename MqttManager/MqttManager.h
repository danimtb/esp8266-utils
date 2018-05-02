#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

#undef min
#undef max

#include <AsyncMqttClient.h>

#include "SimpleTimer.h"
#include "MqttDiscoveryComponent.h"

#include <map>
#include <string>
#include <vector>


class MqttManager
{
private:
    String m_mqttServer;
    uint16_t m_mqttPort;
    String m_mqttUsername;
    String m_mqttPassword;

    String m_deviceName;
    String m_deviceIP;
    String m_deviceMac;
    String m_hardware;
    String m_firmware;
    String m_firmwareVersion;

    String m_deviceDataTopic;

    String m_lastWillPayload;
    String m_lastWillTopic;

    MqttDiscoveryComponent* m_deviceStatusSensor;
    MqttDiscoveryComponent* m_deviceIpSensor;
    MqttDiscoveryComponent* m_deviceMacSensor;
    MqttDiscoveryComponent* m_deviceHardwareSensor;
    MqttDiscoveryComponent* m_deviceFirmwareSensor;
    MqttDiscoveryComponent* m_deviceFirmwareVersionSensor;

    std::map<String, String> m_statusTopics;
    std::vector<String> m_subscribeTopics;
    std::vector<MqttDiscoveryComponent*> m_discoveryComponents;

    AsyncMqttClient m_mqttClient;

    bool m_connected;
    bool m_mqttDiscoveryEnabled{false};

    SimpleTimer m_deviceStatusInfoTimer;
    SimpleTimer m_checkConnectivityTimer;
    unsigned long m_checkConnectivityTimeOnline;
    unsigned long m_checkConnectivityTimeOffline;

    void setDeviceMac();
    void publishDeviceStatusInfo();
    void publishDiscoveryInfo();
    void refreshStatusTopics();
    void checkConnectivity();
    void setLastWillMQTT(const String& topic, const String& payload);

public:
    MqttManager();

    void setup(String mqttServer, String mqttPort, String mqttUsername, String mqttPassword, bool mqttDiscoveryEnabled = false);

    void setDeviceData(String deviceName, String hardware, String deviceIP, String firmware, String firmwareVersion, String discovery_prefix="homeassistant");
    void setCallback(void (*callback)(String , String));

    void setDeviceStatusInfoTime(unsigned long deviceStatusInfoTime);

    void enableDiscovery(bool enable);
    void addDiscoveryComponent(MqttDiscoveryComponent* component);

    void addSubscribeTopic(String subscribeTopic);
    void clearSubscribeTopics();

    void addStatusTopic(String statusTopic);
    void clearStatusTopics();

    void startConnection();
    void stopConnection();

    void publishMQTT(String topic, String payload);
    void publishMQTT(String topic, float payload);

    bool connected();

    void loop();
};

#endif

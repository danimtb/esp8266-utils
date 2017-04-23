#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

#undef min
#undef max

#include <AsyncMqttClient.h>

#include "SimpleTimer.h"

#include <map>
#include <string>
#include <vector>


class MqttManager
{
private:
    std::string m_mqttServer;
    uint16_t m_mqttPort;
    std::string m_mqttUsername;
    std::string m_mqttPassword;

    std::string m_deviceName;
    std::string m_deviceIP;
    std::string m_deviceMac;
    std::string m_hardware;
    std::string m_firmware;
    std::string m_firmwareVersion;

    std::string m_deviceDataTopic;

    std::map<std::string, std::string> m_statusTopics;
    std::vector<std::string> m_subscribeTopics;

    AsyncMqttClient m_mqttClient;

    bool m_connected;

    SimpleTimer m_deviceStatusInfoTimer;
    SimpleTimer m_checkConnectivityTimer;
    unsigned long m_checkConnectivityTimeOnline;
    unsigned long m_checkConnectivityTimeOffline;

    void setDeviceMac();
    void publishDeviceStatusInfo();
    void checkConnectivity();
    void refreshStatusTopics();

public:
    MqttManager();

    void setup(std::string mqttServer, std::string mqttPort, std::string mqttUsername, std::string mqttPassword);

    void setDeviceData(std::string deviceName, std::string hardware, std::string deviceIP, std::string firmware, std::string firmwareVersion);
    void setCallback(void (*callback)(std::string , std::string));
    void setLastWillMQTT(std::string topic, const char* payload);

    void setDeviceStatusInfoTime(unsigned long deviceStatusInfoTime);

    void addSubscribeTopic(std::string subscribeTopic);
    void clearSubscribeTopics();

    void addStatusTopic(std::string statusTopic);
    void clearStatusTopics();

    void startConnection();
    void stopConnection();

    void publishMQTT(std::string topic, std::string payload);

    bool connected();

    void loop();
};

#endif

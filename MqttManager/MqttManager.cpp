#include "MqttManager.h"

#include "ArduinoJson.h"

MqttManager::MqttManager()
{
    m_connected = false;
    m_publishMQTT = false;

    m_checkConnectivityTimeOnline = 20000;
    m_checkConnectivityTimeOffline = 3000;

    m_deviceStatusInfoTimer.setup(RT_ON, 300000);
    m_checkConnectivityTimer.setup(RT_ON, m_checkConnectivityTimeOffline);
}

void MqttManager::setup(std::string mqttServer, std::string mqttPort, std::string mqttUsername, std::string mqttPassword)
{
    m_mqttServer = mqttServer;
    m_mqttPort = atoi(mqttPort.c_str());
    m_mqttUsername = mqttUsername;
    m_mqttPassword = mqttPassword;

    IPAddress server;
    server.fromString(m_mqttServer.c_str());

    m_mqttClient.setServer(server, m_mqttPort);
    m_mqttClient.setCredentials(mqttUsername.c_str(), mqttPassword.c_str());
    m_mqttClient.setCleanSession(false);

    m_deviceStatusInfoTimer.start();
    m_checkConnectivityTimer.start();
}

void MqttManager::setDeviceData(std::string deviceName, std::string hardware, std::string deviceIP, std::string firmware, std::string firmwareVersion)
{
    m_deviceName = deviceName;
    m_deviceIP = deviceIP;
    m_hardware = hardware;
    m_firmware = firmware;
    m_firmwareVersion = firmwareVersion;

    m_mqttClient.setClientId(m_deviceName.c_str());

    m_deviceDataTopic = "/" + m_deviceName;
}

void MqttManager::publishDeviceStatusInfo()
{
    StaticJsonBuffer<200> deviceDataBuffer;
    JsonObject& deviceDataObject = deviceDataBuffer.createObject();
    String deviceDataString;

    deviceDataObject["name"] = m_deviceName.c_str();
    deviceDataObject["mac"] = m_deviceMac.c_str();
    deviceDataObject["ip"] = m_deviceIP.c_str();
    deviceDataObject["hardware"] = m_hardware.c_str();
    deviceDataObject["firmware"] = m_firmware.c_str();
    deviceDataObject["firmware_version"] = m_firmwareVersion.c_str();

    deviceDataObject.printTo(deviceDataString);

    m_mqttClient.publish(m_deviceDataTopic.c_str(), 1, true, deviceDataString.c_str());

    this->refreshStatusTopics();
}

void MqttManager::checkConnectivity()
{
    if (!m_mqttClient.connected())
    {
        m_connected = false;
        m_checkConnectivityTimer.load(m_checkConnectivityTimeOffline);
        m_mqttClient.connect();
    }
    else if (!m_connected)
    {
        for (int i = 0; i < m_subscribeTopics.size(); i++)
        {
            m_mqttClient.subscribe(m_subscribeTopics[i].c_str(), 1);
        }

        this->setDeviceMac();
        this->publishDeviceStatusInfo();
        m_connected = true;
        m_checkConnectivityTimer.load(m_checkConnectivityTimeOnline);
    }
    else
    {
        m_connected = true;
    }
}

void MqttManager::setDeviceMac()
{
    if (m_deviceMac.empty())
    {
        m_deviceMac = std::string(WiFi.macAddress().c_str());
    }
}

void MqttManager::addStatusTopic(std::string statusTopic)
{
    m_statusTopics[statusTopic] = "";
}

void MqttManager::clearStatusTopics()
{
    m_statusTopics.clear();
}

void MqttManager::addSubscribeTopic(std::string subscribeTopic)
{
    m_subscribeTopics.push_back(subscribeTopic);
}

void MqttManager::clearSubscribeTopics()
{
    for(int i=0; i < m_subscribeTopics.size(); i++)
    {
        m_mqttClient.unsubscribe(m_subscribeTopics[i].c_str());
    }
    m_subscribeTopics.clear();
}

void MqttManager::startConnection()
{
    this->checkConnectivity();
}

void MqttManager::stopConnection()
{
    m_mqttClient.disconnect();
    m_connected = false;
    m_checkConnectivityTimer.load(m_checkConnectivityTimeOffline);
}

void MqttManager::publishMQTT(std::string topic, std::string payload)
{
    if(m_statusTopics.find(topic) != m_statusTopics.end())
    {
        if(m_statusTopics[topic] != payload)
        {
            m_statusTopics[topic] = payload;
            m_publishMQTT = true;
        }
    }
    else if(m_connected)
    {
        std::pair<std::string, std::string> tempPair;
        tempPair.first = topic;
        tempPair.second = payload;
        m_tempPublishTopics.push_back(tempPair);
        m_publishMQTT = true;
    }
}

void MqttManager::setCallback(void (*callback)(char*, char*, AsyncMqttClientMessageProperties, size_t, size_t, size_t ))
{
    m_mqttClient.onMessage(callback);
}

void MqttManager::setLastWillMQTT(std::string topic, const char* payload)
{
    m_mqttClient.setWill(topic.c_str(), 1, true, payload);
}

void MqttManager::setDeviceStatusInfoTime(unsigned long deviceStatusInfoTime)
{
    m_deviceStatusInfoTimer.load(deviceStatusInfoTime);
    m_deviceStatusInfoTimer.start();
}

void MqttManager::loop()
{
    if (m_checkConnectivityTimer.check())
    {
        this->checkConnectivity();

        m_checkConnectivityTimer.start(); //restart timer
    }


    if (m_connected)
    {

        if (m_deviceStatusInfoTimer.check())
        {
            this->publishDeviceStatusInfo();

            m_deviceStatusInfoTimer.start(); //restart timer
        }


        if (m_publishMQTT)
        {
            this->refreshStatusTopics();

            if (!m_tempPublishTopics.empty())
            {
                for (int i = 0; i < m_tempPublishTopics.size(); i++)
                {
                    m_mqttClient.publish(m_tempPublishTopics[i].first.c_str(), 1, true, m_tempPublishTopics[i].second.c_str());
                }

                m_tempPublishTopics.clear();
            }

            m_publishMQTT = false;
        }
    }
    else
    {
        m_tempPublishTopics.clear(); //Delete temp publish topics buffer if there is not connection
    }
}

void MqttManager::refreshStatusTopics()
{
    for (std::map<std::string, std::string>::iterator it = m_statusTopics.begin(); it != m_statusTopics.end(); it++)
    {
        m_mqttClient.publish(it->first.c_str(), 1, true, it->second.c_str());
    }
}

bool MqttManager::connected()
{
    return m_connected;
}

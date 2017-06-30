#include "MqttManager.h"

#include "ArduinoJson.h"

void (*messageReceivedCallback)(std::string , std::string);

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
    payload[len] = '\0';
    messageReceivedCallback(topic, payload);
}


MqttManager::MqttManager()
{
    m_connected = false;

    m_checkConnectivityTimeOnline = 20000;
    m_checkConnectivityTimeOffline = 3000;

    m_deviceStatusInfoTimer.setup(RT_ON, 300000);
    m_checkConnectivityTimer.setup(RT_ON, m_checkConnectivityTimeOffline);
}

void MqttManager::setup(std::string mqttServer, std::string mqttPort, std::string mqttUsername, std::string mqttPassword, bool mqttDiscoveryEnabled)
{
    m_mqttServer = mqttServer;
    m_mqttPort = atoi(mqttPort.c_str());
    m_mqttUsername = mqttUsername;
    m_mqttPassword = mqttPassword;
    m_mqttDiscoveryEnabled = mqttDiscoveryEnabled;

    IPAddress server;
    server.fromString(m_mqttServer.c_str());

    m_mqttClient.onMessage(onMqttMessage);

    m_mqttClient.setCleanSession(true);
    m_mqttClient.setCredentials(mqttUsername.c_str(), mqttPassword.c_str());
    m_mqttClient.setServer(server, m_mqttPort);

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


    if (m_mqttDiscoveryEnabled)
    {
        m_deviceNameSensor = new MqttDiscoverySensor(deviceName + " name");
        m_discoveryComponents.push_back(m_deviceNameSensor);

        m_deviceIpSensor = new MqttDiscoverySensor(deviceName + " IP");
        m_discoveryComponents.push_back(m_deviceIpSensor);

        m_deviceMacSensor = new MqttDiscoverySensor(deviceName + " Mac");
        m_discoveryComponents.push_back(m_deviceMacSensor);

        m_deviceHardwareSensor = new MqttDiscoverySensor(deviceName + " Hardware");
        m_discoveryComponents.push_back(m_deviceHardwareSensor);

        m_deviceFirmwareSensor = new MqttDiscoverySensor(deviceName + " Firmware");
        m_discoveryComponents.push_back(m_deviceFirmwareSensor);

        m_deviceFirmwareVersionSensor = new MqttDiscoverySensor(deviceName + " Firmware Version");
        m_discoveryComponents.push_back(m_deviceFirmwareVersionSensor);
    }
}

void MqttManager::publishDiscoveryInfo()
{
    if (m_mqttDiscoveryEnabled)
    {
        for (auto component : m_discoveryComponents)
        {
            this->publishMQTT(component->getConfigTopic(), component->getConfigPayload());
        }
    }
}

void MqttManager::publishDeviceStatusInfo()
{
    if (!m_mqttDiscoveryEnabled)
    {
        StaticJsonBuffer<200> deviceDataBuffer;
        JsonObject& deviceDataObject = deviceDataBuffer.createObject();
        String deviceDataString;

        deviceDataObject["name"] = m_deviceName.c_str();
        deviceDataObject["ip"] = m_deviceIP.c_str();
        deviceDataObject["mac"] = m_deviceMac.c_str();
        deviceDataObject["hardware"] = m_hardware.c_str();
        deviceDataObject["firmware"] = m_firmware.c_str();
        deviceDataObject["firmware_version"] = m_firmwareVersion.c_str();

        deviceDataObject.printTo(deviceDataString);

        this->publishMQTT(m_deviceDataTopic, deviceDataString.c_str());
    }
    else
    {
        this->publishMQTT(m_deviceNameSensor->state_topic, m_deviceName);
        this->publishMQTT(m_deviceIpSensor->state_topic, m_deviceIP);
        this->publishMQTT(m_deviceMacSensor->state_topic, m_deviceMac);
        this->publishMQTT(m_deviceHardwareSensor->state_topic, m_hardware);
        this->publishMQTT(m_deviceFirmwareSensor->state_topic, m_firmware);
        this->publishMQTT(m_deviceFirmwareVersionSensor->state_topic, m_firmwareVersion);
    }

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
        this->publishDiscoveryInfo();
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

void MqttManager::enableDiscovery(bool enable)
{
    m_mqttDiscoveryEnabled = enable;
}

void MqttManager::addDiscoveryComponent(MqttDiscoveryLight * component)
{
    m_discoveryComponents.push_back(component);

    if (!component->state_topic.empty())
    {
        this->addStatusTopic(component->state_topic);
    }

    if (!component->command_topic.empty())
    {
        this->addSubscribeTopic(component->command_topic);
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
    if (m_statusTopics.find(topic) != m_statusTopics.end() && m_statusTopics[topic] != payload)
    {
        m_statusTopics[topic] = payload;

        if (m_connected)
        {
            this->refreshStatusTopics();
        }
    }
    else if (m_connected)
    {
        m_mqttClient.publish(topic.c_str(), 0, false, payload.c_str());
    }
}

void MqttManager::publishMQTT(std::string topic, float payload)
{
    String output(payload, 1);
    this->publishMQTT(topic, output.c_str());
}

void MqttManager::setCallback(void (*callback)(std::string , std::string))
{
    messageReceivedCallback = callback;
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

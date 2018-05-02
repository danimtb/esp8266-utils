#include "MqttManager.h"

#include "ArduinoJson.h"

void (*messageReceivedCallback)(String, String);

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
    payload[len] = '\0';
    messageReceivedCallback(topic, payload);
}


MqttManager::MqttManager()
{
    m_connected = false;
    m_lastWillPayload = "OFF";


    m_checkConnectivityTimeOnline = 20000;
    m_checkConnectivityTimeOffline = 3000;

    m_deviceStatusInfoTimer.setup(RT_ON, 300000);
    m_checkConnectivityTimer.setup(RT_ON, m_checkConnectivityTimeOffline);
}

void MqttManager::setup(String mqttServer, String mqttPort, String mqttUsername, String mqttPassword, bool mqttDiscoveryEnabled)
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
    m_mqttClient.setKeepAlive(30);
    m_mqttClient.setCredentials(m_mqttUsername.c_str(), m_mqttPassword.c_str());
    m_mqttClient.setServer(server, m_mqttPort);

    m_deviceStatusInfoTimer.start();
    m_checkConnectivityTimer.start();
}

void MqttManager::setDeviceData(String deviceName, String hardware, String deviceIP, String firmware, String firmwareVersion, String discovery_prefix)
{
    m_deviceName = deviceName;
    m_deviceIP = deviceIP;
    m_hardware = hardware;
    m_firmware = firmware;
    m_firmwareVersion = firmwareVersion;

    m_mqttClient.setClientId(m_deviceName.c_str());

    m_deviceDataTopic = "/" + m_deviceName;
    m_lastWillTopic = m_deviceDataTopic + "/status";


    if (m_mqttDiscoveryEnabled)
    {
        m_deviceStatusSensor = new MqttDiscoveryComponent("binary_sensor", m_deviceName + " Status");
        m_deviceStatusSensor->discovery_prefix = discovery_prefix;
        m_deviceStatusSensor->setConfigurtionVariable("device_class", "connectivity");
        m_discoveryComponents.push_back(m_deviceStatusSensor);

        m_deviceIpSensor = new MqttDiscoveryComponent("sensor", m_deviceName + " IP");
        m_deviceIpSensor->discovery_prefix = discovery_prefix;
        m_discoveryComponents.push_back(m_deviceIpSensor);

        m_deviceMacSensor = new MqttDiscoveryComponent("sensor", m_deviceName + " MAC");
        m_deviceMacSensor->discovery_prefix = discovery_prefix;
        m_discoveryComponents.push_back(m_deviceMacSensor);

        m_deviceHardwareSensor = new MqttDiscoveryComponent("sensor", m_deviceName + " Hardware");
        m_deviceHardwareSensor->discovery_prefix = discovery_prefix;
        m_discoveryComponents.push_back(m_deviceHardwareSensor);

        m_deviceFirmwareSensor = new MqttDiscoveryComponent("sensor", m_deviceName + " Firmware");
        m_deviceFirmwareSensor->discovery_prefix = discovery_prefix;
        m_discoveryComponents.push_back(m_deviceFirmwareSensor);

        m_deviceFirmwareVersionSensor = new MqttDiscoveryComponent("sensor", m_deviceName + " Firmware Version");
        m_deviceFirmwareVersionSensor->discovery_prefix = discovery_prefix;
        m_discoveryComponents.push_back(m_deviceFirmwareVersionSensor);

        this->setLastWillMQTT(m_deviceStatusSensor->getStateTopic(), m_lastWillPayload);
    }
    else
    {
        this->setLastWillMQTT(m_lastWillTopic, m_lastWillPayload);
    }
}

void MqttManager::publishDiscoveryInfo()
{
    if (m_mqttDiscoveryEnabled)
    {
        for (auto component : m_discoveryComponents)
        {
            m_mqttClient.publish(component->getConfigTopic().c_str(), 0, false, component->getConfigPayload().c_str());
        }
    }
}

void MqttManager::publishDeviceStatusInfo()
{
    if (!m_mqttDiscoveryEnabled)
    {
        this->publishMQTT(m_deviceDataTopic + "/status", "ON");
        this->publishMQTT(m_deviceDataTopic + "/ip", m_deviceIP);
        this->publishMQTT(m_deviceDataTopic + "/mac", m_deviceMac);
        this->publishMQTT(m_deviceDataTopic + "/hardware", m_hardware);
        this->publishMQTT(m_deviceDataTopic + "/firmware", m_firmware);
        this->publishMQTT(m_deviceDataTopic + "/firmware_version", m_firmwareVersion);
    }
    else
    {
        this->publishMQTT(m_deviceStatusSensor->getStateTopic(), "ON");
        this->publishMQTT(m_deviceIpSensor->getStateTopic(), m_deviceIP);
        this->publishMQTT(m_deviceMacSensor->getStateTopic(), m_deviceMac);
        this->publishMQTT(m_deviceHardwareSensor->getStateTopic(), m_hardware);
        this->publishMQTT(m_deviceFirmwareSensor->getStateTopic(), m_firmware);
        this->publishMQTT(m_deviceFirmwareVersionSensor->getStateTopic(), m_firmwareVersion);
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
        for (uint8_t i = 0; i < m_subscribeTopics.size(); i++)
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
    if (m_deviceMac.length() == 0)
    {
        m_deviceMac = WiFi.macAddress();
    }
}

void MqttManager::enableDiscovery(bool enable)
{
    m_mqttDiscoveryEnabled = enable;
}

void MqttManager::addDiscoveryComponent(MqttDiscoveryComponent * component)
{
    m_discoveryComponents.push_back(component);

    this->addSubscribeTopic(component->getCommandTopic());
    this->addStatusTopic(component->getStateTopic());
}

void MqttManager::addStatusTopic(String statusTopic)
{
    if (statusTopic.length() != 0)
    {
        m_statusTopics[statusTopic] = "";
    }
}

void MqttManager::clearStatusTopics()
{
    m_statusTopics.clear();
}

void MqttManager::addSubscribeTopic(String subscribeTopic)
{
    if (subscribeTopic.length() != 0)
    {
        m_subscribeTopics.push_back(subscribeTopic);
    }
}

void MqttManager::clearSubscribeTopics()
{
    for(uint8_t i=0; i < m_subscribeTopics.size(); i++)
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

void MqttManager::publishMQTT(String topic, String payload)
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
        m_mqttClient.publish(topic.c_str(), 1, true, payload.c_str());
    }
}

void MqttManager::publishMQTT(String topic, float payload)
{
    String output(payload, 1);
    this->publishMQTT(topic, output.c_str());
}

void MqttManager::setCallback(void (*callback)(String , String))
{
    messageReceivedCallback = callback;
}

void MqttManager::setLastWillMQTT(const String& topic, const String& payload)
{
    m_mqttClient.setWill(topic.c_str(), 1, true, payload.c_str());
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
            this->publishDiscoveryInfo();
            this->publishDeviceStatusInfo();

            m_deviceStatusInfoTimer.start(); //restart timer
        }
    }
}

void MqttManager::refreshStatusTopics()
{
    for (std::map<String, String>::iterator it = m_statusTopics.begin(); it != m_statusTopics.end(); it++)
    {
        m_mqttClient.publish(it->first.c_str(), 1, true, it->second.c_str());
    }
}

bool MqttManager::connected()
{
    return m_connected;
}

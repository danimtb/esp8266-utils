#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>

#include "SimpleTimer.h"

#undef min
#undef max

#include <string>

class WifiManager
{
private:
    String m_staSSID;
    String m_staPASS;
    String m_apSSID;

    IPAddress m_ip;
    IPAddress m_mask;
    IPAddress m_gateway;

    String m_macAddress;

    bool m_staMode;
    bool m_apMode;

    SimpleTimer m_checkConnectivityTimer;

    void checkConnectivity();

public:
    WifiManager();

    void setup(String staSSID, String staPASS, String ip, String mask, String gateway, String apSSID);

    void connectStaWifi();
    void createApWifi();
    void disconnectStaWifi();
    void destroyApWifi();

    bool apModeEnabled();

    bool connected();

    void loop();
};

#endif

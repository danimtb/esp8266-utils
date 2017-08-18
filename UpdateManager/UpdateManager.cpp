#include "UpdateManager.h"

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

UpdateManager::UpdateManager()
{
    m_checkUpdateTimer.setup(RT_ON, 600000); // 10 minutes
}

void UpdateManager::setup(String host, String firmware, String firmwareVersion, String hardware)
{
    m_host = host;
    m_firmware = firmware;
    m_firmwareVersion = firmwareVersion;
    m_hardware = hardware;

    m_checkUpdateTimer.start();
}

int16_t UpdateManager::getServerResponse(String &payload)
{
    String request = m_host + "/" + m_firmware + "/" + m_firmwareVersion + "/" + m_hardware;

    HTTPClient http;
    http.begin((char *)request.c_str());
    http.useHTTP10(true);
    http.setTimeout(8000);

    int16_t httpCode = http.GET();

    if (httpCode > 0)
    {
        payload = http.getString().c_str();
    }
    else
    {
        payload = "";
    }

    http.end();

    return httpCode;
}

void UpdateManager::checkUpdate()
{
    String payload;
    String firmwareDownload = "";
    String spiffsDownload = "";

    uint16_t httpCode = this->getServerResponse(payload);


    if (httpCode == 200)
    {
        if (payload.length() != 0)
        {
            StaticJsonBuffer<500> jsonBuffer;
            JsonObject& response = jsonBuffer.parseObject(payload.c_str());

            if (!response.success())
            {
                return; // Error parsing JSON
            }

            if (response.containsKey("version"))
            {
                if (response.containsKey("firmware"))
                {
                    firmwareDownload = response["firmware"].as<char*>();
                }
                else
                {
                    return; // Error: no firmware
                }

                if (response.containsKey("spiffs"))
                {
                    spiffsDownload = response["spiffs"].as<char*>();
                }
                else
                {
                    // No spiffs to flash
                }
            }
            else
            {
                return; // Device is up to date
            }
        }
    }
    else if (httpCode == -1)
    {
        return; // Could not reach update server
    }
    else //(httpCode == 500 || httpCode == 404 || httpCode == 400)
    {
        return; // Bad request
    }

    this->update(firmwareDownload, spiffsDownload);
}

void UpdateManager::update(String firmwarePath, String spiffsPath)
{
    firmwarePath = m_host + firmwarePath;
    spiffsPath = m_host + spiffsPath;

    ESPhttpUpdate.rebootOnUpdate(false);

    if (this->updateSpiffs(spiffsPath))
    {
        if (this->updateFirmware(firmwarePath))
        {
            ESP.restart(); // Update Successful: RESTART DEVICE
        }
    }
}

bool UpdateManager::updateSpiffs(String spiffsUrl)
{
    bool spiffsSuccessful = false;

    if (spiffsUrl.length() != 0)
    {
        t_httpUpdate_return ret = ESPhttpUpdate.updateSpiffs(spiffsUrl.c_str());

        if (ret == HTTP_UPDATE_FAILED)
        {
            spiffsSuccessful = false; //Error: SPIFFS update failed
        }
        else if (ret == HTTP_UPDATE_OK)
        {
            spiffsSuccessful = true; //SPIFFS update successful
        }
    }
    else
    {
        spiffsSuccessful = true; //No SPIFFS to flash, continue
    }

    return spiffsSuccessful;
}

bool UpdateManager::updateFirmware(String firmwareUrl)
{
    bool firmwareSuccessful = false;

    if (firmwareUrl.length() != 0)
    {
        t_httpUpdate_return ret = ESPhttpUpdate.update(firmwareUrl.c_str());

        if (ret == HTTP_UPDATE_FAILED)
        {
            firmwareSuccessful = false; //Error: firmware update failed
        }
        else if (ret == HTTP_UPDATE_OK)
        {
            firmwareSuccessful = true; //Firmware update successful
        }
    }
    else
    {
        firmwareSuccessful = false; //Error: No firmware to flash
    }

    return firmwareSuccessful;
}

void UpdateManager::loop()
{
    if (m_checkUpdateTimer.check())
    {
        this->checkUpdate();
        m_checkUpdateTimer.start();
    }
}

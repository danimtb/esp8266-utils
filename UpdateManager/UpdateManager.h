#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <Arduino.h>

#include "SimpleTimer.h"

#undef min
#undef max


class UpdateManager
{
private:
    SimpleTimer m_checkUpdateTimer;

    String m_host;
    String m_firmware;
    String m_firmwareVersion;
    String m_hardware;

    int16_t getServerResponse(String &payload);
    bool updateSpiffs(String spiffsUrl);
    bool updateFirmware(String firmwareUrl);
    void update(String firmwarePath, String spiffsPath);

public:
    UpdateManager();
    void checkUpdate();
    void setup(String host, String firmware, String firmwareVersion, String hardware);
    void loop();
};

#endif

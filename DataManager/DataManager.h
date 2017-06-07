#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <Arduino.h>
#include <EEPROM.h>

#include <cstdint>
#include <string>

#include "Embedis.h"
#include "spi_flash.h"

class DataManager
{
private:
    Embedis* m_embedis;

public:
    DataManager();
    std::string get(std::string key);
    void set(std::string key, std::string value);

};

#endif

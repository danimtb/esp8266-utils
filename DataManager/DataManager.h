#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <Arduino.h>
#include <EEPROM.h>


#include "Embedis.h"
#include "spi_flash.h"

class DataManager
{
private:
    Embedis* m_embedis;

public:
    DataManager();
    String get(String key);
    void set(String key, String value);
};

#endif

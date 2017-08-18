#include "DataManager.h"

DataManager::DataManager()
{
    m_embedis = new Embedis(Serial);

    EEPROM.begin(SPI_FLASH_SEC_SIZE);

    Embedis::dictionary(F("EEPROM"),
                        SPI_FLASH_SEC_SIZE,
                        [](size_t pos) -> char { return EEPROM.read(pos); },
                        [](size_t pos, char value) { EEPROM.write(pos, value); },
                        []() { EEPROM.commit(); }
                        );
}

String DataManager::get(String key)
{
    String value;

    Embedis::get(key, value);

    return value;
}

void DataManager::set(String key, String value)
{
    Embedis::set(key, value);
}

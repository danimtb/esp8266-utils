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

std::string DataManager::get(std::string key)
{
    String value;

    Embedis::get(String(key.c_str()), value);

    return value.c_str();
}

void DataManager::set(std::string key, std::string value)
{
    Embedis::set(String(key.c_str()), String(value.c_str()));
}

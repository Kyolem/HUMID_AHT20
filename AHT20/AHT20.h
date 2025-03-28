#ifndef AHT20_H
#define AHT20_H

#include <Arduino.h>
#include <Wire.h>

// les registres

#define AHT20_INIT_REG 0xBE
#define AHT20_TRIGGMES_REG 0xAC
#define AHT20_SOFTRESET_REG 0xBA
#define AHT20_STATUT_REG 0x71

class AHT20
{

public:
    // constructeur
    AHT20();

    float theTemperature = 0.0f;
    float theHumidity = 0.0f;

    // méthodes
    bool begin(uint8_t Adress);

    bool getValues();

private:
    uint8_t _adress;
    uint8_t _value;
    uint32_t _Temperature;
    uint32_t _Humidity;
    bool _start();
};

#endif
#include <AHT20.h>
AHT20 ::AHT20()
{
    _adress = 0;
}
//************************************
bool AHT20 ::begin(uint8_t Adress)
{
    _adress = Adress; // chaque objet aura sa propre adresse

    bool res = false;
    uint8_t cal = 0;
    //
    Wire.begin();
    delay(40);
    //

    Wire.beginTransmission(_adress);
    uint8_t theres = Wire.endTransmission();

    //
    Wire.beginTransmission(_adress);
    Wire.write(AHT20_STATUT_REG);
    Wire.endTransmission();
    //
    Wire.requestFrom(_adress, byte(1));
    cal = Wire.read();
    Wire.endTransmission();

    if (cal & 0b00001000 == 0)
    {
        Wire.beginTransmission(_adress);
        Wire.write(AHT20_INIT_REG);
        Wire.write(0x08);
        Wire.write(0x00);
        Wire.endTransmission();
        delay(10);
        res = true;
    }
    else
    {
        res = true;
    }
    Wire.beginTransmission(_adress);
    Wire.write(AHT20_INIT_REG);
    Wire.endTransmission();
    return res;
}

bool AHT20 ::_start()
{
    bool res = false;
    uint8_t Busy = 0;

    Wire.beginTransmission(_adress);
    Wire.write(AHT20_TRIGGMES_REG);
    Wire.write(0x33);
    Wire.write(0x00);
    Wire.endTransmission();
    //
    delay(80);
    uint32_t startchrono = millis();
    do
    {
        if (millis() - startchrono > 200)
        {
            return false;
        }

        delay(1);

        Wire.beginTransmission(_adress);
        Wire.write(AHT20_STATUT_REG);
        Wire.endTransmission();

        Wire.requestFrom(_adress, byte(1));
        Busy = Wire.read();
        Wire.endTransmission();

        // Busy = (Busy & 0x80) >> 7;
    } while (Busy && 0b10000000 == 1);
    //
    return true;
}
bool AHT20 ::getValues()
{
    bool res = false;
    uint8_t tab[7] = {0, 0, 0, 0, 0, 0, 0};
    uint8_t index = 0;
    uint32_t temperature = 0;
    uint32_t humidity = 0;
    //
    res = _start(); // start conv
    if (res == false)
    {
        Serial.println("YASOUCIS");
        return false;
    }
    else
    {
        Wire.requestFrom(_adress, byte(7));
        while (Wire.available())
        {
            tab[index] = Wire.read();

            index++;
        }
        Wire.endTransmission();
        humidity = tab[1] << 12;                  // Bits 20-12
        humidity = humidity | (tab[2] << 4);      // Bits 11-4
        humidity = humidity | (tab[3] & 0xF0);    // Bits 3-0
        humidity = (humidity * 100) / 1048576.0f; // Conversion en pourcentage
        theHumidity = humidity;
        //
        // Calcul de la température
        temperature = ((tab[3] & 0x0F) << 16);               // Bits 19-16
        temperature = temperature | (tab[4] << 8);           // Bits 15-8
        temperature = temperature | tab[5];                  // Bits 7-0
        temperature = (temperature * 200) / 1048576.0f - 50; // Conversion en °C
        theTemperature = temperature;
    }
    return true;
}

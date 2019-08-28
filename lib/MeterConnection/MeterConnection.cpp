#include "MeterConnection.h"

String meterRead()
{
    String meterMessage = String();
    const int txPin = 5;
    const int rxPin = 4;

    digitalWrite(txPin, 0);

    ConfigurableSoftwareSerial swSerWrite(rxPin, txPin, true);
    swSerWrite.begin(300, 1, 'E', 7);
    swSerWrite.enableRx(false);
    Serial.print("swSerWrite.baudRate: ");
    Serial.println(swSerWrite.baudRate());
    swSerWrite.write("/?!\r\n");

    SoftwareSerial swSerReadSlow(rxPin, txPin, true);
    swSerReadSlow.begin(300, SWSERIAL_7N1);
    
    char readChar;
    unsigned int startMs = millis();
    do
    {
        ESP.wdtFeed();
        while (swSerReadSlow.available() > 0)
        {
            readChar = swSerReadSlow.read();
            meterMessage += readChar;
            Serial.write(readChar);
        }
        if (millis() - startMs >= 10000)
        {
            return "Error - timed out waiting for data";
        }
    } while (readChar != (char)'\n');
    swSerReadSlow.end();

    swSerWrite.write(0x06); //ACK
    swSerWrite.write("020\r\n");

    swSerWrite.end();

    SoftwareSerial swSerReadFast(rxPin, txPin, true);
    swSerReadFast.begin(1200, SWSERIAL_7N1);

    startMs = millis();
    do
    {
        ESP.wdtFeed();
        while (swSerReadFast.available() > 0)
        {
            readChar = swSerReadFast.read();
            meterMessage += readChar;
            Serial.write(readChar);
        }
        if (millis() - startMs >= 20000)
        {
            return "Error - timed out waiting for data";
        }

    } while (readChar != (char)0x03); //ETX

    meterMessage.remove(meterMessage.length() - 1);
    meterMessage.remove(0, 1);

    digitalWrite(txPin, 0);
    return meterMessage;
}
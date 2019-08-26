#include "MeterConnection.h"

String meterRead()
{
    const int pin = 5;
    SoftwareSerial swSer1(2, pin, true);
    // swSer1.enableRx(false);
    // swSer1.enableTx(true);
    swSer1.begin(300, SWSERIAL_7N1);
    swSer1.write("/?!\r\n");
    delay(1000);
    swSer1.write(0x06); //ACK
    swSer1.write("000\r\n");
    swSer1.end();
    return "Ok";
}
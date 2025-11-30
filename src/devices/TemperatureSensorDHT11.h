#ifndef __TEMP_SENSOR_DHT11__
#define __TEMP_SENSOR_DHT11__

#include "TempSensor.h"
#include <DHT.h>

#define DHTTYPE DHT11 

class TempSensorDHT11 : public TempSensor {
    public:
        TempSensorDHT11(uint8_t pin);

        float getTemperature() override;

        // metodo extra specifico del DHT11
        float getHumidity();

    private:
        DHT dht;
};

#endif

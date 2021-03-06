#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "dht_temphum.h"
#include "airq_ccs811.h"
#include "pressure.h"

void appLoop();

ustd::Scheduler sched(10, 16, 32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;

#ifdef __ESP32__
ustd::Dht dht("myDht", 15);
#else
ustd::Dht dht("myDht", D4);
#endif
ustd::AirQualityCCS811 airqual("myAirQuality", 0x5a,
                               "myDht/sensor");  // I2C address of spark fun CCS811
ustd::Pressure pressure("myPressure");

void sensor_messages(String topic, String msg, String originator) {
#ifdef USE_SERIAL_DBG
    Serial.println("Switch received: " + topic + "|" + msg);
#endif
    if (topic == "myDht/sensor/temperature") {
#ifdef USE_SERIAL_DBG
        Serial.println("Temperature: " + msg);
#endif
    }
    if (topic == "myDht/sensor/humidity") {
#ifdef USE_SERIAL_DBG
        Serial.println("Humidity: " + msg);
#endif
    }
    if (topic == "myAirQuality/sensor/co2") {
#ifdef USE_SERIAL_DBG
        Serial.println("CO2: " + msg);
#endif
    }
    if (topic == "myAirQuality/sensor/voc") {
#ifdef USE_SERIAL_DBG
        Serial.println("VOC: " + msg);
#endif
    }
}

uint8_t hwErrs = 0;
uint8_t i2cDevs = 0;
bool i2c_checkAddress(uint8_t address) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();
    if (error == 0) {
        return true;
    } else if (error == 4) {
        ++hwErrs;
    }
    return false;
}

void i2c_doctor() {
    i2cDevs = 0;
    hwErrs = 0;
    for (uint8_t address = 1; address < 127; address++) {
        if (i2c_checkAddress(address)) {
            ++i2cDevs;
            char msg[32];
            sprintf(msg, "0x%02x", address);
            sched.publish("i2c/doctor/device", msg);
        }
    }
    sched.publish("i2c/doctor/devicecount", String(i2cDevs));
    sched.publish("i2c/doctor/hwErrorCount", String(hwErrs));
}

void runDoctor(String topic, String msg, String originator) {
    i2c_doctor();
}

void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
    Serial.println("Startup");
#endif  // USE_SERIAL_DBG
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
    int tID = sched.add(appLoop, "main", 1000000);
    dht.begin(&sched);

#ifdef __ESP32__
    Wire.begin();
#else
    const int sclPin = D1;
    const int sdaPin = D2;
    pinMode(sdaPin, INPUT_PULLUP);  // Set input (SDA) pull-up resistor on
    // Wire.setClock(2000000);
    Wire.begin(sdaPin, sclPin);
// Wire.setClockStretchLimit(500);
#endif

    airqual.begin(&sched);
    pressure.begin(&sched);

    dht.registerHomeAssistant("Labor", "Breadboard");
    airqual.registerHomeAssistant("Labor", "Breadboard");
    pressure.registerHomeAssistant("Labor", "Breadboard");

    sched.subscribe(tID, "myDht/sensor/#", sensor_messages);
    sched.subscribe(tID, "myAirQuality/sensor/#", sensor_messages);
    sched.subscribe(tID, "myPressure/sensor/#", sensor_messages);
    sched.subscribe(tID, "i2c/doctor", runDoctor);
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
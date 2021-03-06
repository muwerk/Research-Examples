#define USE_SERIAL_DBG 1

#include "../../../ustd/platform.h"
#include "scheduler.h"
#include "../../../munet/net.h"
#include "../../../munet/mqtt.h"
#include "ota.h"

#include "pressure_bmp280.h"

void appLoop();

ustd::Scheduler sched(10, 16, 32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;

ustd::PressureBmp280 pressure("Pressure_BMP280");

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

#ifdef __ESP32__
    Wire.begin();
#else
    // const int sclPin = D1;
    // const int sdaPin = D2;
    // pinMode(sdaPin, INPUT_PULLUP); //Set input (SDA) pull-up resistor on
    Wire.begin();  // sdaPin, sclPin);
#endif

    pressure.begin(&sched);
    pressure.registerHomeAssistant("Labor3", "Breadboard3");

    sched.subscribe(tID, "i2c/doctor",
                    runDoctor);  // publish to <hostname>/i2c/doctor  to get enumerations of i2c
                                 // devices to MQTT omu/<hostname>/i2c/doctor/#
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
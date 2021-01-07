#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "mup_util.h"
#include "jsonfiles.h"

#include "Wire.h"
#include <SPI.h>

//#include "SSD1306Spi.h"
#include "SSD1306Wire.h"

#include "switch.h"
#include "led.h"
#include "frequency_counter.h"

SSD1306Wire display(0x3c, 21, 22);  // i2c, SDA, SCL ;

ustd::Led led("indicator", 2, true);
// ustd::Switch geiger("geiger", 15, ustd::Switch::Mode::Falling, false, "", 0, 0);
ustd::FrequencyCounter geiger("geiger", 15, 0,
                              ustd::FrequencyCounter::MeasureMode::LOWFREQUENCY_MEDIUM);

void appLoop();

ustd::Scheduler sched(10, 16, 32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;

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

double freq;
void subMsg(String topic, String msg, String originator) {
    if (topic == "geiger/sensor/frequency") {
        freq = msg.toFloat();
    }
}

void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
    Serial.println("Startup");
#endif  // USE_SERIAL_DBG
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
    int tID = sched.add(appLoop, "main", 100000);

    geiger.begin(&sched);
    led.begin(&sched);

#ifdef __ESP32__
    Wire.begin();
#endif

    sched.subscribe(tID, "i2c/doctor",
                    runDoctor);  // publish to <hostname>/i2c/doctor  to get enumerations of
                                 // i2c devices to MQTT omu/<hostname>/i2c/doctor/#
    sched.subscribe(tID, "geiger/sensor/frequency", subMsg);

    display.init();
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_16);  // 10, 16, 24
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.clear();
    String msg = ">     ";
    display.drawString(5, 5, msg);
    display.display();

    sched.publish("indicator/light/mode/set", "pulse 500");
}

void appLoop() {
    String msgd = "> " + String(freq) + " Hz   ";
    display.clear();
    display.drawString(5, 5, msgd);
    display.display();

    // sched.publish("indicator/light/mode/set", "pulse 70");
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}

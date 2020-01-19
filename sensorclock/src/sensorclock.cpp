#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"

#include "net.h"
#include "mqtt.h"
#include "ota.h"
#include "../../../munet/web.h"

//#include "led.h"
#include "switch.h"
#include "mup_util.h"

#include "clock7seg.h"
#include "tsl2561.h"
#include "dht_temphum.h"
#include "pressure.h"
#include "switch.h"
#include "airqual.h"


void appLoop();

ustd::Scheduler sched(10,16,32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;
ustd::Web web;

ustd::Clock7Seg clock7("clock", 0x70, D5, true, "tsl2561/sensor/unitilluminance");
ustd::Illuminance illumin("tsl2561", 0x39, "1x", "medium", 28.0);
ustd::Dht dht("dht22", 0, DHT22);  // port 0 == D3
ustd::Pressure pressure("bmp085");
ustd::Switch sw1("sensorclock1", D7);
ustd::Switch sw2("sensorclock2", D6,ustd::Switch::Mode::Default,false,
                 "clock/alarm/off");
ustd::AirQuality airq("air",0x5a);



void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
    Serial.println("Startup");
#endif  // USE_SERIAL_DBG
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
    web.begin(&sched);

    #if defined(I2C_D1_D2)
        #ifdef USE_SERIAL_DBG
            Serial.println("Using slightly non-standard I2C port D1, D2");
        #endif
        Wire.begin(D1, D2);  // SDA, SCL; Non-standard, from the old days...
    #endif

    int tID = sched.add(appLoop, "main", 1000000); // every 1000000 micro sec = once a second call appLoop
    clock7.begin(&sched);
    clock7.maxAlarmDuration = 10;
    illumin.begin(&sched);
    dht.begin(&sched);
    pressure.begin(&sched);
    sw1.begin(&sched);
    sw2.begin(&sched);
    airq.begin(&sched);

    // Use Home Assistant's auto-discovery to register sensors:
    illumin.registerHomeAssistant("Illuminance","Sensor Uhr");
    dht.registerHomeAssistant("Klima","Sensor Uhr");
    pressure.registerHomeAssistant("Luftdruck","Sensor Uhr");
    airq.registerHomeAssistant("Luftquali","Sensor Uhr");
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}


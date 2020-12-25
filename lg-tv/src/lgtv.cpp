#include "platform.h"
#include "scheduler.h"

#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "tv_serial.h"

void appLoop();

ustd::Scheduler sched(10, 16, 32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;

#ifdef __ESP32__
ustd::TvSerial tv("LG-TV", &Serial1, ustd::TvSerial::TV_SERIAL_TYPE::LG_TV);
#else  // !__ESP32__
#ifdef USE_SERIAL_DBG
#error You cannot use define USE_SERIAL_DBG with TvSerial mupplet,
// since the serial port is needed for the communication with the TV hardware!
#endif  // USE_SERIAL_DBG
ustd::TvSerial tv("TV", (HardwareSerial *)&Serial, ustd::TvSerial::TV_SERIAL_TYPE::LG_TV);
#endif  // !__ESP32__

void setup() {
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);

    tv.begin(&sched);
    // Use Home Assistant's auto-discovery to register tv in HA as switch.
    tv.registerHomeAssistant("Fernseher", "ESP Serial TV");

    // Currently unused process appLoop:
    /*int tID = */ sched.add(appLoop, "main",
                             1000000);  // every 1000000 micro sec = once a second call appLoop
}

void appLoop() {
    // unused. Use this for application logic.
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
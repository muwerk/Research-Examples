#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"

#include "net.h"
#include "mqtt.h"
#include "ota.h"
#include "../../../munet/web.h"

#include "mup_util.h"
#include "illuminance_ldr.h"
#include "neocandle.h"

void appLoop();

ustd::Scheduler sched(10, 16, 32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;
ustd::Web web;

ustd::Ldr ldr("ldr", A0);
ustd::NeoCandle candles("candles", NEOCANDLE_PIN, NEOCANDLE_NUMPIXELS, NEOCANDLEX_OPTIONS);

void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
    Serial.println("Startup");
#endif  // USE_SERIAL_DBG
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
    web.begin(&sched);

    /* int tID = */
    sched.add(appLoop, "main", 1000000);  // every 1000000 micro sec = once a second call appLoop

    ldr.begin(&sched);
    candles.wind = 50;
    candles.begin(&sched);

    // Use Home Assistant's auto-discovery to register the butterlamp
    // ldr.registerHomeAssistant("Butterlampe-1 Helligkeitssensor", "Butterlampe-1");
    candles.registerHomeAssistant("Butterlampe Drei", "Butterlampe-3");
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}

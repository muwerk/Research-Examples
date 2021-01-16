#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"
#include "console.h"

#include "net.h"
#include "mqtt.h"
#include "ota.h"
#include "../../../munet/web.h"

#include "mup_util.h"
#include "illuminance_ldr.h"
#include "neocandle.h"

void appLoop();

ustd::Scheduler sched(10, 16, 32);
ustd::SerialConsole con;
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

    ustd::jsonfile jf;
    bool hasLdr = jf.readBool("neocandle/has_ldr", false);
    String friendlyName = jf.readString("neocandle/friendly_name", "neocandle");
    bool candleTimer = jf.readBool("neocandle/candle_timer", true);
    bool registerHomeAssistant = jf.readBool("neocandle/homeassistant", false);
    String startTime, endTime;
    uint8_t sh, sm, eh, em;
    if (candleTimer) {
        startTime = jf.readString("neocandle/start_time", "18:00");
        endTime = jf.readString("neocandle/end_time", "0:00");
        if (!ustd::NeoCandle::parseHourMinuteString(startTime, &sh, &sm))
            candleTimer = false;
        if (!ustd::NeoCandle::parseHourMinuteString(endTime, &eh, &em))
            candleTimer = false;
    }

    /* int tID = */
    sched.add(appLoop, "main", 1000000);  // every 1000000 micro sec = once a second call appLoop

    if (hasLdr)
        ldr.begin(&sched);
    candles.wind = 50;
    if (candleTimer) {
        candles.begin(&sched, sh, sm, eh, em);
    } else {
        candles.begin(&sched);
    }

    // Use Home Assistant's auto-discovery to register the butterlamp
    if (registerHomeAssistant) {
        if (hasLdr)
            ldr.registerHomeAssistant(friendlyName + " Helligkeitssensor", "Butterlampe-1");
        candles.registerHomeAssistant(friendlyName, friendlyName);
    }
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}

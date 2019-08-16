#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "led.h"
#include "switch.h"

void appLoop();

ustd::Scheduler sched(10,16,32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;
ustd::Led ledblue("LedBlue",D8,false);
ustd::Switch switchblack("switchblack",D7, ustd::Switch::Mode::Flipflop, false);
ustd::Led ledyellow("LedYellow",D6,false);
ustd::Switch switchblue("switchblue",D5, ustd::Switch::Mode::Flipflop, false);

void switch_messages(String topic, String msg, String originator) {
    #ifdef USE_SERIAL_DBG
    Serial.println("Switch received: "+topic+"|"+msg);
    #endif
    if (topic == "switchblack/switch/state") {
        if (msg=="on") {
            ledblue.setMode(ustd::Led::Mode::Blink, 500,0.0);
//            ledblue.set(true);
        } else {
            ledblue.set(false);
        }
    }
    if (topic == "switchblue/switch/state") {
        if (msg=="on") {
            ledyellow.setMode(ustd::Led::Mode::Blink, 500,0.5);
//            ledyellow.set(true);
        } else {
            ledyellow.set(false);
        }
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
    int tID = sched.add(appLoop, "main", 1000000);
    ledblue.begin(&sched);
    ledyellow.begin(&sched);
    switchblack.begin(&sched);
    switchblue.begin(&sched);
    sched.subscribe(tID, "+/switch/state", switch_messages);
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
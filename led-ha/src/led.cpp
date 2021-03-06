#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"

#include "net.h"
#include "mqtt.h"
#include "ota.h"
//#include "../../../munet/web.h"

#include "led.h"
#include "switch.h"
#include "mup_util.h"

void appLoop();

ustd::Scheduler sched(10, 16, 32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;
// ustd::Web web;

#ifdef __ESP32__
ustd::Led led("myLed", 14, false);
// ustd::Switch toggleswitch("mySwitch",32, ustd::Switch::Mode::Default, false);
// Optional IRQ support: (each switch needs unique interruptIndex [0..9])
ustd::Switch toggleswitch("mySwitch", 32, ustd::Switch::Mode::Flipflop, false,
                          "mySwitch/switch/IRQ/0", 0, 25);
#else
ustd::Led led("myLed", D5, false);
ustd::Switch toggleswitch("mySwitch", D6, ustd::Switch::Mode::Default, false);
// Optional IRQ support: (each switch needs unique interruptIndex [0..9])
// ustd::Switch toggleswitch("mySwitch",D6, ustd::Switch::Mode::Flipflop, false,
// "mySwitch/switch/IRQ/0", 0, 25);
#endif

void switch_messages(String topic, String msg, String originator) {
#ifdef USE_SERIAL_DBG
    Serial.println("Switch received: " + topic + "|" + msg);
#endif
    if (topic == "mySwitch/switch/state") {
        if (msg == "on") {
            led.set(true);
            // sched.publish("relay/myLed/light/set","on");
        } else if (msg == "off") {
            led.set(false);
            // sched.publish("relay/myLed/light/set","off");
        } else if (msg == "trigger") {
            led.setMode(ustd::Led::Mode::Pulse, 50);
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
    // web.begin(&sched);

    int tID = sched.add(appLoop, "main",
                        1000000);  // every 1000000 micro sec = once a second call appLoop
    led.begin(&sched);
    toggleswitch.begin(&sched);
    toggleswitch.setMode(ustd::Switch::Mode::Flipflop);

    // Use Home Assistant's auto-discovery to register switch and led in HA with
    // names DigiTast, Blaue Led.
    toggleswitch.registerHomeAssistant("DigiTast", "Custom Hardware");
    led.registerHomeAssistant("Blaue Led", "Custom Hardware");

    // led.setMode(led.Mode::Blink,1000);
    sched.subscribe(tID, "mySwitch/switch/state", switch_messages);

    sched.publish("Starting", "Write");
    // String tmp;
    // if (ustd::readJson("/net.json",tmp)) {
    //     JSONVar jsonObj = JSON.parse(tmp);
    //     ustd::writeJson("/test.json",jsonObj);
    // }
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
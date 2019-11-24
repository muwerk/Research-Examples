//#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"

#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "led.h"
#include "switch.h"

void appLoop();

ustd::Scheduler sched(10,16,32);
ustd::Net net(1);
ustd::Mqtt mqtt;
ustd::Ota ota;

ustd::Led led1("Led1",1,false);
ustd::Led led2("Led2",1,false);
ustd::Switch toggleswitch("mySwitch",14, ustd::Switch::Mode::Default, false);
// Optional IRQ support: (each switch needs unique interruptIndex [0..9])
// ustd::Switch toggleswitch("mySwitch",D6, ustd::Switch::Mode::Flipflop, false, "mySwitch/switch/IRQ/0", 0, 25);

void switch_messages(String topic, String msg, String originator) {
#ifdef USE_SERIAL_DBG
    Serial.println("Switch received: "+topic+"|"+msg);
#endif
    if (topic == "mySwitch/switch/state") {
        if (msg=="on") {
            led1.set(true);
            // sched.publish("relay/Led1/light/set","on");
        } else if (msg=="off") {
            led1.set(false);
            // sched.publish("relay/Led1/light/set","off");
        } else if (msg=="trigger") {
            led1.setMode(ustd::Led::Mode::Pulse, 50);
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
    int tID = sched.add(appLoop, "main", 1000000); // every 1000000 micro sec = once a second call appLoop
    led1.begin(&sched);
    led2.begin(&sched);
    toggleswitch.begin(&sched);
    toggleswitch.setMode(ustd::Switch::Mode::Flipflop);

    // Use Home Assistant's auto-discovery to register switch and led in HA with names DigiTast, Blaue Led.
    toggleswitch.registerHomeAssistant("Gosund 01 Switch", "Gosund-SP1 01");
    led1.registerHomeAssistant("Gosund 01 Led1", "Gosund-SP1 01");
    led2.registerHomeAssistant("Gosund 01 Led2", "Gosund-SP1 01");

    // led.setMode(led.Mode::Blink,1000);
    sched.subscribe(tID, "mySwitch/switch/state", switch_messages);
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
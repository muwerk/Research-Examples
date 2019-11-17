#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"

#include "net.h"
#include "mqtt.h"
//#include "ota.h"

//#include "/home/dsc/gith/muwerk/munet/net.h"
//#include "/home/dsc/gith/muwerk/munet/mqtt.h"
//#include "/home/dsc/gith/muwerk/munet/ota.h"

#include "led.h"
#include "switch.h"

void appLoop();

ustd::Scheduler sched(10,16,32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
//ustd::Ota ota;
#ifdef __ESP32__
ustd::Led led("myLed",14,false);
ustd::Switch toggleswitch("mySwitch",32, ustd::Switch::Mode::Default, false);
#else
ustd::Led led("myLed",D5,false);
ustd::Switch toggleswitch("mySwitch",D6, ustd::Switch::Mode::Default, false);
#endif
// ustd::Switch toggleswitch("mySwitch",D6, ustd::Switch::Mode::Default, false);
// Optional IRQ support: (each switch needs unique interruptIndex [0..9])
// ustd::Switch toggleswitch("mySwitch",D6, ustd::Switch::Mode::Flipflop, false, "mySwitch/switch/IRQ/0", 0, 25);

void switch_messages(String topic, String msg, String originator) {
#ifdef USE_SERIAL_DBG
    Serial.println("Switch received: "+topic+"|"+msg);
#endif
    if (topic == "mySwitch/switch/state") {
        if (msg=="on") {
            led.set(true);
            sched.publish("relay/myLed/led/set","on");
        } else if (msg=="off") {
            led.set(false);
            sched.publish("relay/myLed/led/set","off");
        } else if (msg=="trigger") {
            led.setMode(ustd::Led::Mode::Pulse, 50);
        }
    }
}

void mqttCallback(String topic, String msg, String originator) {
    sched.publish("!ledapp/mqtt/relay",msg);
}

void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
    Serial.println("Startup");
#endif  // USE_SERIAL_DBG
    net.begin(&sched);
    mqtt.begin(&sched);
    //ota.begin(&sched);
    int tID = sched.add(appLoop, "main", 1000000);
    led.begin(&sched);
    toggleswitch.begin(&sched);

    // Use Home Assistant's auto-discovery to register switch in HA with name DigiSwitch
    toggleswitch.registerHomeAssistant("DigiTast");

    // led.setMode(led.Mode::Blink,1000);
    sched.subscribe(tID, "mySwitch/switch/state", switch_messages);
    sched.subscribe(tID,"mqtt/state",mqttCallback);
}

void appLoop() {
    //sched.publish("!testdirect/for/example","no message here");
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
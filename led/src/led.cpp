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
ustd::Led led("myLed",D5,false);
ustd::Switch toggleswitch("mySwitch",D6, false);

void switch_messages(String topic, String msg, String originator) {
    Serial.println("Switch received: "+topic+"|"+msg);
    if (topic == "mySwitch/switch/state") {
        if (msg=="on") {
            led.set(true);
            //sched.publish("myLed/led/set","on");
        } else {
            led.set(false);
            //sched.publish("myLed/led/set","off");
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
    led.begin(&sched);
    toggleswitch.begin(&sched);

    //led.setMode(led.Mode::PULSE,1000);
    sched.subscribe(tID, "mySwitch/switch/state", switch_messages);
}

void appLoop() {
    //if (led.state) led.set(false);
    //else led.set(true);
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
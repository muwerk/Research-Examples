//#define USE_SERIAL_DBG 0

#include "platform.h"
#include "scheduler.h"

#ifdef __ATMEGA__
#include "functional.h"
#endif

#ifdef __ESP__
#include "net.h"
#include "mqtt.h"
//#include "ota.h"
#endif

#ifndef __ATTINY__
#include "led.h"
#endif
//#include "switch.h"

void appLoop();

ustd::Scheduler sched(10,16,32);
#ifdef __ESP__
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
//ustd::Ota ota;
#endif

#ifndef __ATTINY__
ustd::Led led("myLed",LED_BUILTIN,false);
#endif

void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
    Serial.println("Startup");
#endif  // USE_SERIAL_DBG
#ifdef __ESP__
    net.begin(&sched);
    mqtt.begin(&sched);
//    ota.begin(&sched);
#endif
    /* int tID = */ sched.add(appLoop, "main", 1000000);
#ifndef __ATTINY__
    led.begin(&sched);
#endif
    // led.setMode(led.Mode::Wave,1000);
}

bool ls=false;
void appLoop() {
#ifndef __ATTINY__
    if (ls==false) {
        led.set(true);
        ls=true;
    } else {
        led.set(false);
        ls=false;
    }
#endif
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}

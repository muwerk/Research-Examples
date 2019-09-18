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
ustd::Led led("myLed",LED_BUILTIN,false);

void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
    Serial.println("Startup");
#endif  // USE_SERIAL_DBG
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
    /* int tID = */ sched.add(appLoop, "main", 1000000);
    led.begin(&sched);

    // led.setMode(led.Mode::Wave,1000);
}

bool ls=false;
void appLoop() {
    if (ls==false) {
        led.set(true);
        ls=true;
    } else {
        led.set(false);
        ls=false;
    }
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}

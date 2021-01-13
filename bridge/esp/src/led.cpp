#include "platform.h"
#include "scheduler.h"

#include "muserial.h"
#ifdef __ESP__
#include "net.h"
#include "mqtt.h"
#include "ota.h"
#endif

void appLoop();

ustd::Scheduler sched(10, 16, 32);
ustd::MuSerial muser("node1", &Serial);

#ifdef __ESP__
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
// ustd::Ota ota;
#endif

void setup() {
    muser.begin(&sched);
#ifdef __ESP__
    net.begin(&sched);
    mqtt.begin(&sched);
//    ota.begin(&sched);
#endif
    sched.add(appLoop, "main", 1000000);
    // led.setMode(led.Mode::Wave,1000);
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}

//#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"

#include "console.h"

#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "web.h"

#include "mup_util.h"
#include "jsonfiles.h"

ustd::Scheduler sched(10, 16, 32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;
ustd::Web web;

ustd::SerialConsole con;

void appLoop();

void setup() {

    con.begin(&sched);

    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
    web.begin(&sched);

    int tID = sched.add(appLoop, "main", 3000000);
}

void appLoop() {
    sched.publish("debug/msg", "alive");
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}

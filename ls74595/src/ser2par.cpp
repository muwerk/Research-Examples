#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "shift_reg_74595.h"

void appLoop();

ustd::Scheduler sched(10, 16, 32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;

ustd::ShiftReg sr("Shift", D7, D5, D8, true);

void setup() {
    Serial.begin(115200);
    Serial.println("Start");
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
    sr.begin(&sched);
    Serial.println("End init");
    /*int tID = */ sched.add(appLoop, "main", 1000000);
}

bool sw = false;
void appLoop() {
    if (sw) {
        sw = false;
        sr.pulseBit(1, 200);
    } else {
        sr.pulseBit(2, 200);
        sw = true;
    }
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
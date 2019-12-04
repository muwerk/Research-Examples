#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "shift_reg_74595.h"

void appLoop();

ustd::Scheduler sched(10,16,32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;

ustd::ShiftReg sr("Shift",D5,D6,D7);


void setup() {
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
    int tID = sched.add(appLoop, "main", 200000);

    sr.begin(&sched);
}

uint8_t b=1;
void appLoop() {
    sr.writeShiftReg(b);
    if (b<128) b=b+b;
    else b=1;
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
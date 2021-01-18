#define USB_SERIAL_DBG 1

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
ustd::MuSerial muser("arduino", &Serial);

#ifdef __ESP__
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;
#endif

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    muser.begin(&sched);
#ifdef __ESP__
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
#endif
    sched.add(appLoop, "main", 1000000);
    // led.setMode(led.Mode::Wave,1000);
}

bool bl = false;
void appLoop() {
    if (bl) {
        bl = false;
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
        bl = true;
        digitalWrite(LED_BUILTIN, LOW);
    }
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}

#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"

#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "dcc.h"

void appLoop();

ustd::Scheduler sched(10,16,32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;

#ifdef __ESP32__
//ustd::Dcc dcc("Märklin",ustd::Dcc::Mode::DCC, 21, 0, 26,25);
ustd::Dcc dcc("Märklin",ustd::Dcc::Mode::DC, 21, 0);
#else
#error Not implemented.
//ustd::Dcc dcc("Märklin",D6,D7,D8);
#endif

void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
    Serial.println("Startup");
#endif  // USE_SERIAL_DBG
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
    dcc.begin(&sched);

    dcc.setTrainSpeed(60);
    /* int tID = */sched.add(appLoop, "main", 1000000); 
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
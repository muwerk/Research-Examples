#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "ldr.h"
#include "tsl2561.h"

void appLoop();

ustd::Scheduler sched(10,16,32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;
#ifdef __ESP32__
ustd::Ldr ldr("myLdr",A9); // caution:  All pins connected to ADC #2 conflict with wifi!
#else
ustd::Ldr ldr("myLdr",A0);
#endif
ustd::Illuminance illum("myLum",0x39); 

void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
    Serial.println("Startup");
#endif  // USE_SERIAL_DBG
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
    /*int tID = */
    sched.add(appLoop, "main", 1000000);
    ldr.begin(&sched);
    illum.begin(&sched);

    ldr.registerHomeAssistant("Labor LDR", "Enlightenment");
    illum.registerHomeAssistant("Labor TSL2561", "Enlightenment");
    //ldr.illuminanceSensor.pollTimeSec=1;
    //illum.illuminanceSensor.pollTimeSec=1;   
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}

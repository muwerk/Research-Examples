#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "dht_temphum.h"

void appLoop();

ustd::Scheduler sched(10,16,32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;
ustd::Dht dht("myDht",D4);

void sensor_messages(String topic, String msg, String originator) {
#ifdef USE_SERIAL_DBG
    Serial.println("Switch received: "+topic+"|"+msg);
#endif
    if (topic == "myDht/temperature") {
#ifdef USE_SERIAL_DBG
        Serial.println("Temperature: "+msg);
#endif
    }
    if (topic == "myDht/humidity") {
#ifdef USE_SERIAL_DBG
        Serial.println("Humidity: "+msg);
#endif
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
    dht.begin(&sched);

    sched.subscribe(tID, "myDht/#", sensor_messages);
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
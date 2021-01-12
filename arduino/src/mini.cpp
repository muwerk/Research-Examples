//#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"

//#include "net.h"
//#include "ota.h"
//#include "mqtt.h"
#include "console.h"

// entities for core functionality
ustd::Scheduler sched(2, 2, 2);
ustd::Console con;
// ustd::Net       net( LED_BUILTIN );
// ustd::Ota       ota;
// ustd::Mqtt      mqtt;

// main application command handler
void app_command_handler(String topic, String msg, String originator) {
    /*
    if (topic == "cmnd/restart") {
        //        ESP.restart();
    } else if (topic == "cmnd/test1") {
    } else if (topic == "cmnd/test2") {
    } else if (topic == "cmnd/test3") {
    } else if (topic == "cmnd/test2") {
    }
    */
}

// main application task
void app() {
}

// application setup
void setup() {
    DBG_INIT(115200);
    // DBG( "\nStartup: " + WiFi.macAddress() );

    // extend console
    /*
    con.extend("hurz", [](String cmd, String args) {
        Serial.println("Im brennenden Walde waren die Löwen frisch!");
        if (args.length()) {
            Serial.println(args);
        }
    });
    */
    // initialize core components
    con.begin(&sched, "", 115200);
    // net.begin( &sched );
    // ota.begin( &sched );
    // mqtt.begin( &sched, "", "", "" );

    // initialize application

    int tID = sched.add(app, "main", 50000);
    // sched.subscribe(tID, "cmnd/#", app_command_handler);
}

// application runtime
void loop() {
    sched.loop();
}
//#define USE_SERIAL_DBG 1

#include "ustd_platform.h"
#include "scheduler.h"

#include "console.h"

#include "net.h"
#include "mqtt.h"
#include "ota.h"
#include "web.h"

//#include "led.h"
#include "switch.h"
#include "mup_util.h"

#include "clock7seg.h"
#include "illuminance_tsl2561.h"
#include "temp_hum_dht.h"
#include "pressure.h"
#include "switch.h"
#include "airq_ccs811.h"
#include "doctor.h"
#include "i2cdoctor.h"

#include "mup_oled.h"

void appLoop();

ustd::Scheduler sched(10, 16, 32);
ustd::SerialConsole con;

ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;
ustd::Web web;

ustd::Doctor doctor("doctor");
ustd::I2CDoctor i2cdoctor("i2cdoctor");
ustd::IlluminanceTsl2561 illumin("tsl2561", 0x39);
ustd::Dht dht("dht22", 0, DHT22);  // port 0 == D3
ustd::Pressure pressure("bmp085");
#ifdef I2C_D1_D2
ustd::Clock7Seg clock7("clock", 0x70, D5, true, "tsl2561/sensor/unitilluminance");
ustd::Switch sw1("sensorclock1", D7);
ustd::Switch sw2("sensorclock2", D6, ustd::Switch::Mode::Default, false, "clock/alarm/off");
ustd::SensorDisplay display("display",128,64,0x3c);
#else
ustd::Clock7Seg clock7("clock", 0x70, 14, true, "tsl2561/sensor/unitilluminance");
#endif
ustd::AirQualityCCS811 airq("air", 0x5a, "dht22/sensor");


void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
    Serial.println("Startup");
#endif  // USE_SERIAL_DBG
    doctor.begin(&sched);
    i2cdoctor.begin(&sched, &Wire);
    con.begin(&sched);

    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
    web.begin(&sched);


#if defined(I2C_D1_D2)
#ifdef USE_SERIAL_DBG
    Serial.println("Using slightly non-standard I2C port D1, D2");
#endif
    Wire.begin(D1, D2);  // SDA, SCL; Non-standard, from the old days...
#endif

    display.begin(&sched, &mqtt);

    clock7.begin(&sched);
    clock7.maxAlarmDuration = 10;
    illumin.setMaxLux(200);
    illumin.begin(&sched);
    dht.begin(&sched);
    pressure.begin(&sched);
#ifdef I2C_D1_D2
    sw1.begin(&sched);
    sw2.begin(&sched);
#endif
    airq.begin(&sched);

    illumin.registerHomeAssistant("Illuminance", "Sensor Uhr");
    dht.registerHomeAssistant("Klima", "Sensor Uhr");
    pressure.registerHomeAssistant("Luftdruck", "Sensor Uhr");
    airq.registerHomeAssistant("Luftquali", "Sensor Uhr");
    /*int tID =*/sched.add(appLoop, "main",
                           1000000);  // every 1000000 micro sec = once a second call appLoop

    // Use Home Assistant's auto-discovery to register sensors:
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}

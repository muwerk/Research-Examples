#define USE_SERIAL_DBG 1
// xx
#include "platform.h"
#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "mup_util.h"
#include "jsonfiles.h"

#include "airq_bsec_bme680.h"
#include "temperature_mcp9808.h"
#include "airq_bme280.h"
#include "temp_hum_dht.h"

void appLoop();

ustd::Scheduler sched(10, 16, 32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;

ustd::AirQualityBsecBme680 airqual680("AirQuality680", BME680_I2C_ADDR_SECONDARY);
ustd::TemperatureMCP9808 precTemp("PrecTemperature");
ustd::AirQualityBme280 airqual280("AirQuality280", 0x76);
ustd::Dht dht("dht22", 21, DHT22);

uint8_t hwErrs = 0;
uint8_t i2cDevs = 0;
bool i2c_checkAddress(uint8_t address) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();
    if (error == 0) {
        return true;
    } else if (error == 4) {
        ++hwErrs;
    }
    return false;
}

void i2c_doctor() {
    i2cDevs = 0;
    hwErrs = 0;
    for (uint8_t address = 1; address < 127; address++) {
        if (i2c_checkAddress(address)) {
            ++i2cDevs;
            char msg[32];
            sprintf(msg, "0x%02x", address);
            sched.publish("i2c/doctor/device", msg);
        }
    }
    sched.publish("i2c/doctor/devicecount", String(i2cDevs));
    sched.publish("i2c/doctor/hwErrorCount", String(hwErrs));
}

void runDoctor(String topic, String msg, String originator) {
    i2c_doctor();
}

void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
    Serial.println("Startup");
#endif  // USE_SERIAL_DBG
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
    int tID = sched.add(appLoop, "main", 3000000);

#ifdef __ESP32__
    Wire.begin();
#endif

    airqual680.begin(&sched);
    airqual280.begin(&sched);
    precTemp.begin(&sched);
    dht.begin(&sched);

    String tt = "doesn't work!";
    if (ustd::readFriendlyName(tt)) {
        Serial.println("OK: " + tt);
    } else {
        Serial.println("FALSE: " + tt);
    }

    airqual680.registerHomeAssistant("Labor3-BME680", "Breadboard3");
    airqual280.registerHomeAssistant("Labor3-BME280", "Breadboard3");
    precTemp.registerHomeAssistant("Labor3-MCP9808", "Breadboard3");
    dht.registerHomeAssistant("Labor3-DHT22", "Breadboard3");

    sched.subscribe(tID, "i2c/doctor",
                    runDoctor);  // publish to <hostname>/i2c/doctor  to get enumerations of
                                 // i2c devices to MQTT omu/<hostname>/i2c/doctor/#

    Serial.println("[-------------------------------------");
    String val = ustd::muReadVal("murkel/sub1/sub2/sub3/statse", String("wrong"));
    Serial.println(val);
    bool ret = ustd::muWriteVal("murkel/sub1/sub2/sub3/statse", (String)"dong");
    if (ret)
        Serial.println("Seems ok");
    else
        Serial.println("Failed write");
    ret = ustd::muWriteVal("murkel/a/b/c/d", (String)"lala");
    if (ret)
        Serial.println("Seems ok");
    else
        Serial.println("Failed write");
    ret = ustd::muWriteVal("murkel/k", (String)"lulu");
    if (ret)
        Serial.println("Seems ok");
    else
        Serial.println("Failed write");
    ret = ustd::muWriteVal("makka/z", (String)"tong");
    if (ret)
        Serial.println("Seems ok");
    else
        Serial.println("Failed write");
    val = ustd::muReadVal("murkel/sub1/sub2/sub3/statse", (String)"wrong");
    Serial.println(val);
    val = ustd::muReadVal("murkel/a/b/c/d", (String)"wrong");
    Serial.println(val);
    val = ustd::muReadVal("murkel/k", (String)"wrong");
    Serial.println(val);
    val = ustd::muReadVal("murkel/sub1/sub2/sub3/statse", (String)"wrong");
    Serial.println(val);
    val = ustd::muReadVal("makka/z", (String)"wrong");
    Serial.println(val);
    Serial.println("]------------------------------------");
    String msg="asdf";
    ustd::muWriteVal( msg + "/blah/blubb/a", (String)"VaffanCulo" );
    ustd::muWriteVal( msg + "/blah/blubb/b", (long)42 );
    ustd::muWriteVal( msg + "/blah/blubb/c", (double)3.14159265359 );
    ustd::muWriteVal( msg + "/blah/blubb/d", (bool)true );    

    String retS = ustd::muReadVal( msg + "/blah/blubb/a", (String)"Def" );
    long retL = ustd::muReadVal( msg + "/blah/blubb/b", (long)99 );
    double retD = ustd::muReadVal( msg + "/blah/blubb/c", (double)99.99 );
    bool retB = ustd::muReadVal( msg + "/blah/blubb/d", (bool)false );    

    Serial.println(retS);
    Serial.println(retL);
    Serial.println(retD);
    Serial.println(retB);
    Serial.println("{{------------------------------------}}");
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}

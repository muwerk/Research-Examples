#define USE_SERIAL_DBG 1

#include "platform.h"
#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "led.h"
#include "switch.h"
#include "i2c_pwm.h"

void appLoop();

ustd::Scheduler sched(10,16,32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;
ustd::Led ledg("myLed1",D5,false);
ustd::Led ledr("myLed2",D6,false);
ustd::Switch toggleswitch("mySwitch",D7, ustd::Switch::Mode::Default, false);
ustd::I2CPWM servo("myServo",ustd::I2CPWM::Mode::SERVO);

int flipflop=0;
bool active=false;
void switch_messages(String topic, String msg, String originator) {
    Serial.println("Switch received: "+topic+"|"+msg);
    if (topic == "mySwitch/switch/state") {
        if (msg=="on") {
            if (!flipflop) {
                ledg.set(true);
                ledr.set(false);
                flipflop=1;
                active=true;
            } else {
                ledr.set(true);
                ledg.set(false);
                active=false;
                flipflop=0;
                servo.setState(0,false);
            }
            //sched.publish("myLed/light/set","on");
        } else {
            //sched.publish("myLed/light/set","off");
        }
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
    int tID = sched.add(appLoop, "main", 160000);
    ledr.begin(&sched);
    ledg.begin(&sched);
    toggleswitch.begin(&sched);
    servo.begin(&sched);

    //led.setMode(led.Mode::PULSE,1000);
    sched.subscribe(tID, "mySwitch/switch/state", switch_messages);
}

double count=0.0;
void appLoop() {
    if (active) {
        if (count<0.0)
            servo.setUnitLevel(0,0.0);
        else
            servo.setUnitLevel(0,count);
        count+=0.2;
        if (count>1.0) count=-0.60;
    }
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
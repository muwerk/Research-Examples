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
ustd::Led led("myLed",D5,false);
ustd::Switch toggleswitch("mySwitch",D6, false);
ustd::I2CPWM servo("myServo",ustd::I2CPWM::Mode::SERVO);

void switch_messages(String topic, String msg, String originator) {
    Serial.println("Switch received: "+topic+"|"+msg);
    if (topic == "mySwitch/switch/state") {
        if (msg=="on") {
            led.set(true);
            servo.setUnitLevel(0,0.0);
            //sched.publish("myLed/led/set","on");
        } else {
            led.set(false);
            servo.setUnitLevel(0,1.0);
            //sched.publish("myLed/led/set","off");
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
    int tID = sched.add(appLoop, "main", 1000000);
    led.begin(&sched);
    toggleswitch.begin(&sched);
    servo.begin(&sched);

    //led.setMode(led.Mode::PULSE,1000);
    sched.subscribe(tID, "mySwitch/switch/state", switch_messages);
}

void appLoop() {
    //if (led.state) led.set(false);
    //else led.set(true);
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
#include "platform.h"
#include "scheduler.h"

#include "net.h"
#include "../../../munet/mqtt.h"
#include "ota.h"

#include "led.h"
#include "switch.h"
#include "digital_out.h"
#include "power_bl0397.h"

void appLoop();

ustd::Scheduler sched(10,16,32);
ustd::Net net(13); // use red led for network connection phase
ustd::Mqtt mqtt;
ustd::Ota ota;

ustd::Led led1("Led1",1,false); // blue
ustd::Led led2("Led2",13,false); // red
ustd::Switch toggleswitch("mySwitch",3, ustd::Switch::Mode::Flipflop, false);
ustd::DigitalOut relay("Relay",14,true);
ustd::PowerBl0937 power("Power",4,5,12,0,1);

void switch_messages(String topic, String msg, String originator) {
    if (topic == "mySwitch/switch/state") {
        if (msg=="on") {
            led1.set(true);
            relay.set(true);
        } else if (msg=="off") {
            led1.set(false);
            relay.set(false);
        }
    }
}

void setup() {
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
    int tID = sched.add(appLoop, "main", 1000000);
    led1.begin(&sched);
    led2.begin(&sched);
    toggleswitch.begin(&sched);
    relay.begin(&sched);
    power.begin(&sched);
    power.setUserCalibrationFactors(1.0,1.0,1.0); // calib. factors for power (W), voltage (V) and current (A)

    String friendlyName;
    if (!ustd::readFriendlyName(friendlyName)) friendlyName="Gosund SP1";
    toggleswitch.registerHomeAssistant(friendlyName, friendlyName);
    power.registerHomeAssistant(friendlyName+" power", friendlyName);

    sched.subscribe(tID, "mySwitch/switch/state", switch_messages);
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
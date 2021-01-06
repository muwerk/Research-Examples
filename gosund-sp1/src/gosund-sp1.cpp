#include "platform.h"
#include "scheduler.h"

#include "net.h"
#include "mqtt.h"
#include "ota.h"
#include "jsonfiles.h"

#include "led.h"
#include "switch.h"
#include "digital_out.h"
#include "power_bl0397.h"

void appLoop();

ustd::Scheduler sched(10, 16, 32);
ustd::Net net(13);  // use red led for network connection phase
ustd::Mqtt mqtt;
ustd::Ota ota;

ustd::Led led1("Led1", 1, false);   // blue
ustd::Led led2("Led2", 13, false);  // red
ustd::Switch toggleswitch("mySwitch", 3, ustd::Switch::Mode::Flipflop, false);
ustd::DigitalOut relay("Relay", 14, true);
ustd::PowerBl0937 power("Power", 4, 5, 12, 0, 1);

void switch_messages(String topic, String msg, String originator) {
    if (topic == "mySwitch/switch/state") {
        if (msg == "on") {
            led1.set(true);
            relay.set(true);
            ustd::muWriteVal("gosund/last_state", true);
        } else if (msg == "off") {
            led1.set(false);
            relay.set(false);
            ustd::muWriteVal("gosund/last_state", false);
        }
    }
}

void setup() {
    net.begin(&sched, false);  // false: Never reboot on net failure
    mqtt.begin(&sched);
    ota.begin(&sched);
    int tID = sched.add(appLoop, "main", 1000000);
    led1.begin(&sched);
    led2.begin(&sched);
    toggleswitch.begin(&sched);
    String swMode = ustd::muReadVal("gosund/state_on_start", (String) "last");
    bool lastState = ustd::muReadVal("gosund/last_state", (bool)false);
    if (swMode == "on") {
        toggleswitch.setLogicalState(true);
    } else if (swMode == "off") {
        toggleswitch.setLogicalState(false);
    } else if (swMode == "last") {
        toggleswitch.setLogicalState(lastState);
    }

    relay.begin(&sched);
    power.begin(&sched);

    double VC = ustd::muReadVal("gosund/volt_calibration", (double)1.0);
    double AC = ustd::muReadVal("gosund/amp_calibration", (double)1.0);
    double WC = ustd::muReadVal("gosund/power_calibration", (double)1.0);
    // calib. factors for power (W), voltage (V) and current (A)
    power.setUserCalibrationFactors(WC, VC, AC);

    String friendlyName;
    if (!ustd::readFriendlyName(friendlyName))  // XXX: API update at some time
        friendlyName = "Gosund SP1";
    String icon = ustd::muReadVal("gosund/homeassistant_icon", (String) "mdi:power-socket-de");
    toggleswitch.registerHomeAssistant(friendlyName, friendlyName, icon);
    power.registerHomeAssistant(friendlyName + " power", friendlyName);

    sched.subscribe(tID, "mySwitch/switch/state", switch_messages);
}

void appLoop() {
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
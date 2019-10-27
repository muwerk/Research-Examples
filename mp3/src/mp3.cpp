//#define USE_SERIAL_DBG 1
#include "time.h"

#include "platform.h"
#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "../../../mupplets/shift_reg_74595.h"
#include "../../../mupplets/mp3.h"

void appLoop();

ustd::Scheduler sched(10,16,32);
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;

ustd::ShiftReg shifter("shifter",D5,D6,D7);
ustd::Mp3Player mp3("mp3", &Serial, ustd::Mp3Player::MP3_PLAYER_TYPE::OPENSMART);

void setup() {
    Serial.begin(9600);
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);

    shifter.begin(&sched);
    shifter.set(0x09);

    mp3.begin(&sched);
    mp3.repeatLoopFolderTrack(1,2);
}

bool doneBim=false;
void appLoop() {
    if (!(time(nullptr) % 60)) {\
        if (!doneBim) {
            mp3.interleaveFolderTrack(1,3);
            doneBim=true;
        }
    } else {
        doneBim=false;
    }
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
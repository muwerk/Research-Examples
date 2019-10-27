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
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);

    shifter.begin(&sched);
    shifter.set(0x09);

    /* int tID = */ sched.add(appLoop, "main", 50000);

    delay(50);
    mp3.begin(&sched);
    delay(50);
    mp3.setVolume(10);
    delay(50);
    //mp3.play();
    delay(50);
    mp3.playFolderTrack(1,2);
}

bool doneBim=false;
time_t rtime=0;
void appLoop() {

    if (time(nullptr)==rtime) {
        rtime=0;
        mp3.stopInterleave();
    }
    if (!(time(nullptr) % 60)) {\
        if (!doneBim) {
            mp3.interleaveFolderTrack(1,3);
            doneBim=true;
            rtime=time(nullptr)+3;
        }
    } else {
        doneBim=false;
    }
}

// Never add code to this loop, use appLoop() instead.
void loop() {
    sched.loop();
}
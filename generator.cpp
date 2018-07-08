#include "generator.h"
#include <Arduino.h>
//generator warmup 5 minutes (300 seconds)
#define GEN_SAFE 3

//generator guard 10 minutes (600 seconds)
#define GEN_GUARD 6

//generator cooldown 5 minutes (300 seconds)
#define GEN_COOL 3

#define STATE_WARMUP 0
#define STATE_GUARD 1
#define STATE_COOLDOWN 2
#define STATE_HALTED 3


void Generator::setup() {
    cycleTimer.setS(GEN_SAFE,0);
    cycleTimer.setS(GEN_GUARD,1);
    cycleTimer.setS(GEN_COOL,2);
    cycleTimer.loop(false);
}

bool Generator::in_halt() {
    if (cycleTimer.state() == STATE_HALTED) 
        return true;
    else
        return false;
}

void Generator::update(bool startS, bool started) {
    /*if(cycleTimer.event()) {
        Serial.print("Generator event: ");
        Serial.print(cycleTimer.state());
        Serial.print("\n");
    }*/
    lineStatus = startS;
    switch(cycleTimer.state()) {
        case STATE_HALTED:
            cycleTimer.reset();
        break;
        case STATE_WARMUP:
            if (started && startS)
                cycleTimer.update();
            else
                cycleTimer.reset();
        break;
        case STATE_GUARD:
            if(started) {
                if (!startS)
                    cycleTimer.update();
                else
                    cycleTimer.clear();
            } else {
                cycleTimer.reset();
            }
        break;
        case STATE_COOLDOWN:
            if(started) {
                if (!startS)
                    cycleTimer.update();
                else
                    cycleTimer.reset();
            } else {
                cycleTimer.reset();
            }
        break;
        default:
            cycleTimer.reset();
        break;            
    }
}

void Generator::reset() {
    cycleTimer.reset();
}

Generator::Generator() {
    setup();
}

bool Generator::connect() {
    if(cycleTimer.state() == STATE_GUARD)
        return true;
    else
        return false;
}

bool Generator::false_warmup() {
    if(cycleTimer.state() == STATE_WARMUP && lineStatus == false)
        return true;
    else
        return false;
}

bool Generator::halt() {
    if (in_halt() || false_warmup())
        return true;
    else
        return false;
}


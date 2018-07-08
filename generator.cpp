#include "generator.h"
//generator warmup 5 minutes (300 seconds)
#define GEN_SAFE 30

//generator cooldown 15 minutes (900 seconds)
#define GEN_COOL 90

#define STATE_HALTED 0
#define STATE_WARMUP 1
#define STATE_COOLDOWN 2
#define STATE_WARM 3


void Generator::setup() {
    state = STATE_HALTED;
    connectS = false;   
    warmup.setS(GEN_SAFE);
    cooldown.setS(GEN_COOL);
}

bool Generator::in_warmup() {
    if (state == STATE_WARMUP) 
        return true;
    else 
        return false;
}

bool Generator::in_cooldown() {
    if (state == STATE_COOLDOWN) 
        return true;
    else
        return false;
}

bool Generator::in_halt() {
    if (state == STATE_HALTED) 
        return true;
    else
        return false;
}

void Generator::update(bool startS, bool started) {
    switch(state) {
        case STATE_HALTED:
            startS ? state = STATE_WARMUP : state = STATE_COOLDOWN;
        break;
        case STATE_WARMUP:
            warmup.event() ? state = STATE_WARM: state = STATE_WARMUP; 
        break;
        case STATE_COOLDOWN:
            cooldown.event() ? state = STATE_HALTED: state = STATE_COOLDOWN; 
        break;
        case STATE_WARM:
            connectS = true;
        break;
        default:
            state = STATE_HALTED;
        break;            
    }
    in_warmup() ? warmup.update() : warmup.reset();
    in_cooldown() ? cooldown.update() : cooldown.reset();
}

void Generator::reset() {
    warmup.reset();
    cooldown.reset();
}

Generator::Generator() {
    setup();
}

bool Generator::connect() {
    return connectS;
}

bool Generator::halt() {
    return in_halt();
}


#include "DrumTimer.h"

void DrumTimer::clear() {
    triggered = false;
    lapTimer.reset();
}

void DrumTimer::reset() {
    status = 0;
    currentSlot = 0;
    lapTimer.setMS(preset[0]);
    clear();   
}

DrumTimer::DrumTimer() {
    preset[0] = 0;  
    maxSlots = 0;
    loopable = true;  
    reset();
}

void DrumTimer::setMS(unsigned long value, unsigned char slot) {
    if (slot < DRUM_SLOTS) {
        preset[slot] = value;
        if(maxSlots < slot)
            maxSlots = slot;
        reset();
    }
}

void DrumTimer::setS(unsigned int s, unsigned char slot) {
    unsigned long aux = s * 1000;
    setMS(aux,slot);
}

void DrumTimer::update() {
    if(lapTimer.event()) {
        if(currentSlot <= maxSlots) {
            currentSlot++;
            status++;
            lapTimer.reset();
        }       
        triggered = true;
        lapTimer.setMS(preset[currentSlot]);
    }
    if (currentSlot > maxSlots && loopable)
        reset();
    lapTimer.update();
}

bool DrumTimer::event() {
    bool aux = triggered;
    triggered = false;
    return aux;
}

unsigned char DrumTimer::state() {
    return status;
}

void DrumTimer::loop(bool enable) {
    loopable = enable;
}

unsigned long DrumTimer::valueMS() {
    return lapTimer.valueMS();
}

unsigned long DrumTimer::valueS() {
    return lapTimer.valueS();
}



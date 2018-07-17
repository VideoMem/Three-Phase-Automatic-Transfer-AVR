/*
*  battery.h
*
*/

#ifndef BATTERY_H
#define BATTERY_H
#define BATTERY_VERSION "0"
#include "Module.h"
#include "Timers.h"
//#include <Arduino.h>

//input pin
#define VBATPIN A0
//sampling interval (s)
#define BATSAMP 60
//minimum charging voltage (volts)
#define MINIMAL_V_LEVEL 12.4

class Battery: public Module {
    public: 
        Battery();
        void update();
        float value();
        bool alarm();
        void logValue();
        void check();
    protected:
        float vBat = 0;
        Timer batTimer;
    private:
        void setup();

        void logAlarm();
};

#endif

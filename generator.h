#ifndef GENERATOR_H
#define GENERATOR_H
#define GENERATOR_VERSION "0"

#include "timers.h"

class Generator {
    public:
        Generator();
        void setup();
        void update(bool start, bool started);
        void reset();
        bool connect(); //safe to connect generator
        bool halt();    //halt generator
    private:
        timer warmup;
        timer cooldown;
        bool in_warmup();
        bool in_cooldown();
        bool in_halt();
        bool startSignal;
        void transferHot();
        void transferWarm();
        bool start;
        bool connectS;
        unsigned char state;
            
};  



#endif


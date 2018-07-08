#ifndef GENERATOR_H
#define GENERATOR_H
#define GENERATOR_VERSION "0"

#include "DrumTimer.h"

class Generator {
    public:
        Generator();
        void setup();
        void update(bool start, bool started);
        void reset();
        bool connect(); //safe to connect generator
        bool halt();    //halt generator
    private:
        DrumTimer cycleTimer;
        bool in_halt(); 
        bool false_warmup();    //halt generator
        bool lineStatus;          
};  



#endif


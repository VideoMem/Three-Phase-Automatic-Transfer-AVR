/*
*  line.h
*/
#include "timers.h"

#ifndef LINE_H
#define LINE_H
#define LINE_VERSION "0"

//line sampling interval (ms)
#define SAMPLE_INTERVAL 22

//generator warmup 5 minutes (300 seconds)
#define GEN_SAFE 30

//if refrigerator compressors stops, waits 3 minutes (180 seconds) before starting it up again 
#define COMP_SAFE 18

//line input pins Phase 0, Phase 1, Generator Phase
#define P0 2
#define P1 9
#define G0 10 

class line {
    public:
        line();
        bool connect();
        bool generator();
        bool genConnect();
        bool fail();
        void update();
        void setup();
    private:
        bool genSafe;
        bool Connect;
        bool Generator;
        timer genTimer;                
        timer sampleTimer;
        timer compressorTimer;
        unsigned char sampleIndex;
        unsigned char sample[9];
        unsigned char status[3];
        const unsigned char phase[9];
        bool checkPhases();
        void phaseUpdate();
};

#endif


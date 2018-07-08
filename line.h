/*
*  line.h
*/
#ifndef LINE_H
#define LINE_H
#define LINE_VERSION "0"

#include "timers.h"
#include "generator.h"

//line input pins Phase 0, Phase 1, Phase 2, Generator Phase
#define P0 9
#define P1 10
#define P2 11
#define G0 2 

#define P0_P 3
#define P1_P 4
#define P2_P 5
#define G0_P 6

//lines (three phase, one generator) 
#define LINES 4
//lines * 2
#define LINES2 8
//lines * 3
#define LINES3 12
//lines * LINES
#define INITP 16 

//line sampling interval (ms)
#define SAMPLE_INTERVAL 30


class line {
    public:
        line();
        bool genStart();
        bool genStarted();
        bool genConnect();
        bool Ok();
        bool fail();
        bool halt();

        void update();
        void setup();

    private:
        Generator generator;
        timer sampleTimer;
        unsigned char sampleIndex;
        unsigned char sample[INITP];
        unsigned char status[LINES];
        unsigned char phase[LINES];
        bool checkPhases();
        void phaseUpdate();
        void generatorMSG();
        void mainsMSG();
        void abnormalMSG();
};

#endif


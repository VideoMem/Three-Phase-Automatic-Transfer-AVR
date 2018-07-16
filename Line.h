/*
*  line.h
*/
#ifndef LINE_H
#define LINE_H
#define LINE_VERSION "0"

#include "Timers.h"
#include "Generator.h"
#include <Arduino.h>

//discrete line input presence pins Phase 0, Phase 1, Phase 2, Generator Phase
#define P0 9
#define P1 10
#define P2 11
#define G0 2 

//discrete power input control pins, 3 Phases, 1 Generator 
#define P0_P 3
#define P1_P 4
#define P2_P 5
#define G0_P 6

//analog phase voltage readout
#define P0_A A1
#define P1_A A2
#define P2_A A3
#define G0_A A4

//lines (three phase, one generator) 
#define LINES 4
//lines * 2
#define LINES2 8
//lines * 3
#define LINES3 12
//lines * LINES
#define INITP 16 

//line presence sampling interval (ms) 30 for 50Hz, 25 for 60Hz, with RC optocoupler inputs
#define SAMPLE_INTERVAL 30

//ṕhase change limit interval (ms) 
#define CHANGE_INTERVAL 50

//voltage threshold %
#define CHANGE_THRESHOLD 10.0

class Line {
    public:
        Line();
        bool genStart();
        bool genStarted();
        bool genConnect();
        bool Ok();
        bool fail();
        bool halt();

        void update();
        void setup();
        unsigned char inputSelect();
    private:
        Generator generator;
        Timer sampleTimer;
        Timer changeTimer;
        float vThr(unsigned char vindex);
        unsigned char phaseSelect();
        unsigned char sampleIndex;
        unsigned char sample[INITP];
        unsigned char status[LINES];
        unsigned char phase[LINES];
        unsigned char currentPhase;
        float voltages[LINES];
        unsigned char analog[LINES];
        float vSample[INITP];
        float voltageRead(unsigned char pin);
        bool checkPhases();
        void phaseUpdate();
        void generatorMSG();
        void mainsMSG();
        void abnormalMSG();
        void outVoltages();
        void samplePhase();
        void changePhase();
};

#endif


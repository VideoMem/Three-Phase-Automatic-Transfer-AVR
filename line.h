/*
*  line.h
*/
#include "timers.h"

#ifndef LINE_H
#define LINE_H
#define LINE_VERSION "0"

//line sampling interval (ms)
#define SAMPLE_INTERVAL 30

//generator warmup 5 minutes (300 seconds)
#define GEN_SAFE 30

//if refrigerator compressors stops, waits 3 minutes (180 seconds) before starting it up again 
#define COMP_SAFE 18

//line input pins Phase 0, Phase 1, Generator Phase
#define P0 2
#define P1 9
#define G0 10 

//0 all disconnected
//1 generator warmup time
//2 generator ready (gen_safe)
//3 compressor wait
//4 line present
//5 abnormal line present
//6 line ready to connect
//7 generator ready to connect
#define S_LDIS 0
#define S_LGEN 1
#define S_GRED 2
#define S_WCMP 3
#define S_LINE 4
#define S_ABNR 5
#define S_RLIN 6
#define S_RGEN 7



class line {
    public:
        line();
        bool present();
        bool ready();
        bool genStart();
        bool genStarted();
        bool Ok();
        bool linConnect();
        bool genConnect();
        bool fail();
        void update();
        void setup();
    private:
        unsigned char state;
        timer genTimer;                
        timer sampleTimer;
        timer compressorTimer;
        unsigned char sampleIndex;
        unsigned char sample[9];
        unsigned char status[3];
        unsigned char phase[3];
        bool checkPhases();
        void phaseUpdate();
        void generatorMSG();
        void mainsMSG();
};

#endif


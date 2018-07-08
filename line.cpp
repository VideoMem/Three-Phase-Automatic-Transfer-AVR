#include <Arduino.h>
#include "line.h"
#define CON 1
#define DIS 0

void line::setup() {
    // some[LINES] = phases[LINES]; many[LINES] = LINES * LINES
    unsigned char some[LINES] = {CON,CON,CON,CON};
    unsigned char many[INITP] = {CON,CON,CON,CON,CON,CON,CON,CON,CON,CON,CON,CON,CON,CON,CON,CON};
    unsigned char phases[LINES] = {P0,P1,P2,G0};
    sampleIndex = 0;
    memcpy(status,some,sizeof(unsigned char) * LINES);
    memcpy(sample,many,sizeof(unsigned char) * INITP);
    memcpy(phase,phases,sizeof(unsigned char) * LINES);
    sampleTimer.setMS(SAMPLE_INTERVAL);
    sampleTimer.reset();
}

//generator start signal
bool line::genStart() {
    if(status[3] == DIS && !Ok())
        return true;
    else
        return false;
}

bool line::genStarted() {
    return !genStart();
}

bool line::genConnect() {
    return generator.connect(); 
}

// line present (Lp)
bool line::Ok() {
    if(status[0] == DIS || status[1] == DIS || status[2] == DIS)
        return false;
    else
        return true;
}

bool line::halt() {
   return generator.halt();
}

void line::generatorMSG() {
    Serial.print("No power present at the mains, starting generator ...\n");
}

void line::mainsMSG() {
    Serial.print("Power present, reconnecting to the mains ...\n");          
}

void line::abnormalMSG() {
    Serial.print("Abnormal power input!\n");
}

line::line() {
    setup();    
}


//phase sample & status update
bool line::checkPhases() {
    unsigned char phaseChange[LINES]  = {DIS,DIS,DIS,DIS};
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned char k = 0;
    unsigned char l = 0;
    unsigned char m = 0;
    unsigned char limit = sampleIndex + LINES;
    bool changed = false;

    if (sampleIndex < INITP) {
        for(i=sampleIndex; i < limit; i++) {  
            l = i - sampleIndex;
            sample[i] = digitalRead(phase[l]);
        }
        sampleIndex += LINES;
    } else {
        sampleIndex = 0;
        for(i=0; i < LINES; i++) {
            j = i + LINES;
            k = i + LINES2;
            m = i + LINES3;
            phaseChange[i] = sample[i] | sample[j] | sample[k] | sample[m];
            if(status[i] != phaseChange[i])
                 changed = true;
            status[i] = phaseChange[i];
        }
    }

    return changed;
}

//abnormal line condition
bool line::fail() {

    if(status[2] == DIS && status[0] == CON && status[1] == DIS) {
        abnormalMSG();
        return true;
    }

    if(status[2] == DIS && status[0] == DIS && status[1] == CON) {
        abnormalMSG();
        return true;
    }

    if(status[2] == DIS && status[0] == CON && status[1] == CON) {
        abnormalMSG();
        return true;
    }

    if(status[2] == CON && status[0] == DIS && status[1] == DIS) {
        abnormalMSG();
        return true;
    }

    if(status[2] == CON && status[0] == CON && status[1] == DIS) {
        abnormalMSG();
        return true;
    }

    if(status[2] == CON && status[0] == DIS && status[1] == CON) {
        abnormalMSG();
        return true;
    }

    return false;
}

//phase update log
void line::phaseUpdate() {
    unsigned char i,j = 0;
    bool failure = fail();

    for(i=0; i < LINES; i++) {
        if (status[i] == DIS) {
            j = i + 1;
            Serial.print("Phase ");
            Serial.print(j);
            Serial.print(": disconnected\n");       
        }
    }
    
    if(failure) {
        return;
    }

    if(!Ok()) {
        if(genStart()) {
            generatorMSG();
        }
    } else {
        mainsMSG();
    }        
    
}

//main control loop
void line::update() {
    if(sampleTimer.event()) {
        sampleTimer.reset();
        if(checkPhases()) 
            phaseUpdate();
    } else {
        sampleTimer.update();    
    }
    generator.update(!Ok(),genStarted());
}

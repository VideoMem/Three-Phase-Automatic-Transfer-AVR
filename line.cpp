#include <Arduino.h>
#include "line.h"
#define CON 1
#define DIS 0

void line::setup() {
    unsigned char threezeros[3] = {CON,CON,CON};
    unsigned char ninezeros[9] = {CON,CON,CON,CON,CON,CON,CON,CON,CON};
    unsigned char phases[9] = {P0,P1,G0,P0,P1,G0,P0,P1,G0};
    sampleIndex = 0;
    memcpy(status,threezeros,sizeof(unsigned char) * 3);
    memcpy(sample,ninezeros,sizeof(unsigned char) * 9);
    memcpy(phase,phases,sizeof(unsigned char) * 9);
    sampleTimer.setMS(SAMPLE_INTERVAL);
    genTimer.setS(GEN_SAFE);
    compressorTimer.setS(COMP_SAFE);
    genSafe = false;
    Connect = false;
    Generator = false;
}

line::line() {
    setup();    
}

bool line::generator() {
    return Generator;
}

bool line::genConnect() {
    return genSafe;
}

bool line::connect() {
    return Connect;
}

bool line::checkPhases() {
    unsigned char phaseChange[3]  = {DIS,DIS,DIS};
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned char k = 0;
    unsigned char limit = sampleIndex + 3;
    bool changed = false;

    if (sampleIndex < 9) {
        for(i=sampleIndex; i < limit; i++)  
            sample[i] = digitalRead(phase[i]);
        sampleIndex += 3;
    } else {
        sampleIndex = 0;
        for(i=0; i < 3; i++) {
            j = i + 3;
            k = i + 6;
            phaseChange[i] = sample[i] | sample[j] | sample[k];
        }
        for(i=0; i < 3; i++) {
            if(status[i] != phaseChange[i])
                 changed = true;
            status[i] = phaseChange[i];
        }
    }

    return changed;
}

bool line::fail() {

    if(status[0] == DIS && status[1] == CON) {
        Serial.print("Abnormal power input!\n");
        return true;
    }

    if(status[0] == CON && status[1] == DIS) {
        Serial.print("Abnormal power input!\n");
        return true;
    }

    return false;
}

void line::phaseUpdate() {
    unsigned char i,j = 0;
    bool failure = fail();

    for(i=0; i < 3; i++) {
        if (status[i] == DIS) {
            j = i + 1;
            Serial.print("Phase ");
            Serial.print(j);
            Serial.print(": disconnected\n");       
        }
    }
    
    if(failure) {
        Connect = false;
        return;
    }

    if(status[0] == DIS && status[1] == DIS) {
        Serial.print("No power present in the mains, starting generator ...\n");
        if(!Generator) {        
            Generator = true;
            genTimer.reset();
        }
    } else {
        Serial.print("Power present, reconnecting to the mains ...\n");
        if(Generator) {
            Generator = false;
            compressorTimer.reset();
        }            
    }        
    

}

void line::update() {
    if(sampleTimer.event()) {
        sampleTimer.reset();
        if(checkPhases()) 
            phaseUpdate();
    } else {
        sampleTimer.update();    
    }

    if(Generator && !genSafe) {
        if(genTimer.event()) {
            genSafe = true;
            Serial.print("Generator is ready!\n");
        } else {
            genTimer.update();
        }
        compressorTimer.reset();
    } else {
        genTimer.reset();
        genSafe = false;
    }

    if(Generator && genSafe && !Connect) {
        if(compressorTimer.event()) {
            Connect = true;
            Serial.print("Connecting generator load\n");
        } else {
            compressorTimer.update();
        }
    }

}

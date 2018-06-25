#include <Arduino.h>
#include "line.h"
#define CON 1
#define DIS 0

void line::setup() {
    unsigned char threezeros[3] = {CON,CON,CON};
    unsigned char ninezeros[9] = {CON,CON,CON,CON,CON,CON,CON,CON,CON};
    unsigned char phases[3] = {P0,P1,G0};
    sampleIndex = 0;
    memcpy(status,threezeros,sizeof(unsigned char) * 3);
    memcpy(sample,ninezeros,sizeof(unsigned char) * 9);
    memcpy(phase,phases,sizeof(unsigned char) * 3);
    sampleTimer.setMS(SAMPLE_INTERVAL);
    genTimer.setS(GEN_SAFE);
    compressorTimer.setS(COMP_SAFE);
    state = S_LDIS;
    sampleTimer.reset();
    compressorTimer.reset();
    genTimer.reset(); 
}

//generator start signal
bool line::genStart() {
    if(status[2] == DIS)
        return true;
    else
        return false;
}

bool line::genStarted() {
    if(status[2] == CON)
        return true;
    else
        return false;
}

bool line::Ok() {
    if(status[0] == DIS || status[1] == DIS)
        return false;
    else
        return true;
}


//connect generator signal
bool line::genConnect() {
    if(genStarted() && state == S_GRED)
        return true;
    else
        return false;
}

void line::generatorMSG() {
    Serial.print("No power present at the mains, starting generator ...\n");
}

void line::mainsMSG() {
    Serial.print("Power present, reconnecting to the mains ...\n");          
}

//connect line signal
bool line::linConnect() {
    if(Ok() && state == S_LINE)
        return true;
    else
        return false;
}

//power is ready
bool line::ready() {
    if(state == S_RLIN || state == S_RGEN) 
        return true;
    else
        return false;
}

//power present
bool line::present() {
    if(state == S_GRED || state == S_LINE)
        return true;
    else
        return false;
}

line::line() {
    setup();    
    if(Ok()) 
        state = S_LINE;
    else
        state = S_LGEN;
}

//phase sample & status update
bool line::checkPhases() {
    unsigned char phaseChange[3]  = {DIS,DIS,DIS};
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned char k = 0;
    unsigned char l = 0;
    unsigned char limit = sampleIndex + 3;
    bool changed = false;

    if (sampleIndex < 9) {
        for(i=sampleIndex; i < limit; i++) {  
            l = i - sampleIndex;
            sample[i] = digitalRead(phase[l]);
        }
        sampleIndex += 3;
    } else {
        sampleIndex = 0;
        for(i=0; i < 3; i++) {
            j = i + 3;
            k = i + 6;
            phaseChange[i] = sample[i] | sample[j] | sample[k];
            if(status[i] != phaseChange[i])
                 changed = true;
            status[i] = phaseChange[i];
        }
    }

    return changed;
}

//abnormal line condition
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

//phase update log
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
        state = S_ABNR;
        return;
    }

    if(!Ok()) {
        if(genStart()) {
            generatorMSG();
            state = S_LGEN;
        }
    } else {
        mainsMSG();
        if(state == S_RGEN)
            state = S_RLIN;
        else
            state = S_LINE;
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

    if(present() && !ready()) {
        if(compressorTimer.event()) {
            Serial.print("Power ready to apply\n");
            if(genConnect()) {
                Serial.print("Connecting generator load\n");
                state = S_RGEN;
            } else {
                Serial.print("Connecting line load\n");
                state = S_RLIN;
            }
        } else {
            compressorTimer.update();
        }
    } else {
        compressorTimer.reset();
    }
   
    if(genConnect() && !ready()) {
        if(genTimer.event()) {
            Serial.print("Generator warmed up ...\n");
            state = S_GRED;
        } else {
            genTimer.update();
        }          
    } else {
         genTimer.reset();
    }       
}

#include <Arduino.h>
#include "Line.h"
#define VDEF 0.0
#define CON 1
#define DIS 0


void Line::setup() {
    // some[LINES] = phases[LINES]; many[LINES] = LINES * LINES
    // initialize sample memory
    unsigned char some[LINES] = {CON,CON,CON,CON};
    unsigned char many[INITP] = {CON,CON,CON,CON,CON,CON,CON,CON,CON,CON,CON,CON,CON,CON,CON,CON};
    float someF[LINES] = {VDEF,VDEF,VDEF,VDEF};
    float manyF[INITP] = {VDEF,VDEF,VDEF,VDEF,VDEF,VDEF,VDEF,VDEF,VDEF,VDEF,VDEF,VDEF,VDEF,VDEF,VDEF,VDEF};
    unsigned char phases[LINES] = {P0,P1,P2,G0};
    unsigned char analogs[LINES] = {P0_A,P1_A,P2_A,G0_A};
    sampleIndex = 0;
    memcpy(status,some,sizeof(unsigned char) * LINES);
    memcpy(sample,many,sizeof(unsigned char) * INITP);
    memcpy(phase,phases,sizeof(unsigned char) * LINES);
    memcpy(analog,analogs,sizeof(unsigned char) * LINES);
    memcpy(voltages,someF,sizeof(float) * LINES);
    memcpy(vSample,manyF,sizeof(float) * INITP);

    sampleTimer.setMS(SAMPLE_INTERVAL);
    sampleTimer.reset();
    changeTimer.setMS(CHANGE_INTERVAL);
    changeTimer.reset();
}

//generator start signal
bool Line::genStart() {
    if(status[3] == DIS && !Ok())
        return true;
    else
        return false;
}

bool Line::genStarted() {
    return !genStart();
}

bool Line::genConnect() {
    return generator.connect(); 
}

float Line::vThr(unsigned char vindex) {
    float aux = voltages[vindex] * CHANGE_THRESHOLD / 100.0;
    float log = voltages[vindex] - aux;
    return log;
}

unsigned char Line::phaseSelect() {
    if(vThr(0) > voltages[1]) {
        if (vThr(2) > voltages[0])
            return 2;
        else
            return 0;
    } else {
        if (vThr(2) > voltages[1])
            return 2;
        else
            return 1;
    }
}


// line present (Lp)
bool Line::Ok() {
    if(status[0] == DIS || status[1] == DIS || status[2] == DIS)
        return false;
    else
        return true;
}

bool Line::halt() {
   return generator.halt();
}

void Line::generatorMSG() {
    Serial.print("No power present at the mains, starting generator ...\n");
}

void Line::mainsMSG() {
    Serial.print("Power present, reconnecting to the mains ...\n");          
}

void Line::abnormalMSG() {
    Serial.print("Abnormal power input!\n");
}

Line::Line() {
    setup();    
}

float Line::voltageRead(unsigned char pin) {
    int aux = analogRead(pin);
    //value for three phase 440v 
    float vRead = 254.03 * (float) aux / 1023.0;
    return vRead;
}

void Line::outVoltages() {
    unsigned char i = 0;
 
    for(i=0; i < LINES; i++) {
        Serial.print("Voltage ");
        switch(i) {
            case 0:
                Serial.print("phase R: ");
            break;
            case 1:
                Serial.print("phase S: ");
            break;
            case 2:
                Serial.print("phase T: ");
            break;
            case 3:
                Serial.print("phase G: ");
            break;
        }
        Serial.print(voltages[i]);
        Serial.print("V\n");
    } 
}

//phase sample & status update
bool Line::checkPhases() {
    unsigned char phaseChange[LINES]  = {DIS,DIS,DIS,DIS};
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned char k = 0;
    unsigned char l = 0;
    unsigned char m = 0;
    unsigned char limit = sampleIndex + LINES;
    bool changed = false;
    float sum = 0;

    if (sampleIndex < INITP) {
        for(i=sampleIndex; i < limit; i++) {  
            l = i - sampleIndex;
            sample[i] = digitalRead(phase[l]);
            vSample[i] = voltageRead(analog[l]);
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
            sum = vSample[i] + vSample[j] + vSample[k] + vSample[m];
            voltages[i] = sum / 4.0;
        }
    }

    return changed;
}

//abnormal line condition
bool Line::fail() {

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
void Line::phaseUpdate() {
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
 
    outVoltages();       
}

unsigned char Line::inputSelect() {
    return currentPhase;
}

void Line::changePhase() {
    unsigned char oldPhase = currentPhase;
    if(changeTimer.event()) {
        changeTimer.reset();
        currentPhase = phaseSelect();
        //delta Phase
        if (oldPhase != currentPhase) 
            outVoltages();
    } else {
        changeTimer.update();
    }
}

void Line::samplePhase() {
   if(sampleTimer.event()) {
        sampleTimer.reset();
        if(checkPhases()) 
            phaseUpdate();
    } else {
        sampleTimer.update();    
    }
}

//main control loop
void Line::update() {
    samplePhase();
    changePhase();
    generator.update(!Ok(),genStarted());
}

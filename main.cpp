#include <Arduino.h>
#include "Timers.h"
#include "Toggle.h"
#include "Line.h"
#include "Battery.h"

#define BOOT_DELAY 6000
#define NO_ERROR 500
#define ON_ERROR 50
#define MINUTE 60
#define BAUDRATE 9600

//safe reconnect delay for refrigerator compressors 5 minutes (300 seconds)
#define COMPRESSOR_DELAY 30

unsigned char ledPin = 13;                             // LED connected to digital pin 13
unsigned char phase[LINES] = {P0,P1,P2,G0};            // phase input pins, 0, 1, 2, generator (input)
unsigned char conn[LINES] = {P0_P,P1_P,P2_P,G0_P};     // phase selector pins, 0, 1, 2, generator (output)
unsigned char generator = 12;                          // generator start signal output pin
unsigned char generatorHalt = 7;                       // generator halt  signal output pin

unsigned char analogs[LINES] = {P0_A,P1_A,P2_A,G0_A};

unsigned char error = 0;
bool powerEnable = false;

Timer blinkTimer;
Timer compTimer;

Battery battery;
Line lineControl;
Toggle blinker;

void initPins(unsigned char* pins, unsigned char mode, unsigned char num) {
    unsigned char i = 0;
    for(i=0; i < num; i++)
        pinMode(pins[i],mode);
}

void setup() {
    Serial.begin(BAUDRATE); 
    Serial.print("Initializing ... \n");
    pinMode(ledPin, OUTPUT);

    initPins(phase, INPUT, LINES);
    initPins(analogs, INPUT, LINES);
    initPins(conn, OUTPUT, LINES);
    pinMode(generator, OUTPUT);
    pinMode(generatorHalt, OUTPUT);
	blinkTimer.setMS(NO_ERROR);
	compTimer.setS(COMPRESSOR_DELAY);
    battery.check();

    Serial.flush();
    delay(BOOT_DELAY);
    Serial.print("Done\n");
}

void lineSelect() {
    unsigned char ph = lineControl.inputSelect();
    unsigned char i = 0;
    if (!lineControl.genConnect()) {
        digitalWrite(G0_P,LOW);
        for(i=0; i < 3; i++) 
            if(ph == i)
                powerEnable ? digitalWrite(conn[i],HIGH) : digitalWrite(conn[i],LOW);
            else
                digitalWrite(conn[i],LOW);
   } else {
        for(i=0; i < 3; i++) 
            digitalWrite(conn[i],LOW);
        powerEnable ? digitalWrite(G0_P,HIGH) : digitalWrite(G0_P,LOW);
   }
}


void outputPatch() {
    lineControl.genStart() ? digitalWrite(generator,HIGH): digitalWrite(generator,LOW);
    lineControl.halt() ? digitalWrite(generatorHalt,HIGH): digitalWrite(generatorHalt,LOW);
    lineSelect();
}

void ledDrive() {
    if(blinker.value() == true)
        digitalWrite(ledPin, HIGH);   // set the LED on
    else
        digitalWrite(ledPin, LOW);    // set the LED off

    if(blinkTimer.event()) {
        blinker.change();
        blinkTimer.reset();
    }
    
    blinkTimer.update();
}


void errorDrive() {
    if(error == 1) {
        blinkTimer.setMS(ON_ERROR);         
    } else {
        blinkTimer.setMS(NO_ERROR);         
    }
}

void powerDrive() {
    if(compTimer.event())
        powerEnable = true;
    if(lineControl.Ok() || lineControl.genConnect()) {
        compTimer.update();
    } else {
        powerEnable = false;
        compTimer.reset();
    }
}

void loop() {
    ledDrive();
    battery.update();
    errorDrive();
    powerDrive();
    lineControl.update();
    outputPatch();
}

int main(void) {
    init();
    setup();
    for (;;) {
       loop();
    }
}


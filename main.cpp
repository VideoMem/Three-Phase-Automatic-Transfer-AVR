#include <Arduino.h>
#include "timers.h"
#include "toggle.h"
#include "line.h"
#include "DrumTimer.h"

#define BOOT_DELAY 6000
#define NO_ERROR 500
#define ON_ERROR 50
#define MINUTE 60
#define BAUDRATE 9600

unsigned char ledPin = 13;                             // LED connected to digital pin 13
unsigned char phase[LINES] = {P0,P1,P2,G0};            // phase input pins, 0, 1, 2, generator (input)
unsigned char conn[LINES] = {P0_P,P1_P,P2_P,G0_P};     // phase selector pins, 0, 1, 2, generator (output)
unsigned char generator = 12;                          // generator start signal output pin
unsigned char generatorHalt = 7;                       // generator halt  signal output pin
unsigned char vBatPin = A0;
float vBat = 0;
unsigned char error = 0;

timer blinkTimer;
timer batTimer;
line lineControl;
toggle blinker;

void readVBat() {
    int aux = analogRead(vBatPin);
    vBat = 16.0 * (float) aux / 1024.0;
    if(vBat <= 12.4) {
        error = 1;
        Serial.print("vBat LOW!: ");
        Serial.print(vBat);                
        Serial.print("\n");
    } else {    
        error = 0;
    }
}

void initPins(unsigned char* pins, unsigned char mode, unsigned char num) {
    unsigned char i = 0;
    for(i=0; i < num; i++)
        pinMode(pins[i],mode);
}

void setup() {
    Serial.begin(BAUDRATE); 
    Serial.print("Initializing ... ");
    pinMode(ledPin, OUTPUT);

    initPins(phase, INPUT, LINES);
    initPins(conn, OUTPUT, LINES);
    pinMode(generator, OUTPUT);
    pinMode(generatorHalt, OUTPUT);
	blinkTimer.setMS(NO_ERROR);
	batTimer.setS(MINUTE);
    readVBat();
    Serial.print("Ok\n");
    Serial.print("vBat: ");
    Serial.print(vBat);                
    Serial.print("\n");  
    Serial.print("Boot Delay ... ");
    Serial.flush();
    delay(BOOT_DELAY);
    Serial.print("Done\n");
}

void outputPatch() {
    lineControl.genStart() ? digitalWrite(generator,HIGH): digitalWrite(generator,LOW);
    lineControl.halt() ? digitalWrite(generatorHalt,HIGH): digitalWrite(generatorHalt,LOW);
    lineControl.genConnect() ? digitalWrite(G0_P,HIGH): digitalWrite(G0_P,LOW);
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

void batDrive() {
    if(batTimer.event()) {
        readVBat();              
        batTimer.reset();
    }
    batTimer.update();
}

void errorDrive() {
    if(error == 1) {
        blinkTimer.setMS(ON_ERROR);         
    } else {
        blinkTimer.setMS(NO_ERROR);         
    }
}

void loop() {
    ledDrive();
    batDrive();
    errorDrive();
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


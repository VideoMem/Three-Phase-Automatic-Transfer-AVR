#include <Arduino.h>
#include "timers.h"
#include "toggle.h"
#include "line.h"

#define BAUDRATE 9600
#define BOOT_DELAY 6000
#define COMP_DELAY 180
#define NO_ERROR 500
#define ON_ERROR 50
#define MINUTE 60

unsigned char ledPin = 13;              // LED connected to digital pin 13
unsigned char phase[3] = {P0,P1,G0};    // phase input pins, 0, 1, 2
unsigned char conn[3] = {14,11,16};     // phase selector pins, 0, 1, 2
unsigned char generator = 12;           // generator start signal output pin
unsigned char phaseStatus[3] = {1,1,1};
unsigned char vBatPin = A0;
float vBat = 0;
unsigned char compDelay = 0;
unsigned char error = 0;
timer blinkTimer;
timer batTimer;
timer compTimer;
timer printTimer;
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

    initPins(phase, INPUT, 3);
    //initPins(conn, OUTPUT, 3);
    pinMode(generator, OUTPUT);
   
	blinkTimer.setMS(NO_ERROR);
	batTimer.setS(MINUTE);
	compTimer.setS(COMP_DELAY);
    printTimer.setMS(500);
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
    if(error == 1 || compDelay == 1) {
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
//    phaseUpdate();
}

int main(void) {
    init();
    setup();
    for (;;) {
       loop();
    }
}


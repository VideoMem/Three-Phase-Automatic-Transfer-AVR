#include "timers.h"
#include <Arduino.h>

timer::timer() {
  elapsed = 0;
  enabled = 0;
  preset = 0;
  last_read = 0;
  trigged = 0;
}

void timer::check() {
 if (preset == 0)
    enabled = 0; 
 else
    if (elapsed > preset) {
      trigged = 1;
      elapsed = 0;
      enabled = 0;
    } 
}

void timer::update() {
   unsigned long read = millis(); 

   if (enabled == 1) {   
     // read overflow
     if (read < last_read)
       elapsed += read;
     else {
       elapsed += read - last_read;
     }     
   }
   
   last_read = read;
   check();
}

unsigned long timer::valueMS() {
  return elapsed; 
}

unsigned long timer::valueS() {
  return elapsed / 1000; 
}

int timer::event() {
  if (trigged == 1) {
     trigged = 0;
     enabled = 1;
     return 1;
  }
  return 0;
}

void timer::setMS(unsigned long value) {
   preset = value; 
   enabled = 1;
}

void timer::setS(unsigned int s) {
  unsigned long aux = s * 1000;
  setMS(aux);
}

void timer::reset() {
  update();
  elapsed = 0;
  trigged = 0;
}

void timer::enable() {
  reset();
}

void timer::disable() {
  reset();
  enabled = 0;
}


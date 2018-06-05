/*
*  timers.h
*
*/

#ifndef TIMERS_H
#define TIMERS_H
#define TIMERS_VERSION "0"

class timer {
 public: 
   timer();
   void setMS(unsigned long ms);
   void setS(unsigned int s);
   void reset();
   void enable();
   void disable();
   void update();
   unsigned long valueMS();
   unsigned long valueS();
   int event();
   int rotator(int modulus);
   
 private:
  void check();
 
  unsigned long last_read;
  unsigned long elapsed;
  unsigned long preset;
  int enabled; 
  int trigged;
};

#endif

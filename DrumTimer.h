/*
*  DrumTimer.h
*
*/

#ifndef DRUM_TIMER_H
#define DRUM_TIMER_H
#define DRUM_TIMER_VERSION "0"
#define DRUM_SLOTS 4
#include "Timers.h"

class DrumTimer {
   public: 
      DrumTimer();
      void setMS(unsigned long ms, unsigned char slot);
      void setS(unsigned int s, unsigned char slot);
      void reset();
      void update();
      void clear();
      unsigned long valueMS();
      unsigned long valueS();
      bool event();
      unsigned char state();
      void loop(bool enable);
   private:
      Timer lapTimer;
      unsigned long preset[DRUM_SLOTS];
      unsigned char status;
      unsigned char currentSlot;
      unsigned char maxSlots;
      bool triggered;
      bool loopable;  
};

#endif


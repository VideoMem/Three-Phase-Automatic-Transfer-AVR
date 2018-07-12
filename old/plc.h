/*
 Select line logic

 ---| |---+------(Lp)--
 l1 > 110v|
 ---| |---+
 l2 > 110v|
 ---| |---+ 
 l3 > 110v

***********************************************
: Transfer sequence detail, pseudo functional :
***********************************************

transfer to generator:
    Start generator. 
    If generator is started:
        wait generator warmup time or outage time
        connect generator line
    else
        Reset outage time.
        Reset warmup time

transfer to line hot:
    l = select line to connect
    connect selected line
    
tranfer to line cold:
    wait outage time
    transfer to line hot

main loop:
    Mains failure (!Lp):
        Generator not present:
            Transfer to generator    
            
    (else, mains returns):            
        Generator present:
            transfer to line hot
            
        Generator not present:
            transfer to line cold

*/

#ifndef PLC_H
#define PLC_H
#define PLC_VERSION "0"
#include "generator.h"

class Plc {
    public:
        Plc();
        void setup();               //generic module handlers, setup()
        void update(bool status, bool started);   //generic module handlers, update()
        void reset();               //generic module handlers, reset()     
        bool halt();
    private:
        Generator generator;
        bool inputStatus; 

};

#endif


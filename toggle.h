/*
*  toggle.h
*
*/

#ifndef TOGGLE_H
#define TOGGLE_H
#define TOGGLE_VERSION "0"

class toggle {
    public: 
        toggle();
        bool value();       
        void reset();
        void set();
        void change();
   
    private:
        bool status;

};

#endif

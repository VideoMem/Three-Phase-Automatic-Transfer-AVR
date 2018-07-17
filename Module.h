/*
*  Module.h
*   Module Interface
*   It has two main methods for setup, and loop (update)
*/

#ifndef MODULE_H
#define MODULE_H
#define MODULE_VERSION "0"

class Module {
    public:
        virtual void setup(void);
        virtual void update(void);
    protected:
    private:
};

#endif

/*
*  Module.h
*   ModuleLoader as executor of Strategy Modules (see Strategy Software Design Pattern)
*   It has two main methods for setup, and loop (update)
*/

#ifndef MODULELOADER_H
#define MODULELOADER_H
#define MODULELOADER_VERSION "0"
//#include "Module.h"

class Module;
class Input;

class ModuleLoader {
 public:
    ModuleLoader()
    {
        module_ = NULL;
    }
    void setup();
    void update();
  private:
    Module *module_;
}

#endif

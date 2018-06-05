#include "toggle.h"

toggle::toggle() {
    status = false;
}

void toggle::set() {
    status = true;
}

void toggle::reset() {
    status = false;
}

bool toggle::value() {
    return status;
}

void toggle::change() {
    if(status == false)
        status = true;
    else
        status = false;
}


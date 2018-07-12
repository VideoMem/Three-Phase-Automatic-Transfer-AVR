#include "plc.h"

Plc::Plc() {
    generator.setup();
}

void Plc::update(bool status,bool started) {
    inputStatus = status;
    generator.update(status, started);
}

bool Plc::halt() {
    return generator.halt();
}

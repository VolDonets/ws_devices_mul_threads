//
// Created by vovan on 02.04.2020.
//

#include "event_ws.h"

EventWS::EventWS(int idEvent) {
    this->idEvent = idEvent;
}

int getEventID() {
    return this->idEvent;
}

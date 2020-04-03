//
// Created by vovan on 02.04.2020.
//

#include "delegate_ws.h"

DelegateWS* DelegateWS::p_instance = 0;

DelegateWS* DelegateWS::getInstance() {
    if (!p_instance)
        p_instance = new DelegateWS();
    return p_instance;
}

void DelegateWS::doEvent(EventWS& event) {
    for (HandlerWS* handlerWs: m_handlers)
        handlerWs->handleEventWS(event);
}

void DelegateWS::addHandler(HandlerWS& handler) {
    m_handlers.push_back(&handler);
}
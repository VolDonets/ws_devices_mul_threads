//
// Created by vovan on 02.04.2020.
//

#ifndef WS_DEVICES_2_HANDLER_WS_H
#define WS_DEVICES_2_HANDLER_WS_H

/*These is abstract class HandlerWS, which easy to use, when you just extend it
 * it's used for handling events*/
class HandlerWS {
public:
    /*function for handling events, it get a param Event
     * and to do smt when event happens
     * ~> public function*/
    virtual void handleEventWS(EventWS&) {};

protected:
    HandlerWS() {}
};


#endif //WS_DEVICES_2_HANDLER_WS_H

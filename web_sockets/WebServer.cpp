//
// Created by vovan on 25.03.2020.
//

#include "WebServer.h"

MyHandler::MyHandler(MyServer* server) : _server(server){
    this->_delegate = DelegateWS::getInstance();
    this->_eventNewClient = new EventWS(EVENT_HAVE_CLIENT);
    this->_eventLoseClient = new EventWS(EVENT_HAVE_LEAVE_CLIENT);
    this->_eventGetMQTT = new EventWS(EVENT_GIVE_MQTT_STATUS);
    this->_eventChangeMQTT = new EventWS(EVENT_MQTT_CHANGE_STATUS);
}

void MyHandler::onConnect(WebSocket* connection) {

    cout << "onConnect getRequestUri: " << connection->getRequestUri() << endl;
    _connections.insert(connection);
    cout << "Connected: " << connection->getRequestUri()
         << " : " << formatAddress(connection->getRemoteAddress())
         << endl;
    cout << "Credentials: " << *(connection->credentials()) << endl;
    _delegate->doEvent(*_eventNewClient);
}

void MyHandler::onData(WebSocket* connection, const char* data) {
    cout << "onData: " << data << endl;
    if ( strcmp(data, "GIVE_MQTT_STATUS") == 0) {
        cout << "wanna GET MQTT status" << endl;
    } else if ( strcmp(data, "MQTT_CHANGE_STATUS")) {
        cout << "wanna CHANGE MQTT status" << endl;
    }
}

void MyHandler::onDisconnect(WebSocket* connection) {
    _connections.erase(connection);
    cout << "Disconnected: " << connection->getRequestUri()
         << " : " << formatAddress(connection->getRemoteAddress())
         << endl;
    _delegate->doEvent(*_eventLoseClient);
}

void MyHandler::sendValuesJSON(std::string values) {
    for (auto c : _connections) {
        c->send(values);
    }
}
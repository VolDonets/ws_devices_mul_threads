//
// Created by vovan on 25.03.2020.
//

#include "WebServer.h"

MyHandler::MyHandler(MyServer* server) : _server(server){
    //_count_connections = 0;
    this->_delegate = DelegateWS::getInstance();
    this->_eventNewClient = new EventWS(EVENT_HAVE_CLIENT);
    this->_eventLoseClient = new EventWS(EVENT_HAVE_LEAVE_CLIENT);
}

void MyHandler::onConnect(WebSocket* connection) {

    cout << "onConnect getRequestUri: " << connection->getRequestUri() << endl;
    _connections.insert(connection);
    cout << "Connected: " << connection->getRequestUri()
         << " : " << formatAddress(connection->getRemoteAddress())
         << endl;
    cout << "Credentials: " << *(connection->credentials()) << endl;
    _delegate->doEvent(*_eventNewClient);
    //_count_connections++;
}

void MyHandler::onData(WebSocket* connection, const char* data) {
    cout << "onData " << endl;
}

void MyHandler::onDisconnect(WebSocket* connection) {
    _connections.erase(connection);
    cout << "Disconnected: " << connection->getRequestUri()
         << " : " << formatAddress(connection->getRemoteAddress())
         << endl;
    //_count_connections--;
    _delegate->doEvent(*_eventLoseClient);
}

void MyHandler::sendValuesJSON(std::string values) {
    //std::cout << values << endl;
    for (auto c : _connections) {
        c->send(values);
    }
}

/*int MyHandler::getCountConnections() {
    return _count_connections;
}*/
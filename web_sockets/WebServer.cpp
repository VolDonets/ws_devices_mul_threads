//
// Created by vovan on 25.03.2020.
//

#include "WebServer.h"

MyHandler::MyHandler(MyServer* server) : _server(server) {
}

void MyHandler::onConnect(WebSocket* connection) {

    cout << "onConnect getRequestUri: " << connection->getRequestUri() << endl;
    _connections.insert(connection);
    cout << "Connected: " << connection->getRequestUri()
         << " : " << formatAddress(connection->getRemoteAddress())
         << endl;
    cout << "Credentials: " << *(connection->credentials()) << endl;
}

void MyHandler::onData(WebSocket* connection, const char* data) {
    cout << "onData " << endl;
}

void MyHandler::onDisconnect(WebSocket* connection) {
    _connections.erase(connection);
    cout << "Disconnected: " << connection->getRequestUri()
         << " : " << formatAddress(connection->getRemoteAddress())
         << endl;
}

void MyHandler::sendValuesJSON(std::string values) {
    //std::cout << values << endl;
    for (auto c : _connections) {
        c->send(values);
    }
}
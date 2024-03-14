//
// Created by alperen on 27.09.2023.
//

#include "Client.h"

Client::Client(string const& _id, string const& _ip, string const& _mac) {
    client_id = _id;
    client_ip = _ip;
    client_mac = _mac;
}

ostream &operator<<(ostream &os, const Client &client) {
    os << "client_id: " << client.client_id << " client_ip: " << client.client_ip << " client_mac: "
       << client.client_mac << endl;
    return os;
}

Client::~Client() {
    // TODO: Free any dynamically allocated memory if necessary.
    while (!outgoing_queue.empty()) {
        stack<Packet*> frameStack = outgoing_queue.front();
        while (!frameStack.empty()) {
            delete frameStack.top();
            frameStack.pop();
        }
        outgoing_queue.pop();
    }

    while (!incoming_queue.empty()) {
        stack<Packet*> frameStack = incoming_queue.front();
        while (!frameStack.empty()) {
            delete frameStack.top();
            frameStack.pop();
        }
        incoming_queue.pop();
    }
}
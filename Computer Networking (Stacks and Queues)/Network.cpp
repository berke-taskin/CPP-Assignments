#include "Network.h"
#include <fstream>
#include <ctime>

Network::Network() {

}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {
    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */
    for (auto& command : commands){
        std::istringstream iss(command);
        std::string commandString;
        iss >> commandString;
        if (commandString == "MESSAGE"){
            //Printing command info
            for (int i = 0; i < command.length()+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            std::cout << "Command: " << command << std::endl;
            for (int i = 0; i < command.length()+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;

            //Extracting message from commend
            std::string sender, receiver, message;
            iss >> sender >> receiver;
            int first = command.find('#');
            int second = command.find('#',first+1);
            message = command.substr(first+1,(second-first-1));
            std::string completeMessage = message;

            std::cout << "Message to be sent: \"" << message << "\"" << std::endl;
            std::cout << std::endl;

            int count = 1;
            
            //When message exceeds the limit
            while (message.length()>message_limit){
                std::stack<Packet*> frameStack;
                std::cout << "Frame #" << count << std::endl;
                std::string frameMessage = message.substr(0,message_limit);
                std::string restOfMessage = message.substr(message_limit,message.length()-message_limit);
                message = restOfMessage;

                for (auto& client : clients){
                    if (sender == client.client_id){
                        for (auto& client2 : clients){
                            if (receiver == client2.client_id){
                                //Application Layer Packet
                                ApplicationLayerPacket* p0 = new ApplicationLayerPacket(0, sender, receiver, frameMessage);
                                p0->frameNumber = count;
                                frameStack.push(p0);

                                //Transport Layer Packet
                                TransportLayerPacket* p1 = new TransportLayerPacket(1, sender_port, receiver_port);
                                frameStack.push(p1);

                                //Network Layer Packet
                                NetworkLayerPacket* p2 = new NetworkLayerPacket(2, client.client_ip, client2.client_ip);
                                frameStack.push(p2);

                                //Physical Layer Packet
                                auto iterator = client.routing_table.find(receiver);
                                std::string newReceiver = iterator->second;
                                if (newReceiver!=receiver){
                                    for (auto& client3 : clients){
                                        if (newReceiver==client3.client_id){
                                            PhysicalLayerPacket* p3 = new PhysicalLayerPacket(3, client.client_mac, client3.client_mac);
                                            frameStack.push(p3);
                                        }
                                    }
                                } else {
                                    PhysicalLayerPacket* p3 = new PhysicalLayerPacket(3, client.client_mac, client2.client_mac);
                                    frameStack.push(p3);
                                }

                                //Printing Every Packet Info
                                stack<Packet*> infoStack = frameStack;
                                for (int i = 0; i < frameStack.size() && !infoStack.empty(); ++i) {
                                    Packet* p = infoStack.top();
                                    p->print();
                                    infoStack.pop();
                                }

                                //Adding to queue
                                client.outgoing_queue.push(frameStack); 
                            }   
                        }
                    }
                }
                
                std::cout << "Message chunk carried: \"" << frameMessage << "\"" << std::endl;
                std::cout << "Number of hops so far: 0" << std::endl;
                for (int i = 0; i < 8; ++i) {
                    std::cout << "-";
                }
                std::cout << std::endl;

                count++;
            }
            //When message do not exceed the limit
            std::stack<Packet*> frameStack;
            std::cout << "Frame #" << count << std::endl;
            std::string frameMessage = message;

            for (auto& client : clients){
                    if (sender == client.client_id){
                        for (auto& client2 : clients){
                            if (receiver == client2.client_id){
                                //Application Layer Packet
                                ApplicationLayerPacket* p0 = new ApplicationLayerPacket(0, sender, receiver, frameMessage);
                                p0->frameNumber = count;
                                frameStack.push(p0);

                                //Transport Layer Packet
                                TransportLayerPacket* p1 = new TransportLayerPacket(1, sender_port, receiver_port);
                                frameStack.push(p1);

                                //Network Layer Packet
                                NetworkLayerPacket* p2 = new NetworkLayerPacket(2, client.client_ip, client2.client_ip);
                                frameStack.push(p2);

                                //Physical Layer Packet
                                auto iterator = client.routing_table.find(receiver);
                                std::string newReceiver = iterator->second;
                                if (newReceiver!=receiver){
                                    for (auto& client3 : clients){
                                        if (newReceiver==client3.client_id){
                                            PhysicalLayerPacket* p3 = new PhysicalLayerPacket(3, client.client_mac, client3.client_mac);
                                            frameStack.push(p3);
                                        }
                                    }
                                } else {
                                    PhysicalLayerPacket* p3 = new PhysicalLayerPacket(3, client.client_mac, client2.client_mac);
                                    frameStack.push(p3);
                                }

                                //Printing Every Packet Info
                                stack<Packet*> infoStack = frameStack;
                                for (int i = 0; i < frameStack.size() && !infoStack.empty(); ++i) {
                                    Packet* p = infoStack.top();
                                    p->print();
                                    infoStack.pop();
                                }

                                //Adding to queue
                                client.outgoing_queue.push(frameStack); 
                            }
                        }
                    }
                }
            //LOG
            for (auto& client : clients){
                if (client.client_id==sender){
                    std::time_t currentTime = std::time(nullptr);
                    std::string cTimeStr = std::ctime(&currentTime);
                    Log log(cTimeStr,completeMessage,count,0,sender,receiver,true,ActivityType::MESSAGE_SENT);
                    client.log_entries.push_back(log);     
                } 
            }    

            std::cout << "Message chunk carried: \"" << frameMessage << "\"" << std::endl;
            std::cout << "Number of hops so far: 0" << std::endl;
            for (int i = 0; i < 8; ++i) {
                std::cout << "-";
            }

            std::cout << std::endl;
            count++;
        }
        else if (commandString == "SHOW_FRAME_INFO"){
            //Printing command info
            for (int i = 0; i < command.length()+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            std::cout << "Command: " << command << std::endl;
            for (int i = 0; i < command.length()+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;

            std::string clientid, inOrOut, num;
            iss >> clientid >> inOrOut >> num;
            int frameNum = stoi(num);   

            for (auto& client : clients){
                if (client.client_id == clientid){
                    //Outgoing Queue
                    if (inOrOut=="out"){
                        if (frameNum <= client.outgoing_queue.size()){
                            queue<stack<Packet*>> infoQueue = client.outgoing_queue;

                            for (int i = 0; i < frameNum-1 && !infoQueue.empty(); ++i) {
                                infoQueue.pop();
                            }  

                            stack<Packet*> frameStack = infoQueue.front();
                            stack<Packet*> infoStack = frameStack;
                            stack<Packet*> infoStack2;
                            
                            int hop=0;
                            std::cout << "Current Frame #" << frameNum << " on the outgoing queue of client " << clientid <<std::endl;
                            for (int i = 0; i < frameStack.size() && !infoStack.empty(); ++i) {
                                Packet* p = infoStack.top();
                                infoStack2.push(p);
                                infoStack.pop();
                            }
                            for (int i = 0; i < frameStack.size() && !infoStack2.empty(); ++i) {
                                Packet* p = infoStack2.top();
                                if (i==0){
                                    ApplicationLayerPacket* p2 = dynamic_cast<ApplicationLayerPacket*>(p);
                                    hop = p2->hopNumber;
                                    std::cout << "Carried Message: \"" << p2->message_data << "\""<<std::endl;
                                }
                                std::cout << "Layer " << p->layer_ID << " info: ";
                                p->print();
                                infoStack2.pop();
                            }
                            std::cout << "Number of hops so far: " << hop << std::endl;   
                        } else {
                            std::cout << "No such frame." << std::endl;
                        }     
                    }
                    //Incoming Queue
                    if (inOrOut=="in"){
                        if (frameNum <= client.incoming_queue.size()){
                            queue<stack<Packet*>> infoQueue = client.incoming_queue;

                            for (int i = 0; i < frameNum-1 && !infoQueue.empty(); ++i) {
                                infoQueue.pop();
                            }  

                            stack<Packet*> frameStack = infoQueue.front();
                            stack<Packet*> infoStack = frameStack;
                            stack<Packet*> infoStack2;
                            
                            int hop=0;
                            std::cout << "Current Frame #" << frameNum << " on the incoming queue of client " << clientid <<std::endl;
                            for (int i = 0; i < frameStack.size() && !infoStack.empty(); ++i) {
                                Packet* p = infoStack.top();
                                infoStack2.push(p);
                                infoStack.pop();
                            }
                            for (int i = 0; i < frameStack.size() && !infoStack2.empty(); ++i) {
                                Packet* p = infoStack2.top();
                                if (i==0){
                                    ApplicationLayerPacket* p2 = dynamic_cast<ApplicationLayerPacket*>(p);
                                    hop = p2->hopNumber;
                                    std::cout << "Carried Message: \"" << p2->message_data << "\""<<std::endl;
                                }
                                std::cout << "Layer " << p->layer_ID << " info: ";
                                p->print();
                                infoStack2.pop();
                            }
                            std::cout << "Number of hops so far: " << hop << std::endl; 
                        } else {
                            std::cout << "No such frame." << std::endl;
                        }     
                    }      
                }
            }     
        }
        else if (commandString == "SHOW_Q_INFO"){
            //Printing command info
            for (int i = 0; i < command.length()+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            std::cout << "Command: " << command << std::endl;
            for (int i = 0; i < command.length()+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;

            std::string clientid, inOrOut;
            iss >> clientid >> inOrOut;

            for (auto& client : clients){
                if (client.client_id == clientid){
                    //Outgoing Queue
                    if (inOrOut=="out"){
                        std::cout << "Client " << clientid << " Outgoing Queue Status" << std::endl;
                        std::cout << "Current total number of frames: " << client.outgoing_queue.size() << std::endl;
                    }
                    //Incoming Queue
                    if (inOrOut=="in"){
                        std::cout << "Client " << clientid << " Incoming Queue Status" << std::endl;
                        std::cout << "Current total number of frames: " << client.incoming_queue.size() << std::endl;
                    }
                }
            }
        }
        else if (commandString == "SEND"){
            //Printing command info
            for (int i = 0; i < command.length()+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            std::cout << "Command: " << command << std::endl;
            for (int i = 0; i < command.length()+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            
            for (auto& client : clients){
                if (client.outgoing_queue.size()>0){
                    int n = client.outgoing_queue.size();   
                    //Traversing through outgoing queue
                    for (int i = 0; i < n && !client.outgoing_queue.empty(); ++i) {
                        stack<Packet*> frameStack = client.outgoing_queue.front();
                        stack<Packet*> infoStack = frameStack;
                        stack<Packet*> infoStack2 = frameStack;
                        int hop=0;
                        int frame=0;
                        std::string message;

                        //Getting information
                        for (int i = 0; i < frameStack.size() && !infoStack2.empty(); ++i) {
                            Packet* p = infoStack2.top();
                            if (i==frameStack.size()-1){
                                ApplicationLayerPacket* p2 = dynamic_cast<ApplicationLayerPacket*>(p);
                                p2->hopNumber=p2->hopNumber+1;
                                hop = p2->hopNumber;
                                frame = p2->frameNumber;
                                message = p2->message_data;
                            }
                            infoStack2.pop();
                        }

                        //Sending frames
                        for (int i = 0; i < frameStack.size() && !infoStack.empty(); ++i) {
                            Packet* p = infoStack.top();
                            if (i==0){
                                PhysicalLayerPacket* p2 = dynamic_cast<PhysicalLayerPacket*>(p);
                                for (auto& client2 : clients){
                                    if (client2.client_mac==p2->receiver_MAC_address){
                                        std::cout << "Client " << client.client_id << " sending frame #" << frame << " to client " << client2.client_id << std::endl;
                                        client2.incoming_queue.push(frameStack);
                                    }
                                }
                            }
                            p->print();
                            infoStack.pop();
                        }
                        client.outgoing_queue.pop();
                        std::cout << "Message chunk carried: \"" << message << "\"" << std::endl;
                        std::cout << "Number of hops so far: " << hop << std::endl;
                        for (int i = 0; i < 8; ++i) {
                            std::cout << "-";
                        }
                        std::cout << std::endl;
                    }
                }
            }
        }
        else if (commandString == "RECEIVE"){
            //Printing command info
            for (int i = 0; i < command.length()+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            std::cout << "Command: " << command << std::endl;
            for (int i = 0; i < command.length()+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;

            for (auto& client : clients){
                //If any client has at least one frame in their incoming queue
                if (client.incoming_queue.size()>0){
                    int n = client.incoming_queue.size();
                    queue<stack<Packet*>> infoQueue = client.incoming_queue;
                    int frame=0;
                    int hop=0;
                    int count=0;
                    bool forward;
                    bool found;
                    std::string receiver, sender, messageData, message, storingPrevStackSender;
                    stack<Packet*> maybeICanUseStack;

                    //Traversing through incoming queue 
                    for (int i = 0; i < n && !client.incoming_queue.empty(); ++i) {
                        stack<Packet*> frameStack = client.incoming_queue.front();
                        stack<Packet*> infoStack = frameStack;
                        stack<Packet*> infoStack2 = frameStack;
                        maybeICanUseStack = frameStack;
                        forward=false;
                        bool frameStart=false;

                        //Getting information
                        for (int i = 0; i < frameStack.size() && !infoStack.empty(); ++i) {
                            Packet* p = infoStack.top();
                            if (i==frameStack.size()-1){
                                ApplicationLayerPacket* p2 = dynamic_cast<ApplicationLayerPacket*>(p);
                                receiver = p2->receiver_ID;
                                sender = p2->sender_ID;
                                messageData = p2->message_data;
                                if (client.client_id == receiver){ 
                                    forward=false;
                                }
                                if (client.client_id != receiver) {
                                    forward=true;
                                }
                                hop = p2->hopNumber;
                                frame = p2->frameNumber;
                                if (frame==1){
                                    frameStart=true;
                                }
                            }
                            infoStack.pop();
                        }

                        //If message needs to be forwarded to another client
                        if (forward){
                            auto iterator = client.routing_table.find(receiver);
                            std::string newReceiver = iterator->second;

                            std::string prevSender;
                            Packet* p = frameStack.top();
                            PhysicalLayerPacket* p2 = dynamic_cast<PhysicalLayerPacket*>(p);
                            for (auto& client3 : clients){
                                if (client3.client_mac == p2->sender_MAC_address){
                                    prevSender = client3.client_id;
                                }
                            }

                            //Receiving frames
                            found=false;
                            for (auto& client2 : clients){
                                if (client2.client_id==newReceiver){
                                    found=true;
                                    //If a frame with another message started and its the first frame in queue
                                    if (frameStart && i==0){
                                        std::cout << "Client " << client.client_id << " receiving a message from client " << prevSender << ", but intended for client " <<
                                        receiver << ". Forwarding... " << std::endl;    
                                    }

                                    //If a frame with another message started and its NOT the first frame in queue
                                    if (frameStart && i!=0){
                                        for (int i = 0; i < 8; ++i) {
                                            std::cout << "-";
                                        }
                                        std::cout << std::endl;   
                                        std::cout << "Client " << client.client_id << " receiving a message from client " << prevSender << ", but intended for client " <<
                                        receiver << ". Forwarding... " << std::endl;  
                                        
                                        //LOG
                                        std::time_t currentTime = std::time(nullptr);
                                        std::string cTimeStr = std::ctime(&currentTime);
                                        Log log(cTimeStr,"",count,hop,sender,receiver,true,ActivityType::MESSAGE_FORWARDED);
                                        client.log_entries.push_back(log);
                                        count=0;        
                                    }
                                    p2->sender_MAC_address = client.client_mac;
                                    p2->receiver_MAC_address = client2.client_mac;

                                    std::cout << "Frame #" << frame << " MAC address change: New sender MAC " << p2->sender_MAC_address << ", new receiver MAC " << 
                                    p2->receiver_MAC_address << std::endl; 
 
                                    client.outgoing_queue.push(frameStack);
                                }
                            }

                            //If clients are routed incorrectly and the next hop shall be failed
                            if (!found){
                                if (frameStart && i!=0){
                                    for (int i = 0; i < 8; ++i) {
                                        std::cout << "-";
                                    }
                                    std::cout << std::endl;

                                    //LOG
                                    std::time_t currentTime = std::time(nullptr);
                                    std::string cTimeStr = std::ctime(&currentTime);
                                    Log log(cTimeStr,"",count,hop,sender,receiver,false,ActivityType::MESSAGE_DROPPED);
                                    client.log_entries.push_back(log);
                                    count=0; 
                                }
                                std::cout << "Client " << client.client_id << " receiving frame #" << frame << " from client " << sender << ", but intended for client " <<
                                receiver << ". Forwarding... " << std::endl;  
                                std::cout << "Error: Unreachable destination. Packets are dropped after "  << hop << " hops!" << std::endl; 

                                //Deallocating memory here
                                while (!frameStack.empty()) {
                                    delete frameStack.top();
                                    frameStack.pop();
                                }
                            }       
                        }  

                        //If message has made its way to the originally intended client
                        else if (!forward){
                            std::string prevSender;
                            Packet* p = frameStack.top();
                            PhysicalLayerPacket* p2 = dynamic_cast<PhysicalLayerPacket*>(p);
                            for (auto& client2 : clients){
                                if (client2.client_mac == p2->sender_MAC_address){
                                    prevSender = client2.client_id;
                                }
                            }

                            //Receiving frames
                            for (auto& client3 : clients){
                                if (client3.client_id==receiver){
                                    //If a frame with another message started and its NOT the first frame in queue
                                    if (frameStart && i!=0){
                                        std::cout << "Client " << receiver << " received the message \"" << message << "\" from client " << storingPrevStackSender << "." << std::endl;
                                        for (int i = 0; i < 8; ++i) {
                                            std::cout << "-";
                                        }
                                        std::cout << std::endl;  

                                        //LOG
                                        std::time_t currentTime = std::time(nullptr);
                                        std::string cTimeStr = std::ctime(&currentTime);
                                        Log log(cTimeStr,message,count,hop,sender,receiver,true,ActivityType::MESSAGE_RECEIVED);
                                        client.log_entries.push_back(log);
                                        count=0; 

                                        message = "";
                                    }
                                    
                                    //Assembling the message as per frame comes
                                    message += messageData;
                                    std::cout << "Client " << receiver << " receiving frame #" << frame << " from client " << prevSender << ", originating from client " <<
                                    sender << std::endl;  
                                    for (int i = 0; i < frameStack.size() && !infoStack2.empty(); ++i) {
                                        Packet* p = infoStack2.top();
                                        p->print();
                                        infoStack2.pop();
                                    }
                                    std::cout << "Message chunk carried: \"" << messageData << "\"" << std::endl;
                                    std::cout << "Number of hops so far: " << hop << std::endl;
                                    for (int i = 0; i < 8; ++i) {
                                        std::cout << "-";
                                    }
                                    std::cout << std::endl;
                                }
                            }

                            //Deallocating memory here
                            while (!frameStack.empty()) {
                                delete frameStack.top();
                                frameStack.pop();
                            }
                        }
                        storingPrevStackSender = sender;
                        client.incoming_queue.pop();    
                        count++;        
                    }  

                    //If last frame in queue is forwarded
                    if (forward){
                        for (int i = 0; i < 8; ++i) {
                            std::cout << "-";
                        }
                        std::cout << std::endl;

                        //If reachable destination
                        if (found){
                            //LOG
                            std::time_t currentTime = std::time(nullptr);
                            std::string cTimeStr = std::ctime(&currentTime);
                            Log log(cTimeStr,"",count,hop,sender,receiver,true,ActivityType::MESSAGE_FORWARDED);
                            client.log_entries.push_back(log);
                            count=0;     
                        }  

                        //If unreachable destination 
                        else if (!found){
                            //LOG
                            std::time_t currentTime = std::time(nullptr);
                            std::string cTimeStr = std::ctime(&currentTime);
                            Log log(cTimeStr,"",count,hop,sender,receiver,false,ActivityType::MESSAGE_DROPPED);
                            client.log_entries.push_back(log);
                            count=0; 
                        }                  
                    }

                    //If last frame in queue is received and message is assembled
                    if (!forward){
                        std::cout << "Client " << receiver << " received the message \"" << message << "\" from client " << sender << "." << std::endl;
                        for (int i = 0; i < 8; ++i) {
                            std::cout << "-";
                        }
                        std::cout << std::endl;  

                        //LOG
                        std::time_t currentTime = std::time(nullptr);
                        std::string cTimeStr = std::ctime(&currentTime);
                        Log log(cTimeStr,message,count,hop,sender,receiver,true,ActivityType::MESSAGE_RECEIVED);
                        client.log_entries.push_back(log);
                        count=0; 

                        message = "";
                    }
                }
            }
        }
        else if (commandString == "PRINT_LOG"){
            //Printing command info
            for (int i = 0; i < command.length()+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            std::cout << "Command: " << command << std::endl;
            for (int i = 0; i < command.length()+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;

            //Printing the logs accordingly
            std::string clientid;
            iss >> clientid;
            for (auto& client : clients){
                if (client.client_id == clientid){
                    if (client.log_entries.size()>0){
                        std::cout << "Client " << clientid  << " Logs:"<< std::endl;
                        for (int i = 0; i < 14; ++i) {
                            std::cout << "-";
                        }
                        std::cout << std::endl;
                        for (int i = 0; i < client.log_entries.size(); ++i) {
                            Log log = client.log_entries[i];
                            std::string typeString, successString;

                            //Strings are determined 
                            if(log.activity_type==ActivityType::MESSAGE_SENT){
                                typeString="Message Sent";
                            }
                            else if(log.activity_type==ActivityType::MESSAGE_DROPPED){
                                typeString="Message Dropped";
                            }
                            else if(log.activity_type==ActivityType::MESSAGE_FORWARDED){
                                typeString="Message Forwarded";
                            }
                            else if(log.activity_type==ActivityType::MESSAGE_RECEIVED){
                                typeString="Message Received";
                            }
                            if (log.success_status){
                                successString = "Yes";
                            }
                            else if (!log.success_status){
                                successString = "No";
                            }

                            //Printing process
                            std::cout << "Log Entry #" << i+1 << ":" << std::endl;
                            std::cout << "Activity: " << typeString << std::endl;
                            std::cout << "Timestamp: " << log.timestamp;
                            std::cout << "Number of frames: " << log.number_of_frames << std::endl;
                            std::cout << "Number of hops: " << log.number_of_hops << std::endl;
                            std::cout << "Sender ID: " << log.sender_id << std::endl;
                            std::cout << "Receiver ID: " << log.receiver_id << std::endl;
                            std::cout << "Success: " << successString << std::endl;
                            if (typeString=="Message Sent" || typeString=="Message Received"){
                                std::cout << "Message: \"" << log.message_content << "\"" << std::endl;    
                            }
                            if (i!=client.log_entries.size()-1){
                                for (int i = 0; i < 14; ++i) {
                                    std::cout << "-";
                                }
                                std::cout << std::endl;    
                            }
                            
                        }
                    }
                }
            }
        }
        else {
            //Printing command info
            for (int i = 0; i < command.length()+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            std::cout << "Command: " << command << std::endl;
            for (int i = 0; i < command.length()+9; ++i) {
                std::cout << "-";
            }
            std::cout << std::endl;
            std::cout << "Invalid command." << std::endl;
        }
    }
}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;
    // TODO: Read clients from the given input file and return a vector of Client instances.
    std::ifstream inputFile(filename);
    while (inputFile.is_open()){
        std::string line;
        bool once = false;
        int clientNum = 0;
        while (getline(inputFile, line)){
            if (once==false){
                clientNum = stoi(line);
                once = true;
            }
            else if (once==true){
                std::istringstream iss(line);
                string client_id, client_ip, client_mac;
                iss >> client_id >> client_ip >> client_mac;
                Client client(client_id, client_ip, client_mac);
                clients.push_back(client);
            }
        }

        inputFile.close();
    }
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
    std::ifstream inputFile(filename);

    while (inputFile.is_open()){
        for (auto& client : clients) {
            std::string line;
            while (getline(inputFile, line)){
                if (line=="-"){
                    break;
                }
                std::istringstream iss(line);
                string receiverID, nexthopID;
                iss >> receiverID >> nexthopID;
                client.routing_table[receiverID] = nexthopID;
            }
        }
        
        inputFile.close();
    }
    
}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    // TODO: Read commands from the given input file and return them as a vector of strings.
    std::ifstream inputFile(filename);
    while (inputFile.is_open()){
        std::string line;
        bool once = false;
        int commandNum = 0;
        while (getline(inputFile, line)){
            if (once==false){
                commandNum = stoi(line);
                once = true;
            }
            else if (once==true){
                commands.push_back(line);
            }
            
        }

        inputFile.close();
    }
    return commands;
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.
}

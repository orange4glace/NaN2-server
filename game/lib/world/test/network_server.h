#pragma once

#include "locked_queue.h"

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bimap.hpp>
#include <boost/thread.hpp>

#include <string>
#include <array>

using boost::asio::ip::udp;

struct ClientMessage {
    uint8_t* data;
    unsigned int size;

    ~ClientMessage() {
        delete data;
    }
};

typedef boost::bimap<int64_t, udp::endpoint> ClientList;
typedef ClientList::value_type Client;

class NetworkServer {
public:
    NetworkServer(unsigned short local_port);
    ~NetworkServer();

    bool HasMessages();
    ClientMessage* PopMessage();

    void SendToClient(std::vector<uint8_t>& message, uint64_t clientID, bool guaranteed = false);
    void SendToAllExcept(std::vector<uint8_t>& message, uint64_t clientID, bool guaranteed = false);
    void SendToAll(std::vector<uint8_t>& message, bool guaranteed = false);

    inline uint64_t GetStatReceivedMessages() {return receivedMessages;};
    inline uint64_t GetStatReceivedBytes()    {return receivedBytes;};
    inline uint64_t GetStatSentMessages()     {return sentMessages;};
    inline uint64_t GetStatSentBytes()        {return sentBytes;};

private:
    // Network send/receive stuff
    boost::asio::io_service io_service;
    udp::socket socket;
    udp::endpoint server_endpoint;
    udp::endpoint remote_endpoint;
    std::array<char, 1024> recv_buffer;
    boost::thread service_thread;

    void start_receive();
    void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred);
    void handle_send(std::string /*message*/, const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/)    {}
    void run_service();
    uint64_t get_client_id(udp::endpoint endpoint);

    void send(std::vector<uint8_t>& pmessage, udp::endpoint target_endpoint);

    // Incoming messages queue
    locked_queue<ClientMessage*> incomingMessages;

    // Clients of the server
    ClientList clients;
    uint64_t nextClientID;

    NetworkServer(NetworkServer&); // block default copy constructor

    // Statistics
    uint64_t receivedMessages;
    uint64_t receivedBytes;
    uint64_t sentMessages;
    uint64_t sentBytes;
};
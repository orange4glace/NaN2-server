#include <boost/bind.hpp>

#include "network_server.h"

#include <iostream>

NetworkServer::NetworkServer(unsigned short local_port) :
    socket(io_service, udp::endpoint(udp::v4(), local_port)),
    nextClientID(0L),
    service_thread(std::bind(&NetworkServer::run_service, this))
{
    std::cout << "Starting server on port " << local_port << std::endl;
};

NetworkServer::~NetworkServer()
{
    io_service.stop();
    service_thread.join();
}

void NetworkServer::start_receive()
{
    socket.async_receive_from(boost::asio::buffer(recv_buffer), remote_endpoint,
        boost::bind(&NetworkServer::handle_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void NetworkServer::handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (!error)
    {
        try {
            auto message = ClientMessage(std::string(recv_buffer.data(), recv_buffer.data() + bytes_transferred), get_client_id(remote_endpoint));
            if (!message.first.empty())
                incomingMessages.push(message);
            receivedBytes += bytes_transferred;
            receivedMessages++;
        }
        catch (std::exception ex) {
            std::cout << "handle_receive: Error parsing incoming message:" << ex.what() << std::endl;
        }
        catch (...) {
            std::cout << "handle_receive: Unknown error while parsing incoming message" << std::endl;
        }
    }
    else
    {
        std::cout << "handle_receive: error: " << error.message()  << std::endl;
    }

    start_receive();
}

void NetworkServer::send(boost::array<uint8_t, 1024>& buffer, udp::endpoint target_endpoint)
{
    socket.send_to(boost::asio::buffer(buffer), target_endpoint);
    sentBytes += buffer.size();
    sentMessages++;
}

void NetworkServer::run_service()
{
    start_receive();
    while (!io_service.stopped()){
        try {
            io_service.run();
        } catch( const std::exception& e ) {
            std::cout << "Server network exception: " << e.what() << std::endl;
        }
        catch(...) {
            std::cout << "Unknown exception in server network thread" << std::endl;
        }
    }
    std::cout << "Server network thread stopped" << std::endl;
};

uint64_t NetworkServer::get_client_id(udp::endpoint endpoint)
{
    auto cit = clients.right.find(endpoint);
    if (cit != clients.right.end())
        return (*cit).second;

    nextClientID++;
    clients.insert(Client(nextClientID, endpoint));
    return nextClientID;
};

void NetworkServer::SendToClient(uint8_t* flatbuffer_data, unsigned int flatbuffer_data_size, uint64_t clientID, bool guaranteed) 
{ 
    boost::array<uint8_t, 1024> buffer;
    std::memcpy(&buffer[0], flatbuffer_data, flatbuffer_data_size);

    try {
        send(buffer, clients.left.at(clientID));
    }
    catch (std::out_of_range) {
        std::cout << "Unknown client ID" << std::endl;
    }
};

void NetworkServer::SendToAllExcept(uint8_t* flatbuffer_data, unsigned int flatbuffer_data_size, uint64_t clientID, bool guaranteed)
{
    boost::array<uint8_t, 1024> buffer;
    std::memcpy(&buffer[0], flatbuffer_data, flatbuffer_data_size);

    for (auto client: clients)
        if (client.left != clientID)
            send(buffer, client.right);
};

void NetworkServer::SendToAll(uint8_t* flatbuffer_data, unsigned int flatbuffer_data_size, bool guaranteed)
{
    boost::array<uint8_t, 1024> buffer;
    std::memcpy(&buffer[0], flatbuffer_data, flatbuffer_data_size);

    for (auto client: clients)
        send(buffer, client.right);
};

ClientMessage NetworkServer::PopMessage() {
    return incomingMessages.pop();
}

bool NetworkServer::HasMessages()
{
    return !incomingMessages.empty();
};
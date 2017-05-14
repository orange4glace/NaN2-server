#define BOOST_LOG_DYN_LINK 1

#include <iostream>
#include <thread>
#include <ratio>
#include <ctime>
#include <chrono>
#include "network_server.h"
#include <nan2/world/world/world.h>
#include <nan2/world/world_time.h>

#include <boost/log/trivial.hpp>

using namespace std;
using namespace nan2;

int main() {

  NetworkServer serv(3555);

  World world;


  while (1) {
    std::chrono::high_resolution_clock::time_point last_system_time(std::chrono::high_resolution_clock::now());

    while (serv.HasMessages()) {
      ClientMessage* msg = serv.PopMessage();
      world.OnPacketReceived(msg->data, msg->size);
      delete msg;
    }
    world.Update();
    while (world.SendPacketQueueSize()) {
      auto buffer = world.PopSendPacket();
      serv.SendToAll(buffer);
    }

    std::chrono::high_resolution_clock::time_point cur_system_time(std::chrono::high_resolution_clock::now());
    std::chrono::duration<double, std::milli> time_span = cur_system_time - last_system_time;
    int dt = time_span.count();
    dt = (16 - dt > 0 ? 16 - dt : 0);
    this_thread::sleep_for(std::chrono::milliseconds(dt));
  }

}
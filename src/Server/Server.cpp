//  Hello World server
#include <SFML/Graphics.hpp>
#include <chrono>
#include <cmath>
#include <iostream>
#include <map>
#include <thread>
#include <vector>
#include <zmq.hpp>

#include "Object/Object.hpp"
#include "SfmlTimeline/SfmlTimeline.hpp"
#include "UserAction/UserAction.hpp"

void handle_platform(Object& platform) {
  const float velocity = 200.0;
  SfmlTimeline server_time;
  while (true) {
    platform.pos_x =
        150.0 +
        std::abs(std::fmod(server_time.get_time().asSeconds() * velocity, 800) -
                 400);
    std::this_thread::sleep_for(std::chrono::milliseconds{10});
  }
}

void handle_character(std::vector<int>& iteration_list,
                      std::map<int, Object>& character_list) {
  // setup socket
  zmq::context_t sub_context;
  zmq::socket_t sub_socket{sub_context, ZMQ_SUB};
  sub_socket.bind("tcp://127.0.0.1:5556");
  sub_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  //
  while (true) {
    zmq::message_t message;
    zmq::recv_result_t result = sub_socket.recv(message);
    //
    UserAction* action = (UserAction*)message.data();
    if (action->type == 1) {
      character_list[action->user_id] = {2, 10, 0, 395, 200};
      iteration_list.push_back(0);
    } else if (action->type == 2) {
      character_list.erase(action->user_id);
    } else if (action->type == 3) {
      character_list[action->user_id].pos_x -= action->arg;
    } else if (action->type == 4) {
      character_list[action->user_id].pos_x += action->arg;
    } else if (action->type == 5) {
      character_list[action->user_id].pos_y -= action->arg;
    } else if (action->type == 6) {
      character_list[action->user_id].pos_y += action->arg;
    }
  }
}

int main(void) {
  // setup sockets
  zmq::context_t pub_context;
  zmq::socket_t obj_socket{pub_context, ZMQ_PUB};
  obj_socket.bind("tcp://127.0.0.1:5555");
  zmq::socket_t msg_socket{pub_context, ZMQ_PUB};
  msg_socket.bind("tcp://127.0.0.1:5557");
  // setup moving platform
  Object platform{1, 100, 10, 350, 400};
  std::thread platform_handler{handle_platform, std::ref(platform)};
  // setup iteration list and character list
  std::vector<int> iteration_list;
  std::map<int, Object> character_list;
  std::thread character_handler(handle_character, std::ref(iteration_list),
                                std::ref(character_list));
  while (true) {
    std::vector<Object> object_list;
    object_list.push_back(platform);
    for (std::pair<const int, Object>& character : character_list) {
      object_list.push_back(character.second);
    }
    obj_socket.send(zmq::message_t{object_list.begin(), object_list.end()},
                    zmq::send_flags::none);
    //
    for (int& iter_cnt : iteration_list) {
      iter_cnt += 1;
    }
    msg_socket.send(
        zmq::message_t{iteration_list.begin(), iteration_list.end()},
        zmq::send_flags::none);
    //
    std::this_thread::sleep_for(std::chrono::milliseconds{10});
  }
  return 0;
}
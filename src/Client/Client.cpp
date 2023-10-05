#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <set>
#include <thread>
#include <variant>
#include <vector>
#include <zmq.hpp>

#include "Object/Object.hpp"
#include "Timeline/Timeline.hpp"
#include "UserAction/UserAction.hpp"

int main(void) {
  zmq::context_t contex;
  // setup user id
  std::srand(std::time(nullptr));
  int user_id = rand();
  // setup subscribe socket
  zmq::socket_t sub_socket(contex, ZMQ_SUB);
  sub_socket.setsockopt(ZMQ_CONFLATE, 1);
  sub_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  sub_socket.connect("tcp://127.0.0.1:5555");
  // setup publish socket
  zmq::socket_t pub_socket(contex, ZMQ_PUB);
  pub_socket.connect("tcp://127.0.0.1:5556");
  // setup message socket
  zmq::socket_t msg_socket{contex, ZMQ_SUB};
  msg_socket.connect("tcp://127.0.0.1:5557");
  msg_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  // setup keypress list
  std::set<sf::Keyboard::Key> key_pressed;
  // setup timeline
  Timeline system_time;
  Timeline game_time(&system_time);
  sf::Time last_iter = game_time.get_time();
  // create character
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  UserAction join_action{user_id, 1};
  zmq::message_t message{&join_action, sizeof(UserAction)};
  pub_socket.send(message, zmq::send_flags::none);
  // create window
  sf::RenderWindow window(sf::VideoMode{800, 600}, "Homework 2");
  while (window.isOpen()) {
    // handle events
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::KeyPressed) {
        key_pressed.insert(event.key.code);
        if (event.key.code == sf::Keyboard::P) {
          if (game_time.is_paused()) {
            game_time.unpause();
          } else {
            game_time.pause();
          }
        } else if (event.key.code == sf::Keyboard::S) {
          if (game_time.get_ratio() == 1.0) {
            game_time.set_ratio(2.0);
          } else if (game_time.get_ratio() == 2.0) {
            game_time.set_ratio(0.5);
          } else {
            game_time.set_ratio(1.0);
          }
        }
      } else if (event.type == sf::Event::KeyReleased) {
        key_pressed.erase(event.key.code);
      } else if (event.type == sf::Event::Closed) {
        window.close();
        //
        UserAction leave_action{user_id, 2};
        zmq::message_t message{&leave_action, sizeof(UserAction)};
        pub_socket.send(message, zmq::send_flags::none);
      }
    }
    // send move actions
    const float velocity = 100.0;
    const sf::Time this_iter = game_time.get_time();
    const sf::Time time_delta = this_iter - last_iter;
    last_iter = this_iter;
    const float step =
        velocity * time_delta.asSeconds() * game_time.get_ratio();
    if (key_pressed.find(sf::Keyboard::Left) != key_pressed.end()) {
      UserAction move_action{user_id, 3, step};
      zmq::message_t message{&move_action, sizeof(UserAction)};
      pub_socket.send(message, zmq::send_flags::none);
    }
    if (key_pressed.find(sf::Keyboard::Right) != key_pressed.end()) {
      UserAction move_action{user_id, 4, step};
      zmq::message_t message{&move_action, sizeof(UserAction)};
      pub_socket.send(message, zmq::send_flags::none);
    }
    if (key_pressed.find(sf::Keyboard::Up) != key_pressed.end()) {
      UserAction move_action{user_id, 5, step};
      zmq::message_t message{&move_action, sizeof(UserAction)};
      pub_socket.send(message, zmq::send_flags::none);
    }
    if (key_pressed.find(sf::Keyboard::Down) != key_pressed.end()) {
      UserAction move_action{user_id, 6, step};
      zmq::message_t message{&move_action, sizeof(UserAction)};
      pub_socket.send(message, zmq::send_flags::none);
    }
    // recieve messages
    zmq::message_t message;
    zmq::recv_result_t result =
        msg_socket.recv(message, zmq::recv_flags::dontwait);
    if (result.has_value()) {
      std::vector<int> iteration_list;
      iteration_list.resize(message.size() / sizeof(int));
      std::memcpy(iteration_list.data(), message.data(), message.size());
      for (int i = 0; i < iteration_list.size(); i++) {
        std::cout << "Client " << i + 1 << ": Iteration " << iteration_list[i]
                  << "\n";
      }
      std::cout << std::endl;
    }
    // skip window rendering if paused
    if (game_time.is_paused()) {
      continue;
    }
    // recieve objects
    result = sub_socket.recv(message);
    std::vector<Object> object_list;
    object_list.resize(message.size() / sizeof(Object));
    std::memcpy(object_list.data(), message.data(), message.size());
    // draw objects
    window.clear();
    for (Object& object : object_list) {
      std::variant<sf::RectangleShape, sf::CircleShape> shape;
      if (object.shape == 1) {
        shape = sf::RectangleShape{{object.size_1, object.size_2}};
      } else if (object.shape == 2) {
        shape = sf::CircleShape{{object.size_1}};
      }
      //
      std::visit(
          [&](auto& shape) {
            shape.setPosition(object.pos_x, object.pos_y);
            window.draw(shape);
          },
          shape);
    }
    window.display();
  }
}
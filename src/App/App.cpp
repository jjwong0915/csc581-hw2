#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cmath>
#include <functional>
#include <iostream>
#include <set>

#include "../BasicTimeline/BasicTimeline.hpp"
#include "../SfmlTimeline/SfmlTimeline.hpp"
#include "../Timeline/Timeline.hpp"

std::set<sf::Keyboard::Key> key_pressed;
SfmlTimeline system_time;
Timeline game_time{&system_time};

void game_time_update() {
  sf::Time last_pause_unpause;
  sf::Time last_change_speed;
  while (true) {
    if (key_pressed.find(sf::Keyboard::P) != key_pressed.end() &&
        system_time.get_time() - last_pause_unpause > sf::seconds(0.25)) {
      if (game_time.is_paused()) {
        game_time.unpause();
      } else {
        game_time.pause();
      }
      last_pause_unpause = system_time.get_time();
    }
    if (key_pressed.find(sf::Keyboard::S) != key_pressed.end() &&
        system_time.get_time() - last_change_speed > sf::seconds(0.25)) {
      if (game_time.get_ratio() == 1.0) {
        game_time.set_ratio(2.0);
      } else if (game_time.get_ratio() == 2.0) {
        game_time.set_ratio(0.5);
      } else {
        game_time.set_ratio(1.0);
      }
      last_change_speed = system_time.get_time();
    }
    sf::sleep(sf::seconds(0.01));
  }
}

void platform_move(sf::RectangleShape& platform) {
  const float velocity = 200.0;
  //
  while (true) {
    platform.setPosition(
        {(float)150 +
             (float)std::abs(
                 std::fmod(game_time.get_time().asSeconds() * velocity, 800) -
                 400),
         400});
    sf::sleep(sf::seconds(0.01));
  }
}

void character_move(sf::CircleShape& character) {
  const float velocity = 200.0;
  //
  sf::Time last_time;
  while (true) {
    const sf::Time current_time = game_time.get_time();
    const sf::Time time_delta = current_time - last_time;
    if (key_pressed.find(sf::Keyboard::Left) != key_pressed.end()) {
      character.move(time_delta.asSeconds() * -velocity, 0);
    }
    if (key_pressed.find(sf::Keyboard::Right) != key_pressed.end()) {
      character.move(time_delta.asSeconds() * velocity, 0);
    }
    if (key_pressed.find(sf::Keyboard::Up) != key_pressed.end()) {
      character.move(0, time_delta.asSeconds() * -velocity);
    }
    if (key_pressed.find(sf::Keyboard::Down) != key_pressed.end()) {
      character.move(0, time_delta.asSeconds() * velocity);
    }
    last_time = current_time;
    sf::sleep(sf::seconds(0.01));
  }
}

int main() {
  // setup window
  sf::RenderWindow window(sf::VideoMode{800, 600}, "Homework 2");
  // setup game time
  sf::Thread game_time_updating(game_time_update);
  game_time_updating.launch();
  // setup moving platform
  sf::RectangleShape moving_platform({100, 10});
  moving_platform.setPosition(350, 400);
  sf::Thread moving_platform_movement(platform_move, std::ref(moving_platform));
  moving_platform_movement.launch();
  // setup character
  sf::CircleShape character(10);
  character.setPosition(395, 200);
  sf::Thread character_movement(character_move, std::ref(character));
  character_movement.launch();
  // game loop
  while (window.isOpen()) {
    // handle events
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::KeyPressed) {
        key_pressed.insert(event.key.code);
      } else if (event.type == sf::Event::KeyReleased) {
        key_pressed.erase(event.key.code);
      } else if (event.type == sf::Event::Closed) {
        game_time_updating.terminate();
        moving_platform_movement.terminate();
        character_movement.terminate();
        window.close();
      }
    }
    // draw frame
    window.clear();
    window.draw(moving_platform);
    window.draw(character);
    window.display();
  }
  return 0;
}

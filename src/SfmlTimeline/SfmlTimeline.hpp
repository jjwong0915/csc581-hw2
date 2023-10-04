#ifndef SFML_TIMELINE_HPP
#define SFML_TIMELINE_HPP

#include <SFML/System/Clock.hpp>

#include "../BasicTimeline/BasicTimeline.hpp"

class SfmlTimeline : virtual public BasicTimeline {
  sf::Clock clock;

 public:
  sf::Time get_time();
};

#endif
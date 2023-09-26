#ifndef SFML_TIMELINE_H
#define SFML_TIMELINE_H

#include <SFML/System/Clock.hpp>

#include "../BasicTimeline/BasicTimeline.hpp"

class SfmlTimeline : virtual public BasicTimeline {
  sf::Clock clock;

 public:
  sf::Time get_time();
};

#endif
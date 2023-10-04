#ifndef BASIC_TIMELINE_HPP
#define BASIC_TIMELINE_HPP

#include <SFML/System/Time.hpp>

class BasicTimeline {
 public:
  virtual sf::Time get_time() = 0;
};

#endif

#ifndef BASIC_TIMELINE_H
#define BASIC_TIMELINE_H

#include <SFML/System/Time.hpp>

class BasicTimeline {
 public:
  virtual sf::Time get_time() = 0;
};

#endif

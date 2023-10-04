#ifndef TIMELINE_HPP
#define TIMELINE_HPP

#include <SFML/System/Time.hpp>
#include <mutex>

#include "../BasicTimeline/BasicTimeline.hpp"

class Timeline : virtual public BasicTimeline {
  BasicTimeline* anchor;
  sf::Time reference_time;
  sf::Time pause_time;
  float time_ratio;
  bool paused;
  std::mutex mutex;

 public:
  Timeline(BasicTimeline* anchor, float time_ratio = 1.0);
  sf::Time get_time();
  void set_ratio(float time_ratio);
  float get_ratio();
  void pause();
  void unpause();
  bool is_paused();
};

#endif

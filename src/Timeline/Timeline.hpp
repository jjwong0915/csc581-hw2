#ifndef TIMELINE_HPP
#define TIMELINE_HPP

#include <SFML/System.hpp>
#include <mutex>

class Timeline {
  Timeline* anchor;  // nullptr: global time, other: local time
  sf::Time reference_time;
  sf::Time pause_time;
  float time_ratio;
  bool paused;
  std::mutex mutex;

 public:
  Timeline(Timeline* anchor = nullptr, float time_ratio = 1.0);
  sf::Time get_time();
  void set_ratio(float time_ratio);
  float get_ratio();
  void pause();
  void unpause();
  bool is_paused();

  static sf::Clock global_clock;
};

#endif

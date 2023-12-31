#include "Timeline.hpp"

#include <SFML/System.hpp>
#include <iostream>
#include <mutex>

sf::Clock Timeline::global_clock;

Timeline::Timeline(Timeline* anchor, float time_ratio) {
  this->anchor = anchor;
  if (anchor != nullptr) {
    this->reference_time = anchor->get_time();
  }
  this->time_ratio = time_ratio;
  this->paused = false;
}

sf::Time Timeline::get_time() {
  if (this->anchor == nullptr) {
    return this->global_clock.getElapsedTime();
  }
  std::lock_guard<std::mutex> lock(this->mutex);
  if (this->paused) {
    return (this->pause_time - this->reference_time) * this->time_ratio;
  }
  return (this->anchor->get_time() - this->reference_time) * this->time_ratio;
}

void Timeline::set_ratio(float time_ratio) {
  std::lock_guard<std::mutex> lock(this->mutex);
  if (this->paused) {
    this->reference_time =
        this->pause_time - (this->pause_time - this->reference_time) /
                               (time_ratio / this->time_ratio);
  } else {
    this->reference_time = this->anchor->get_time() -
                           (this->anchor->get_time() - this->reference_time) /
                               (time_ratio / this->time_ratio);
  }
  this->time_ratio = time_ratio;
}

float Timeline::get_ratio() {
  std::lock_guard<std::mutex> lock(this->mutex);
  return this->time_ratio;
}

void Timeline::pause() {
  std::lock_guard<std::mutex> lock(this->mutex);
  if (this->paused) {
    return;
  }
  this->pause_time = this->anchor->get_time();
  this->paused = true;
}

void Timeline::unpause() {
  std::lock_guard<std::mutex> lock(this->mutex);
  if (!this->paused) {
    return;
  }
  this->reference_time += (this->anchor->get_time() - this->pause_time);
  this->paused = false;
}

bool Timeline::is_paused() {
  std::lock_guard<std::mutex> lock(this->mutex);
  return this->paused;
}

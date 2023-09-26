#include "SfmlTimeline.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

sf::Time SfmlTimeline::get_time() { return this->clock.getElapsedTime(); }

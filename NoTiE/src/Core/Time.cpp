#include "Time.h"
#include <SFML/System/Clock.hpp>

float Time::DeltaTimeInSeconds = 0;
sf::Clock Time::Timer = sf::Clock();

void Time::ResetTimer()
{
	DeltaTimeInSeconds = Timer.restart().asSeconds();
}

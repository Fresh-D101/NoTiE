#pragma once

namespace sf
{
	class Clock;
}

class Time
{
public:
	friend class Notie;

	static float DeltaTime()
	{
		return DeltaTimeInSeconds;
	}
	
private:
	static void ResetTimer();

	static float DeltaTimeInSeconds;
	static sf::Clock Timer;
};
#pragma once

#include <chrono>

namespace util {

/**
 * @brief Class representing a duration of time
 * 
 */
class time {
public:
	/// By default, initializes to length 0.
	time();

	/**
	 * @brief Create a new time duration given seconds
	 * 
	 * @param s
	 * @return time 
	 */
	static time seconds(float s);
	/**
	 * @brief Create a new time duration, given milliseconds
	 * 
	 * @param ms 
	 * @return time 
	 */
	static time milliseconds(unsigned long ms);
	/**
	 * @brief Create a new time duration, given minutes
	 * 
	 * @param m 
	 * @return time 
	 */
	static time minutes(float m);

	/**
	 * @brief Convert this time duration into seconds
	 * 
	 * @return float 
	 */
	float as_seconds() const;
	/**
	 * @brief Convert this time duration into milliseconds
	 * 
	 * @return unsigned long 
	 */
	unsigned long as_milliseconds() const;
	/**
	 * @brief Convert this time duration into minutes
	 * 
	 * @return float 
	 */
	float as_minutes() const;

	/// Add two times together
	time operator+(const time& other);
	/// Subtract two time durations
	time operator-(const time& other);
	/// Add one duration to this
	time& operator+=(const time& other);
	/// Subtract a duration of time from this
	time& operator-=(const time& other);

	/// Check if one duration is larger than another
	bool operator>(const time& other);
	/// Check if one duration is smaller than another
	bool operator<(const time& other);
	/// Check if one duration is greater than or equal to another
	bool operator>=(const time& other);
	/// Check if one duration is smaller than or equal to another
	bool operator<=(const time& other);

	/// Check if two durations are exactly equal
	bool operator==(const time& other);
	/// Check if two durations aren't exactly equal
	bool operator!=(const time& other);

private:
	/// private constructor for initializing with ms
	time(unsigned long ms);

	/// internally store duration as milliseconds
	unsigned long m_ms;
};

/**
 * @brief Class to measure the passage of time
 * 
 */
class clock {
public:
	/**
	 * @remarks Always starts the clock. 
	 * 
	 */
	clock();

	/**
	 * @brief Restart the clock.
	 * 
	 * @return time The time elapsed since last restart
	 */
	time restart();
	/**
	 * @brief Get the elapsed time since the last restart()
	 * 
	 * @return time 
	 */
	time elapsed() const;

private:
	/// start point of the clock
	std::chrono::time_point<std::chrono::steady_clock> m_start;
};

}

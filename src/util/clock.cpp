#include "util/clock.hpp"

#include <cmath>

namespace util {

time::time()
	: m_ms(0) {
}

time::time(unsigned long ms)
	: m_ms(ms) {
}

time time::seconds(float s) {
	return time(s * 1000);
}

time time::milliseconds(unsigned long ms) {
	return time(ms);
}

time time::minutes(float m) {
	return time(m * 60 * 1000);
}

float time::as_seconds() const {
	return m_ms / 1000.f;
}

unsigned long time::as_milliseconds() const {
	return m_ms;
}

float time::as_minutes() const {
	return m_ms / (1000.f * 60.f);
}

time time::operator+(const time& other) {
	return time(other.m_ms + m_ms);
}

time time::operator-(const time& other) {
	return time(other.m_ms - m_ms);
}

time& time::operator+=(const time& other) {
	m_ms += other.m_ms;
	return *this;
}

time& time::operator-=(const time& other) {
	m_ms -= other.m_ms;
	return *this;
}

bool time::operator>(const time& other) {
	return m_ms > other.m_ms;
}

bool time::operator<(const time& other) {
	return m_ms < other.m_ms;
}

bool time::operator>=(const time& other) {
	return m_ms >= other.m_ms;
}

bool time::operator<=(const time& other) {
	return m_ms <= other.m_ms;
}

bool time::operator==(const time& other) {
	return m_ms == other.m_ms;
}

bool time::operator!=(const time& other) {
	return m_ms != other.m_ms;
}

clock::clock() {
	m_start = std::chrono::steady_clock::now();
}

time clock::restart() {
	auto now = std::chrono::steady_clock::now();
	auto dur = now - m_start;
	m_start	 = now;
	return time::milliseconds(dur.count() / (1000 * 1000));
}

time clock::elapsed() const {
	auto now = std::chrono::steady_clock::now();
	auto dur = now - m_start;
	return time::milliseconds(dur.count() / (1000 * 1000));
}

}

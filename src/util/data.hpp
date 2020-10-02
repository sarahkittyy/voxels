#pragma once

#include <any>
#include <string>
#include <unordered_map>

#include "util/clock.hpp"

namespace util {

// type that can store key-value data pairs
class data {
public:
	data();

	template <typename T>
	data& set(const std::string& key, T value) {
		m_kvs[key] = value;
		return *this;
	}

	template <typename T>
	T get(const std::string& key) const {
		return std::any_cast<T>(m_kvs.at(key));
	}

	template <typename T>
	T get(const std::string& key, T fallback) const {
		try {
			return std::any_cast<T>(m_kvs.at(key));
		} catch (std::bad_any_cast& v) {
			return fallback;
		}
	}

private:
	std::unordered_map<std::string, std::any> m_kvs;
};

}

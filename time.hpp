#pragma once

#include <iomanip>
#include <ostream>
#include <regex>
#include <string>

class Time
{
	std::size_t h{ 0 };
	std::size_t m{ 0 };

public:
	Time() = default;

	Time(std::size_t _h, std::size_t _m) : h{ _h }, m{ _m } {}

	Time(const std::string& str)
	{
		std::regex  reg{ "^(2[0-3]|[01][0-9]):([0-5][0-9])$" };
		std::smatch match;
		if (!std::regex_match(str, match, reg)) {
			throw std::runtime_error{ "Error time format" };
		}
		h = std::stol(match[1].str());
		m = std::stol(match[2].str());
	}

	Time& operator+=(const Time& t)
	{
		h += t.h + (m + t.m) / 60U;
		m = (m + t.m) % 60U;
		return *this;
	}

	Time operator+(const Time& t) const
	{
		return Time{ *this } += t;
	}

	Time operator-(const Time& t) const
	{
		return Time{ h - t.h - (m < t.m ? 1 : 0), m < t.m ? 60U + m - t.m : m - t.m };
	}

	bool operator<(const Time& t) const
	{
		return (h * 60U + m) < (t.h * 60U + t.m);
	}

	std::size_t hour() const
	{
		return h;
	}

	std::size_t minute() const
	{
		return m;
	}

	~Time() = default;
};

std::ostream& operator<<(std::ostream& os, const Time& t)
{
	os << std::setw(2) << std::setfill('0') << t.hour() << ':' << std::setw(2) << std::setfill('0') << t.minute();
	return os;
}
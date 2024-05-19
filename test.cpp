#include <fstream>
#include <iostream>
#include <tuple>

#include "club.hpp"

enum class event : std::size_t
{
	ID1 = 1,
	ID2 = 2,
	ID3 = 3,
	ID4 = 4
};

static constexpr char delim{ '\n' };

std::pair<Time, Time> parse_schedule(const std::string& str)
{
	std::regex  reg{ R"(^(.{5})\s(.{5})$)" };
	std::smatch match;
	if (!std::regex_match(str, match, reg)) {
		throw std::runtime_error{ "Error schedule format" };
	}
	return std::make_pair(match[1].str(), match[2].str());
}

int main(int argc, const char* argv[])
{
	if (argc < 2) {
		std::cout << "No data file" << std::endl;
		return -1;
	}
	std::string str;
	const char* path = argv[1];
	try {
		std::ifstream dat{ path };
		std::getline(dat, str, delim);
		std::size_t count = std::stoul(str);
		std::getline(dat, str, delim);
		const auto [start, stop] = parse_schedule(str);
		std::getline(dat, str, delim);
		std::size_t price = std::stoul(str);
		club        cl{ count, start, stop, price };

		std::regex  reg_event{ R"(^(.{5})\s([1-4])\s(.+)$)" };
		std::regex  reg_table{ R"(^(.+)\s([1-9][0-9]*)$)" };
		std::smatch match;
		auto        match_name = [&match](const std::string& str) {
			std::regex reg_name{ "^([a-z0-9-_]+)$" };
			if (!std::regex_match(str, match, reg_name)) {
				throw std::runtime_error{ "Error client name format" };
			}
			return match[0].str();
		};
		while (std::getline(dat, str, delim)) {
			if (!std::regex_match(str, match, reg_event)) {
				throw std::runtime_error{ "Error event format" };
			}
			const Time        time{ match[1].str() };
			const std::string body{ match[3].str() };
			switch (static_cast<event>(std::stoul(match[2].str()))) {
			case event::ID1:
				cl.arrive(time, match_name(body));
				break;
			case event::ID2:
				if (!std::regex_match(body, match, reg_table)) {
					throw std::runtime_error{ "Error table format" };
				}
				cl.seat(time, match_name(match[1].str()), std::stoul(match[2].str()));
				break;
			case event::ID3:
				cl.await(time, match_name(body));
				break;
			case event::ID4:
				cl.gone(time, match_name(body));
				break;
			}
		}

		cl.work();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << " on string " << str << std::endl;
	}
}
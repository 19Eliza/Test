#pragma once

#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#include "time.hpp"

using table_t = std::size_t;
using client_t = std::string;

class club
{
	table_t releaseTable(const Time& time, const client_t& client)
	{
		auto it = clients_on_table.find(client);
		if (it != clients_on_table.end()) {
			auto       release_table = tables.find(it->second);
			const auto interval = time - release_table->second;
			auto& [summP, summT] = amount_on_table.at(release_table->first);
			summP += price * (interval.hour() + (interval.minute() > 0 ? 1 : 0));
			summT += interval;
			clients_on_table.erase(it);
			const auto table = release_table->first;
			tables.erase(release_table);
			return table;
		}
		return 0;
	}

public:
	club(const std::size_t& c, const Time& b, const Time& e, const std::size_t& p) : count{ c }, begin{ b }, end{ e }, price{ p }
	{
		for (std::size_t i = 1; i <= count; ++i) {
			amount_on_table[i] = std::make_pair(0, Time{});
		}
	}

	void work()
	{
		std::cout << begin << std::endl;
		std::cout << ss.str();
		while (!clients_on_table.empty()) {
			auto it = clients_on_table.begin();
			std::cout << end << " 11 " << it->first << std::endl;
			releaseTable(end, it->first);
		}
		std::cout << end << std::endl;
		for (const auto& [table, amount] : amount_on_table) {
			std::cout << table << ' ' << amount.first << ' ' << amount.second << std::endl;
		}
	}

	void arrive(const Time& time, const client_t& client)
	{
		ss << time << " 1 " << client << std::endl;
		if (time < begin) {
			ss << time << " 13 NotOpenYet" << std::endl;
			return;
		}
		if (clients.find(client) != clients.end()) {
			ss << time << " 13 YouShallNotPass" << std::endl;
			return;
		}


		clients.insert(client);
	}

	void seat(const Time& time, const client_t& client, table_t table)
	{
		ss << time << " 2 " << client << ' ' << table << std::endl;
		if (clients.find(client) == clients.end()) {
			ss << time << " 13 ClientUnknown" << std::endl;
			return;
		}
		if (tables.find(table) != tables.end()) {
			ss << time << " 13 PlaceIsBusy" << std::endl;
			return;
		}
		releaseTable(time, client);
		clients_on_table.emplace(client, table);
		tables.emplace(table, time);
	}

	void await(const Time& time, const client_t& client)
	{
		ss << time << " 3 " << client << std::endl;
		if (tables.size() < count) {
			ss << time << " 13 ICanWaitNoLonger!" << std::endl;
			return;
		}
		if (que.size() > count) {
			clients.erase(client);
			ss << time << " 11 " << client << std::endl;
			return;
		}
		que.push(client);
	}

	void gone(const Time& time, const client_t& client)
	{
		ss << time << " 4 " << client << std::endl;
		if (clients.find(client) == clients.end()) {
			ss << time << " 13 ClientUnknown" << std::endl;
			return;
		}
		clients.erase(client);
		auto table = releaseTable(time, client);
		if (que.size() > 0) {
			auto& next = que.front();
			clients_on_table.emplace(next, table);
			tables.emplace(table, time);
			que.pop();
			ss << time << " 12 " << next << ' ' << table << std::endl;
		}
	}

private:
	const std::size_t count;
	const Time        begin;
	const Time        end;
	const std::size_t price;

	std::stringstream                               ss;
	std::unordered_set<client_t>                    clients;
	std::unordered_map<table_t, Time>               tables;
	std::map<client_t, table_t>                     clients_on_table;
	std::map<table_t, std::pair<std::size_t, Time>> amount_on_table;
	std::queue<client_t>                            que;
};

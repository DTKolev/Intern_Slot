#pragma once

#include "GameObjects/CommonManager.hpp"
#include <iostream>
#include <string>

/**
 * @brief Represents the record of a single reeling outcome
 *
 * This class is meant to be filled with data and its contents
 * written to a save file at the end of each reeling
 */
class Log {

private:
	int bet;
	int win;
	std::string game_mode;

	friend std::ostream& operator<<(std::ostream& out_str, const Log& log);
public:
	Log(const CommonManager& common_manager, int win);
};

std::ostream& operator<<(std::ostream& out_str, const Log& log);

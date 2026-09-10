#pragma once

#include "GameObjects/CommonManager.hpp"
#include <iostream>
#include <string>

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

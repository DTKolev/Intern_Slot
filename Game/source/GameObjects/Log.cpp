#include "GameObjects/Log.hpp"

void Log::GetLogData(const CommonManager& common_manager, int win) {

	bet = common_manager.bet;
	this->win = win;
	game_mode = "Regular";

	if (common_manager.extra_reel_mode) game_mode = "Extra Reel";
	else if (common_manager.free_spins_mode) game_mode = "Free Spins";
}

std::ostream& operator<<(std::ostream& out_str, const Log& log) {

	out_str << "Bet: " << log.bet << ", Win: " << log.win << ", Mode: " << log.game_mode;
	
	return out_str;
}

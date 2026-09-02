#include "../headers/Singleton_TimeManager.hpp"
#include "SDL3/SDL.h"

using namespace single;

TimeManager::TimeManager() : 
    frame_begin{SDL_GetPerformanceCounter()}, frame_end{0}, 
    performance_frequency{SDL_GetPerformanceFrequency()},
    delta_t{0.016}
{};



void TimeManager::CalcuateDeltaTime() {

    frame_end = SDL_GetPerformanceCounter();
    delta_t = (double)(frame_end - frame_begin) / (double)(performance_frequency);
    frame_begin = frame_end;
}



auto TimeManager::DeltaTime() const -> double {return delta_t;}
#pragma once 

#include <cstdint>

namespace single {

// Time Manager is a support class that is only
// tasked with calculating and storing the 
// delta time between frames

class TimeManager {

    private:
    uint64_t frame_begin;
    uint64_t frame_end;
    uint64_t performance_frequency;

    double delta_t;

    public:
    TimeManager();

    void CalcuateDeltaTime();
    auto DeltaTime() const -> double;
};
} // end of namespace single

#pragma once 

#include <cstdint>

namespace single {

    class TimeManager {

        private:
        uint64_t frame_begin;
        uint64_t frame_end;
        uint64_t performance_frequency;

        double delta_t;

        public:
        TimeManager();

        void CalcuateDeltaTime();
        double DeltaTime() const;
    };
}
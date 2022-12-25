#pragma once

#include <chrono>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

class TickTimer {
    int m_tick          = 0;
    int m_tick_interval = 1;

  public:
    explicit TickTimer(int interval) : m_tick_interval(interval) {}

    auto tick() -> bool {
        if (++m_tick == m_tick_interval) {
            m_tick = 0;
            return true;
        }
        return false;
    }
};

class Timer {
    steady_clock::time_point m_next_time;
};

class FpsCounter {
    steady_clock::time_point m_last;
    milliseconds m_update_interval;
    int m_ticks;

  public:
    FpsCounter(int millis) : m_last(steady_clock::now()), m_ticks(0), m_update_interval(millis) {}

    void tick(float* fps) {
        ++m_ticks;
        auto elapsed = steady_clock::now() - m_last;
        if (elapsed > m_update_interval) {
            *fps     = m_ticks * 1.0e9 / duration_cast<std::chrono::nanoseconds>(elapsed).count();
            m_last  = steady_clock::now();
            m_ticks = 0;
        }
    }
};

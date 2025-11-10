#include "pch.h"
#include "Timer.h"

Timer::Timer()
	:m_prevTP{std::chrono::high_resolution_clock::now()}, m_dt{0.016s}
{
}

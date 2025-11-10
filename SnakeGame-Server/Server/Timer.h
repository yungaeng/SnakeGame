#pragma once

class Timer {
private:
	std::chrono::high_resolution_clock::time_point		m_prevTP;
	std::chrono::duration<float>						m_dt;

public:
	Timer();

public:
	inline void Update()
	{
		const auto curTP = std::chrono::high_resolution_clock::now();
		m_dt = curTP - m_prevTP;
		m_prevTP = curTP;
	}
	const auto GetDT() const noexcept { return m_dt.count(); };
};
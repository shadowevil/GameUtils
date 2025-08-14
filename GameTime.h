#pragma once
#include <chrono>
#include <array>
#include <string_view>
#include <sstream>
#include <iomanip>
#include <functional>
#include <vector>
#include <memory>

using namespace std::chrono_literals;
using namespace std::chrono;

class GameTime
{
private:
	GameTime()
	{
		m_startTime = high_resolution_clock::now();
		m_lastTime = m_startTime;
	}
public:
	static GameTime& GetInstance()
	{
		static GameTime instance;
		return instance;
	}

	void Update()
	{
		auto currentTime = high_resolution_clock::now();
		m_deltaTime = currentTime - m_lastTime;
		m_lastTime = currentTime;
		// Update game logic with deltaTime if needed
	}

	template<typename DurationType = std::chrono::duration<float>>
	static inline DurationType GetDeltaTime() {
		return std::chrono::duration_cast<DurationType>(GetInstance().m_deltaTime);
	}

	template<typename DurationType = std::chrono::duration<float>>
	static inline DurationType GetTotalTime() {
		return std::chrono::duration_cast<DurationType>(GetInstance().m_lastTime - GetInstance().m_startTime);
	}

	template<typename DurationType = std::chrono::duration<float>>
	static inline DurationType GetElapsedTime() {
		return std::chrono::duration_cast<DurationType>(high_resolution_clock::now() - GetInstance().m_startTime);
	}

	template<typename DurationType = std::chrono::duration<float>>
	static inline bool HasElapsed(DurationType initial, DurationType duration)
	{
		return GetTotalTime<DurationType>() - initial >= duration;
	}

private:
	high_resolution_clock::time_point m_startTime;
	high_resolution_clock::time_point m_lastTime;

	duration<float> m_deltaTime{ 0.0f };
};

class GameTimer
{
public:
	std::function<void()> OnElapsed; // Optional callback when timer elapses

public:
	enum class TimeUnit { Nanoseconds, Microseconds, Milliseconds, Seconds, Minutes, Hours, Days };
	static constexpr std::array<std::string_view, 7> TimeUnitNames = {
		"Nanoseconds", "Microseconds", "Milliseconds", "Seconds", "Minutes", "Hours", "Days"
	};

	struct DurationResult {
		float value;
		TimeUnit unit;

		constexpr std::string_view UnitString() const {
			return TimeUnitNames[static_cast<size_t>(unit)];
		}

		const std::string ToString() const {
			std::ostringstream oss;
			oss << std::fixed << std::setprecision(2) << value
				<< " " << TimeUnitNames[static_cast<size_t>(unit)];
			return oss.str();
		}
	};

public:
	template<typename Rep, typename Period>
	explicit GameTimer(duration<Rep, Period> dur, bool repeatTimer = false)
		: m_startTime(GameTime::GetTotalTime<duration<float>>()),
		m_duration(duration_cast<duration<float>>(dur)),
		m_repeat(repeatTimer)
	{
	}

	void Start()
	{
		if (!m_active)
		{
			m_startTime = GameTime::GetTotalTime<duration<float>>();
			m_active = true;
		}
	}

	void Resume()
	{
		if (!m_active)
			m_active = true;
	}

	void Stop()
	{
		m_active = false;
	}

	bool IsActive() const
	{
		return m_active;
	}

	bool HasElapsedOnce() const
	{
		return m_elapsedCount > 0;
	}

	bool IsRepeating() const
	{
		return m_repeat;
	}

	bool Elapsed()
	{
		if (!m_active)
			return false;

		if (GameTime::HasElapsed(m_startTime, m_duration)) {
			if (OnElapsed)
				OnElapsed();
			m_elapsedCount++;
			if (m_repeat)
				m_startTime = GameTime::GetTotalTime<std::chrono::duration<float>>();
			else
				m_active = false;
			return true;
		}
		return false;
	}

	void Reset()
	{
		m_startTime = GameTime::GetTotalTime<std::chrono::duration<float>>();
		m_active = true;
	}

	DurationResult Duration() const {
		using namespace std::chrono;
		auto ns = duration_cast<nanoseconds>(m_duration).count();

		if (ns < 1'000)
			return { static_cast<float>(ns), TimeUnit::Nanoseconds };

		auto us = ns / 1'000.0f;
		if (us < 1'000)
			return { us, TimeUnit::Microseconds }; // you'd need to add Microseconds to your enum if you want this

		auto ms = ns / 1'000'000.0f;
		if (ms < 1'000)
			return { ms, TimeUnit::Milliseconds };

		auto s = ms / 1'000.0f;
		if (s < 60)
			return { s, TimeUnit::Seconds };

		auto min = s / 60.0f;
		if (min < 60)
			return { min, TimeUnit::Minutes };

		auto h = min / 60.0f;
		if (h < 24)
			return { h, TimeUnit::Hours };

		return { h / 24.0f, TimeUnit::Days };
	}

private:
	std::chrono::duration<float> m_startTime;
	std::chrono::duration<float> m_duration;
	bool m_active = false;
	bool m_repeat = false;

	uint64_t m_elapsedCount = 0;
};

class TimerCollection
{
public:
	enum class StartMode { ManualStart, StartImmediately };

public:
	void CheckTimers()
	{
		m_timers.erase(
			std::remove_if(m_timers.begin(), m_timers.end(),
				[](const std::unique_ptr<GameTimer>& t) {
					return t->Elapsed() && !t->IsRepeating();
				}),
			m_timers.end()
		);
	}

	GameTimer* AddTimer(GameTimer timer, StartMode start = StartMode::ManualStart)
	{
		m_timers.push_back(std::make_unique<GameTimer>(std::move(timer)));
		if (start == StartMode::StartImmediately)
			m_timers.back()->Start();
		return m_timers.back().get();
	}

	void RemoveTimer(GameTimer* timer)
	{
		m_timers.erase(
			std::remove_if(m_timers.begin(), m_timers.end(),
				[timer](const std::unique_ptr<GameTimer>& t) {
					return t.get() == timer;
				}),
			m_timers.end()
		);
	}

	void ClearTimers()
	{
		m_timers.clear();
	}

private:
	std::vector<std::unique_ptr<GameTimer>> m_timers;
};
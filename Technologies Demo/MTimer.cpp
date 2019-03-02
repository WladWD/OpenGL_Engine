#include "MTimer.h"


long long Time::Timer_Query::GetCycleCount()
{
	long long count_value;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&count_value));
	return count_value;
}

void Time::Timer_Query::Calibrate()
{
	long long frequency;
	//QueryPerformanceCounter(&val);
	if (!QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&frequency)))
	{
		MessageBox(NULL, L"Hieght Resolution Time is Not Support", L"Error", MB_OK || MB_ICONERROR);
		throw L"Hieght Resolution Time is Not Support";
	}

	this->frequency = 1.0l / (frequency);// / 1000.0;
	this->frequency *= 1000.0l;
}

Time::Timer_Query::Timer_Query()
{
	delta_time_tick = begin_time = cur_time = prev_time = stop_time = pause_time = 0;
	delta_time = 0.0l;
	stop_pause = false;
	Calibrate();
}

Time::Timer_Query::~Timer_Query()
{
}

long double Time::Timer_Query::GetDeltaTime()
{
	if (stop_pause) return 0.0;
	else return delta_time;
}

void Time::Timer_Query::Stop()
{
	if (!stop_pause)
	{
		stop_time = GetCycleCount();
		stop_pause = true;
	}
}

void Time::Timer_Query::Start()
{
	if (stop_pause)
	{
		stop_pause = false;
		long long m_cur_time = GetCycleCount();
		pause_time += m_cur_time - stop_time;
		stop_time = 0LL;
		prev_time = cur_time = m_cur_time;

		delta_time = 0.0l;
	}
}

void Time::Timer_Query::Reset()
{
	begin_time = prev_time = cur_time = GetCycleCount();
	stop_pause = false;
}

void Time::Timer_Query::Tick()
{
	if (!stop_pause)
	{
		long long tmp = GetCycleCount();

		prev_time = cur_time;
		cur_time = tmp;

		delta_time_tick = cur_time - prev_time;
		delta_time = delta_time_tick * frequency;
	}
	else
	{
		delta_time_tick = 0LL;
		delta_time = 0.0l;
	}
}

long double Time::Timer_Query::GetTimeMilliSecond()
{
	if (stop_pause){return ((stop_time - pause_time) - begin_time) * frequency;}
	else{return ((cur_time - pause_time) - begin_time) * frequency;}
}
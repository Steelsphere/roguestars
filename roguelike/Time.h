#pragma once
#define TCOD_SDL2

#include <string>

class Time {
public:

	Time() {}
	Time(unsigned long long start_time);
	~Time();

	std::string format_time(const std::string& format);

	void pass_time(int seconds) { _timestamp += seconds; }

	// void pass_time(int year, int month, int day, int hour, int minute, int second);

private:
	unsigned long long _timestamp = 0;

	/*
	int _year;
	int _month;
	int _day;
	int _hour;
	int _minute;
	int _second;
	*/
};

#pragma once

#include <string>

class Time {
public:
	
	Time() {}
	Time(int year, int month, int day, int hour, int minute, int second);
	~Time();

	std::string format_time(const std::string& format);

	void pass_time(int year, int month, int day, int hour, int minute, int second);

private:
	int _year;
	int _month;
	int _day;
	int _hour;
	int _minute;
	int _second;
};


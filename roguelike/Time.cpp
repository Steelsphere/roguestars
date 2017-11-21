#include "Time.h"

#include <cmath>
#include <iostream>

#define SECONDS_IN_YEAR 31104000 // 30 DAY MONTHS!
#define SECONDS_IN_MONTH 2592000
#define SECONDS_IN_DAY 86400
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60

Time::Time(unsigned long long start) :
	_timestamp(start)
{

}

Time::~Time() {}

std::string Time::format_time(const std::string& format) {
	std::string str = "";
	bool processf = false;
	for (char c : format) {
		if (c == '%') {
			processf = true;
			continue;
		}
		if (processf) {
			std::string t;
			switch (c) {
			case '%':
				str.push_back('%');
				break;
			case 'Y': // Year
				str += std::to_string( (unsigned long long) std::floor(_timestamp / SECONDS_IN_YEAR));
				break;
			case 'M': // Month
				str += std::to_string(( ((unsigned long long) std::floor(_timestamp / SECONDS_IN_MONTH)) % 12) + 1);
				break;
			case 'D': // Day
				str += std::to_string( ((unsigned long long) std::floor(_timestamp / SECONDS_IN_DAY) % 30) + 1);
				break;
			case 'H': // Hour
				t = std::to_string(((unsigned long long) std::floor(_timestamp / SECONDS_IN_HOUR) % 24));
				if (t.size() == 1) t.insert(0, 1, '0');
				str += t;
				break;
			case 'm': // Minute
				t = std::to_string(((unsigned long long) std::floor(_timestamp / SECONDS_IN_MINUTE) % 60));
				if (t.size() == 1) t.insert(0, 1, '0');
				str += t;
				break;
			case 'S': // Second
				t = std::to_string(((unsigned long long) _timestamp) % 60);
				if (t.length() == 1) t.insert(0, 1, '0');
				str += t;
				break;
			}
			processf = false;
		}
		else {
			str.push_back(c);
		}
	}
	return str;
}

/*
void Time::pass_time(int year, int month, int day, int hour, int minute, int second) {
	int overflow;
	int next;
	
	_year += year;
	_month += month;
	_day += day;
	_hour += hour;
	_minute += minute;
	_second += second;

	for (int recheck = 1; recheck > 0; recheck--) {
		
		if (_month > 12) {
			overflow = _month % 12;
			next = _month / 12;
			_year += next;
			_month = overflow;

			recheck++;
		}

		if (_day > 30) {
			overflow = _day % 30;
			next = _day / 30;
			_month += next;
			_day = overflow;

			recheck++;
		}

		if (_hour > 24) {
			overflow = _hour % 24;
			next = _hour / 30;
			_day += next;
			_hour = overflow;

			recheck++;
		}

		if (_minute > 59) {
			overflow = _minute % 60;
			next = _minute / 60;
			_hour += next;
			_minute = overflow;

			recheck++;
		}

		if (_second > 60) {
			overflow = _second % 60;
			next = _second / 60;
			_minute += next;
			_second = overflow;

			recheck++;
		}

	}
}

*/
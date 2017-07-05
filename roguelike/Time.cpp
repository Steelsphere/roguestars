#include "Time.h"



Time::Time(int year, int month, int day, int hour, int minute, int second) :
	_year(year), _month(month), _day(day), _hour(hour), _minute(minute), _second(second)
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
			switch (c) {
			case '%':
				str.push_back('%');
				break;
			case 'Y':
				str.append(std::to_string(_year));
				break;
			case 'M':
				str.append(std::to_string(_month));
				break;
			case 'D':
				str.append(std::to_string(_day));
				break;
			case 'H':
				str.append(std::to_string(_hour));
				break;
			case 'm':
				str.append(std::to_string(_minute));
				break;
			case 'S':
				str.append(std::to_string(_second));
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
#pragma once

struct Vec2 {
	Vec2() : x(0), y(0) {}
	Vec2(int x, int y) : x(x), y(y) {}

	friend bool operator<(const Vec2& a, const Vec2& b) {
		return (a.x < b.x && a.y < b.y);
	}
	friend bool operator>(const Vec2& a, const Vec2& b) {
		return (a.x > b.x && a.y > b.y);
	}
	friend bool operator<=(const Vec2& a, const Vec2& b) {
		return (a.x <= b.x && a.y <= b.y);
	}
	friend bool operator>=(const Vec2& a, const Vec2& b) {
		return (a.x >= b.x && a.y >= b.y);
	}
	friend Vec2 operator-(const Vec2& a, const Vec2& b) {
		return Vec2(a.x - b.x, a.y - b.y);
	}
	friend Vec2 operator+(const Vec2& a, const Vec2& b) {
		return Vec2(a.x + b.x, a.y + b.y);
	}

	int x;
	int y;
};

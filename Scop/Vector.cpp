#include "Vector.h"
#include <iostream>

namespace mymath
{
	Vec2::Vec2()
		: _x(0), _y(0)
	{
	}

	Vec2::Vec2(float x, float y)
		: _x(x), _y(y)
	{
	}

	Vec2::Vec2(const Vec2& copy)
		: _x(copy._x), _y(copy._y)
	{
	}

	Vec2& Vec2::operator=(const Vec2& other)
	{
		_x = other._x;
		_y = other._y;
		return *this;
	}

	Vec2 Vec2::operator+(const Vec2& other) const
	{
		return Vec2(_x + other._x, _y + other._y);
	}

	Vec2 Vec2::operator-(const Vec2& other) const
	{
		return Vec2(_x - other._x, _y - other._y);
	}

	Vec2 Vec2::operator*(float scalar) const
	{
		return Vec2(_x * scalar, _y * scalar);
	}

	Vec2 Vec2::operator/(float scalar) const
	{
		return Vec2(_x / scalar, _y / scalar);
	}

	bool Vec2::operator==(const Vec2& other) const
	{
		return _x == other._x && _y == other._y;
	}

	bool Vec2::operator!=(const Vec2& other) const
	{
		return !(*this == other);
	}

	float Vec2::dot(const Vec2& other) const
	{
		return _x * other._x + _y * other._y;
	}

	float Vec2::length() const
	{
		return std::sqrt(_x * _x + _y * _y);
	}

	Vec2 Vec2::normalize() const
	{
		float len = length();
		if (len != 0.0f)
			return *this / len;
		else
			return *this;
	}


	Vec3::Vec3()
		:_x(0), _y(0)
	{
	}

	Vec3::Vec3(float scalar)
	{
		_x = scalar;
		_y = scalar;
		_z = scalar;
	}

	Vec3::Vec3(float x, float y, float z)
		: _x(x), _y(y), _z(z)
	{
	}

	Vec3::Vec3(const Vec3& copy)
		: _x(copy._x), _y(copy._y), _z(copy._z)
	{
	}

	Vec3& Vec3::operator=(const Vec3& copy)
	{
		_x = copy._x;
		_y = copy._y;
		_z = copy._z;

		return *this;
	}

	Vec3 Vec3::operator+(const Vec3& other) const
	{
		return Vec3(_x + other._x, _y + other._y, _z + other._z);
	}

	Vec3 Vec3::operator-(const Vec3& other) const
	{
		return Vec3(_x - other._x, _y - other._y, _z - other._z);
	}

	Vec3 Vec3::operator*(const float scalar) const
	{
		return Vec3(_x * scalar, _y * scalar, _z * scalar);
	}

	Vec3 Vec3::operator/(const float scalar) const
	{
		return Vec3(_x / scalar, _y / scalar, _z / scalar);
	}

	Vec3& Vec3::operator+=(const Vec3& other)
	{
		_x += other._x;
		_y += other._y;
		_z += other._z;

		return *this;
	}

	Vec3& Vec3::operator-=(const Vec3& other)
	{
		_x -= other._x;
		_y -= other._y;
		_z -= other._z;

		return *this;
	}

	bool Vec3::operator==(const Vec3& other) const
	{
		return _x == other._x && _y == other._y && _z == other._z;
	}

	bool Vec3::operator!=(const Vec3& other) const
	{
		return !(*this == other);
	}

	Vec3 Vec3::cross(const Vec3& other) const
	{
		return Vec3(_y * other._z - _z * other._y,
					_z * other._x - _x * other._z,
					_x * other._y - _y * other._x);
	}

	float Vec3::dot(const Vec3& other) const
	{
		return _x * other._x + _y * other._y + _z * other._z;
	}

	float Vec3::length() const
	{
		return std::sqrt(_x * _x +  _y * _y + _z * _z);
	}

	Vec3 Vec3::normalize() const
	{
		float len = length();
		if (len != 0) 
			return *this / len;
		else
			return *this;
	}


	Vec3 operator*(const float scalar, const Vec3& vec)
	{
		return Vec3(vec._x * scalar, vec._y * scalar, vec._z * scalar);
	}

}

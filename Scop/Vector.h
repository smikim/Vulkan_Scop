#pragma once
#include <cmath>

namespace mymath
{
	class Vec2 {
	public:
		float _x;
		float _y;

		Vec2();
		Vec2(float x, float y);
		Vec2(const Vec2& copy);
		Vec2& operator=(const Vec2& other);
		Vec2 operator+(const Vec2& other) const;
		Vec2 operator-(const Vec2& other) const;
		Vec2 operator*(float scalar) const;
		Vec2 operator/(float scalar) const;

		bool operator==(const Vec2& other) const;
		bool operator!=(const Vec2& other) const;
        // Dot product
        float dot(const Vec2& other) const;

        // Magnitude (length) of the vector
		float length() const;

        // Normalize the vector
		Vec2 normalize() const;
	};

	class Vec3 {
	public:
		float _x;
		float _y;
		float _z;

		Vec3();
		Vec3(float scalar);
		Vec3(float x, float y, float z);
		Vec3(const Vec3& copy);
		Vec3& operator=(const Vec3& copy);
		Vec3 operator+(const Vec3& other) const;
		Vec3 operator-(const Vec3& other) const;
		
		Vec3 operator*(const float scalar) const;

		Vec3 operator/(const float scalar) const;
		Vec3& operator+=(const Vec3& other);
		Vec3& operator-=(const Vec3& other);

		bool operator==(const Vec3& other) const;
		bool operator!=(const Vec3& other) const;

		Vec3 cross(const Vec3& other) const;
		float dot(const Vec3& other) const;
		float length() const;

		Vec3 normalize() const;
	};

	// 전역 연산자 함수 선언
	Vec3 operator*(const float scalar, const Vec3& vec);
}



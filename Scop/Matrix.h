#pragma once

#include <cmath>
#include <cstring>
#include <iostream>

#include "Vector.h"

namespace mymath
{
	class Mat4
	{
	public:
		float _m[16];

		Mat4();
		Mat4(const Mat4& other);
		Mat4(float x);
		Mat4& operator=(const Mat4& other);

		float& operator[](std::size_t index);
		const float& operator[](std::size_t index) const;

		Mat4&	operator+=(const Mat4& other);
		Mat4	operator+(const Mat4& other) const;
		Mat4&	operator-=(const Mat4& other);
		Mat4	operator-(const Mat4& other) const;
		Mat4&	operator*=(const Mat4& other);
		Mat4	operator*(const Mat4& other) const;

		Mat4&	operator*=(float scalar);
		Mat4	operator*(float scalar) const;
		Vec3	operator*(const Vec3& other) const;

		void print() const {
			for (int i = 0; i < 16; i++) {
				std::cout << _m[i] << " ";
				if ((i + 1) % 4 == 0) std::cout << std::endl;
			}
		}
	};

	Mat4	lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
	Mat4	lookAtGLM(const Vec3& eye, const Vec3& center, const Vec3& up);
	
	Mat4	perspective(float fov, float aspect_ratio, float near, float far);
	Mat4	perspectiveGLM(float fov, float aspect_ratio, float near, float far);
	
	Mat4	rotate(const Mat4& mat, float angle, const Vec3& axis);
	Mat4	scale(const Mat4& mat, const Vec3& scale);
	Mat4	translate(const Mat4& mat, const Vec3& dir);
}



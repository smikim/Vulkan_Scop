#include "Matrix.h"

namespace mymath
{
	Mat4::Mat4()
	{
		std::memset(_m, 0.0f, 16 * sizeof(float));
	}

	Mat4::Mat4(const Mat4& other)
	{
		for (std::size_t i = 0; i < 16; ++i) {
			_m[i] = other._m[i];
		}
	}

	Mat4::Mat4(float x)
	{
		for (std::size_t i = 0; i < 16; ++i) {
			if (i % 5 == 0)
				_m[i] = x;
			else
				_m[i] = 0;
		}
	}

	Mat4& Mat4::operator=(const Mat4& other)
	{
		for (std::size_t i = 0; i < 16; i++)
		{
			_m[i] = other._m[i];
		}
		return *this;
	}

	float& Mat4::operator[](std::size_t index)
	{
		if (index >= 16)
			return _m[0];
		
		return _m[index];
	}

	const float& Mat4::operator[](std::size_t index) const
	{
		if (index >= 16)
			return _m[0];

		return _m[index];
	}

	Mat4& Mat4::operator+=(const Mat4& other)
	{
		for (std::size_t i = 0; i < 16; i++) {
			_m[i] += other._m[i];
		}
		return *this;
	}

	Mat4 Mat4::operator+(const Mat4& other) const
	{
		Mat4 res;
		return res.operator+=(other);
	}

	Mat4& Mat4::operator-=(const Mat4& other)
	{
		for (std::size_t i = 0; i < 16; i++) {
			_m[i] -= other._m[i];
		}
		return *this;
	}

	Mat4 Mat4::operator-(const Mat4& other) const
	{
		Mat4 res;
		return res.operator-=(other);
	}

	Mat4& Mat4::operator*=(const Mat4& other)
	{
		Mat4 res;

		std::size_t row;
		std::size_t col;
		std::size_t k;


		// Çà·Ä °ö¼À ¼öÇà
		//for (row = 0; row < 4; ++row) {
		//	for (col = 0; col < 4; ++col) {
		//		for (k = 0; k < 4; ++k) {
		//			res._m[row + col * 4] += _m[row + k * 4] * other._m[k + 4 * col];
		//		}
		//	}
		//}

		for (std::size_t row = 0; row < 4; ++row) {
			for (std::size_t col = 0; col < 4; ++col) {
				res._m[row * 4 + col] = 0; // ÃÊ±âÈ­
				for (std::size_t k = 0; k < 4; ++k) {
					res._m[row * 4 + col] += _m[row * 4 + k] * other._m[k * 4 + col];
				}
			}
		}

		*this = res;
		return *this;
	}


	Mat4 Mat4::operator*(const Mat4& other) const
	{
		Mat4 result(*this);
		return result.operator*=(other);
		
	}

	Mat4& Mat4::operator*=(float scalar)
	{
		for (std::size_t i = 0; i < 16; i++)
		{
			_m[i] *= scalar;
		}
		return *this;
	}

	Mat4 Mat4::operator*(float scalar) const
	{
		Mat4 result(*this);
		return result.operator*=(scalar);
	}

	Vec3 Mat4::operator*(const Vec3& other) const
	{
		Vec3 res;

		res._x = _m[0] * other._x + _m[4] * other._y + _m[8] * other._z;
		res._y = _m[1] * other._x + _m[5] * other._y + _m[9] * other._z;
		res._z = _m[2] * other._x + _m[6] * other._y + _m[10] * other._z;

		return res;
	}


	Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
	{
		const Vec3 w = (center - eye).normalize();
		const Vec3 uu = w.cross(up).normalize();
		const Vec3 v = w.cross(uu);

		Mat4 res;
		res[0] = uu._x;
		res[4] = uu._y;
		res[8] = uu._z;
		res[12] = -uu.dot(eye);

		res[1] = v._x;
		res[5] = v._y;
		res[9] = v._z;
		res[13] = -v.dot(eye);

		res[2] = w._x;
		res[6] = w._y;
		res[10] = w._z;
		res[14] = -w.dot(eye);

		res[15] = 1;

		return res;
	}

	// https://www.3dgep.com/understanding-the-view-matrix/
	// https://medium.com/@carmencincotti/lets-look-at-magic-lookat-matrices-c77e53ebdf78
	// https://www.songho.ca/opengl/gl_camera.html
	Mat4 lookAtGLM(const Vec3& eye, const Vec3& center, const Vec3& up)
	{
		const Vec3 front = (eye - center).normalize();
		Vec3 u = up.normalize();
		const Vec3 right = up.cross(front).normalize();
		u = front.cross(right);

		Mat4 res;
		res[0] = right._x;
		res[4] = right._y;
		res[8] = right._z;
		res[12] = -right.dot(eye);

		res[1] = u._x;
		res[5] = u._y;
		res[9] = u._z;
		res[13] = -u.dot(eye);
		
		res[2] = front._x;
		res[6] = front._y;
		res[10] = front._z;
		res[14] = -front.dot(eye);
		
		res[15] = 1;

		return res;
	}


	// https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml


	Mat4 perspective(float fov, float aspect_ratio, float near, float far)
	{
		//std::cout << "fovy : " << fov << std::endl;
		//std::cout << "aspect_ratio : " << aspect_ratio << std::endl;
		//std::cout << "near : " << near << std::endl;
		//std::cout << "far : " << far << std::endl;

		const float f = std::tan(fov / 2.f);
		const float range = far - near;

		Mat4 res;

		res._m[0] = 1.f / (aspect_ratio * f);
		res._m[5] = 1.f / f;

		res._m[10] = far / (far - near);

	
		res._m[11] = 1.f;
		
	
		res._m[14] = -(far * near) / (far - near);


		return res;
	}

	Mat4 perspectiveGLM(float fov, float aspect_ratio, float near, float far)
	{

		const float f = std::tan(fov / 2.f);
		const float range = far - near;

		Mat4 res;

		res._m[0] = 1.f / (aspect_ratio * f);
		res._m[5] = 1.f / f;
		
		res._m[10] = -(far + near) / range;

		res._m[11] = -1;

		res._m[14] = -(2 * far * near) / range;


		return res;

		
	}

	// https://registry.khronos.org/OpenGL-Refpages/gl2.1/
	Mat4 rotate(const Mat4& mat, float angle, const Vec3& axis)
	{
		Vec3 normalizedAxis = axis.normalize();
		float rad = angle; // in radians
		float cosA = std::cos(rad);
		float sinA = std::sin(rad);
		float oneMinusCosA = 1.0f - cosA;

		// Calculate components of the rotation matrix
		float xx = normalizedAxis._x * normalizedAxis._x;
		float yy = normalizedAxis._y * normalizedAxis._y;
		float zz = normalizedAxis._z * normalizedAxis._z;
		float xy = normalizedAxis._x * normalizedAxis._y;
		float xz = normalizedAxis._x * normalizedAxis._z;
		float yz = normalizedAxis._y * normalizedAxis._z;
		float xSinA = normalizedAxis._x * sinA;
		float ySinA = normalizedAxis._y * sinA;
		float zSinA = normalizedAxis._z * sinA;

		Mat4 rotationMatrix{ 1.0 };
		rotationMatrix._m[0] = xx * oneMinusCosA + cosA;
		rotationMatrix._m[1] = xy * oneMinusCosA - zSinA;
		rotationMatrix._m[2] = xz * oneMinusCosA + ySinA;
		rotationMatrix._m[3] = 0;

		rotationMatrix._m[4] = xy * oneMinusCosA + zSinA;
		rotationMatrix._m[5] = yy * oneMinusCosA + cosA;
		rotationMatrix._m[6] = yz * oneMinusCosA - xSinA;
		rotationMatrix._m[7] = 0;

		rotationMatrix._m[8] = xz * oneMinusCosA - ySinA;
		rotationMatrix._m[9] = yz * oneMinusCosA + xSinA;
		rotationMatrix._m[10] = zz * oneMinusCosA + cosA;
		rotationMatrix._m[11] = 0;

		rotationMatrix._m[12] = 0;
		rotationMatrix._m[13] = 0;
		rotationMatrix._m[14] = 0;
		rotationMatrix._m[15] = 1.0f;

		// Apply the rotation to the input matrix
		return mat * rotationMatrix;
	}

	Mat4 scale(const Mat4& mat, const Vec3& scale)
	{
		Mat4 scaleMatrix(1.0f);
		scaleMatrix[0] *= scale._x;
		scaleMatrix[5] *= scale._y;
		scaleMatrix[10] *= scale._z;

		return mat * scaleMatrix;
	}
	
	Mat4 translate(const Mat4& mat, const Vec3& dir)
	{
		Mat4 trans(1.0f);

		trans._m[12] = dir._x;
		trans._m[13] = dir._y;
		trans._m[14] = dir._z;

		return mat * trans;
	}

}

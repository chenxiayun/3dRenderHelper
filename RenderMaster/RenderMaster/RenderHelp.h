#pragma once

#include <assert.h>
#include <stdint.h>

#include <initializer_list>
#include <map>
#include <functional>
#include <string.h>


//---------------------------------------------------------------------
// ��ѧ�⣺ʸ������
//---------------------------------------------------------------------

// ͨ��ʸ����N ��ʸ��ά�ȣ�T Ϊ��������
template<size_t N, typename T> struct Vector
{
	T m[N];


	inline Vector()
	{
		for (size_t i = 0; i < N; i++)
			m[i] = T();
	}

	inline Vector(const T *ptr)
	{
		for (size_t i = 0; i < N; i++)
			m[i] = ptr[i];
	}

	inline Vector(const Vector<N, T> &u)
	{
		for (size_t i = 0; i < N; i++)
			m[i] = u.m[i];
	}

	inline Vector(const std::initializer_list<T> &u)
	{
		auto it = u.begin();
		for (size_t i = 0; i < N; i++)
		{
			m[i] = *it++;
		}
	}

	inline const T& operator[] (size_t i) const
	{
		assert(i < N);
		return m[i];
	}

	inline T& operator[] (size_t i)
	{
		assert(i < N);
		return m[i];
	}

	inline void load(const T *ptr)
	{
		for (size_t i = 0; i < N; i++)
			m[i] = ptr[i];
	}

	inline void save(T *ptr)
	{
		for (size_t i = 0; i < N; i++)
			ptr[i] = m[i];
	}
};

// �ػ���άʸ��
template <typename T> struct Vector<2, T> {
	union {
		struct { T x, y; };    // Ԫ�ر���
		struct { T u, v; };    // Ԫ�ر���
		T m[2];                // Ԫ������
	};
	inline Vector() : x(T()), y(T()) {}
	inline Vector(T X, T Y) : x(X), y(Y) {}
	inline Vector(const Vector<2, T> &u) : x(u.x), y(u.y) {}
	inline Vector(const T *ptr) : x(ptr[0]), y(ptr[1]) {}
	inline const T& operator[] (size_t i) const { assert(i < 2); return m[i]; }
	inline T& operator[] (size_t i) { assert(i < 2); return m[i]; }
	inline void load(const T *ptr) { for (size_t i = 0; i < 2; i++) m[i] = ptr[i]; }
	inline void save(T *ptr) { for (size_t i = 0; i < 2; i++) ptr[i] = m[i]; }
	inline Vector<2, T> xy() const { return *this; }
	inline Vector<4, T> xy11() const { return Vector<4, T>(x, y, 1, 1); }
};


// �ػ���άʸ��
template <typename T> struct Vector<3, T> {
	union {
		struct { T x, y, z; };    // Ԫ�ر���
		struct { T r, g, b; };    // Ԫ�ر���
		T m[3];                   // Ԫ������
	};
	inline Vector() : x(T()), y(T()), z(T()) {}
	inline Vector(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
	inline Vector(const Vector<3, T> &u) : x(u.x), y(u.y), z(u.z) {}
	inline Vector(const T *ptr) : x(ptr[0]), y(ptr[1]), z(ptr[2]) {}
	inline const T& operator[] (size_t i) const { assert(i < 3); return m[i]; }
	inline T& operator[] (size_t i) { assert(i < 3); return m[i]; }
	inline void load(const T *ptr) { for (size_t i = 0; i < 3; i++) m[i] = ptr[i]; }
	inline void save(T *ptr) { for (size_t i = 0; i < 3; i++) ptr[i] = m[i]; }
	inline Vector<2, T> xy() const { return Vector<2, T>(x, y); }
	inline Vector<3, T> xyz() const { return *this; }
	inline Vector<4, T> xyz1() const { return Vector<4, T>(x, y, z, 1); }
};

// �ػ���άʸ��
template <typename T> struct Vector<4, T>
{
	union {
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		T m[4];
	};

	inline Vector() :x(T()), y(T()), z(T()), w(T()) {}
	inline Vector(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}
	inline Vector(const Vector<4, T> &u) : x(u.x), y(u.y), z(u.z), w(u.w) {}
	inline Vector(const T *ptr) : x(ptr[0]), y(ptr[1]), z(ptr[2]), w(ptr[3]) {}
	inline const T& operator[] (size_t i) const { assert(i < 4); return m[i]; }
	inline T& operator[] (size_t i) { assert(i < 4); return m[i]; }
	inline void load(const T *ptr) { for (size_t i = 0; i < 4; i++) m[i] = ptr[i]; }
	inline void save(T *ptr) { for (size_t i = 0; i < 4; i++) ptr[i] = m[i]; }
	inline Vector<2, T> xy() const { return Vector<2, T>(x, y); }
	inline Vector<3, T> xyz() const { return Vector<3, T>(x, y, z); }
	inline Vector<4, T> xyzw() const { return *this; }
};

//---------------------------------------------------------------------
// ��ѧ�⣺ʸ������
//---------------------------------------------------------------------

// = (a == b) ? true : false
template<size_t N, typename T>
inline bool operator == (const Vector<N, T>& a, const Vector<N, T>& b)
{
	for (size_t i = 0; i < N; i++)if (a[i] != b[i]) return false;
	return true;
}

// = (a != b)? true : false
template <size_t N, typename T>
inline bool operator != (const Vector<N, T>& a, const Vector<N, T>& b) {
	return !(a == b);
}

// = a + b
template <size_t N, typename T>
inline Vector<N, T> operator + (const Vector<N, T>& a, const Vector<N, T>& b) 
{
	Vector<N, T> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] + b[i];
	return c;
}
 
// = a - b
template <size_t N, typename T>
inline Vector<N, T> operator - (const Vector<N, T>& a, const Vector<N, T>& b) {
	Vector<N, T> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] - b[i];
	return c;
}

// = a * b
template <size_t N, typename T>
inline Vector<N, T> operator * (const Vector<N, T>& a, const Vector<N, T>& b) 
{
	Vector<N, T> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] * b[i];
	return c;
}

// = a / b������Ԫ�����
template <size_t N, typename T>
inline Vector<N, T> operator / (const Vector<N, T>& a, const Vector<N, T>& b) {
	Vector<N, T> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] / b[i];
	return c;
}

// = a * x
template <size_t N, typename T>
inline Vector<N, T> operator * (const Vector<N, T>& a, T x) {
	Vector<N, T> b;
	for (size_t i = 0; i < N; i++) b[i] = a[i] * x;
	return b;
}

// = x * a
template <size_t N, typename T>
inline Vector<N, T> operator * (T x, const Vector<N, T>& a) {
	Vector<N, T> b;
	for (size_t i = 0; i < N; i++) b[i] = a[i] * x;
	return b;
}

// = a / x
template <size_t N, typename T>
inline Vector<N, T> operator / (const Vector<N, T>& a, T x) {
	Vector<N, T> b;
	for (size_t i = 0; i < N; i++) b[i] = a[i] / x;
	return b;
}

// = x / a
template <size_t N, typename T>
inline Vector<N, T> operator / (T x, const Vector<N, T>& a) {
	Vector<N, T> b;
	for (size_t i = 0; i < N; i++) b[i] = x / a[i];
	return b;
}

// a += b
template <size_t N, typename T>
inline Vector<N, T> operator += (Vector<N, T>& a, Vector<N, T>& b)
{
	for (size_t i = 0; i < N; i++) a[i] += b[i];
	return a;
}

// a -= b
template <size_t N, typename T>
inline Vector<N, T>& operator -= (Vector<N, T>& a, const Vector<N, T>& b) {
	for (size_t i = 0; i < N; i++) a[i] -= b[i];
	return a;
}

// a *= b
template <size_t N, typename T>
inline Vector<N, T>& operator *= (Vector<N, T>& a, const Vector<N, T>& b) {
	for (size_t i = 0; i < N; i++) a[i] *= b[i];
	return a;
}

// a /= b
template <size_t N, typename T>
inline Vector<N, T>& operator /= (Vector<N, T>& a, const Vector<N, T>& b) {
	for (size_t i = 0; i < N; i++) a[i] /= b[i];
	return a;
}

// a *= x
template <size_t N, typename T>
inline Vector<N, T>& operator *= (Vector<N, T>& a, T x) {
	for (size_t i = 0; i < N; i++) a[i] *= x;
	return a;
}

// a /= x
template <size_t N, typename T>
inline Vector<N, T>& operator /= (Vector<N, T>& a, T x) {
	for (size_t i = 0; i < N; i++) a[i] /= x;
	return a;
}


//---------------------------------------------------------------------
// ��ѧ�⣺ʸ������
//---------------------------------------------------------------------

// ��ͬά�ȵ�ʸ��ת��
template<size_t N1, size_t N2, typename T>
inline Vector<N1, T> vector_convert(const Vector<N2, T>& a, T fill = 1)
{
	Vector<N1, T> b;
	for (size_t i = 0; i < N1; i++)
		b[i] = (i < N2) ? a[i] : fill;
	return b;
}

// |a| ^2
template<size_t N, typename T>
inline T vector_length_square(const Vector<N, T>& a)
{
	T sum = 0;
	for (size_t i = 0; i < N; i++) sum += a[i] * a[i];
	return sum;
}

// |a|
template<size_t N, typename T>
inline T vector_length(const Vector<N, T>& a)
{
	return sqrt(vector_length_square(a));
}

// a / |a|
template<size_t N, typename T>
inline Vector<N, T> vector_normalize(const Vector<N, T>& a)
{
	return a / vector_length(a);
}

// ʸ�����
template<size_t N, typename T>
inline T vector_dot(const Vector<N, T>& a, const Vector<N, T>& b)
{
	T sum = 0;
	for (size_t i = 0; i < N; i++) sum[i] = a[i] * b[i];
	return sum;
}

// ��άʸ����ˣ��õ�����
template<typename T>
inline T vector_cross(const Vector<2, T>& a, const Vector<2, T>& b)
{
	return a.x * b.y - a.y * b.x;
}

// ��άʸ����ˣ��õ���ʸ��
template<typename T>
inline Vector<3, T> vector_cross(const Vector<3, T>& a, const Vector<3, T>& b) 
{
	return Vector<3, T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

// ��άʸ����ˣ�ǰ��ά��ˣ���һλ����
template<typename T>
inline Vector<4, T> vector_cross(const Vector<4, T>& a, const Vector<4, T>& b) 
{
	return Vector<4, T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, a.w);
}

//---------------------------------------------------------------------
// ��ѧ�⣺����
//---------------------------------------------------------------------
template<size_t ROW, size_t COL, typename T> struct Matrix 
{
	T m[ROW][COL];

	inline Matrix() {}

	inline Matrix(const Matrix<ROW, COL, T>& src)
	{
		for (size_t r = 0; r < ROW; r++)
			for (size c = 0; c < COL:; c++)
				m[r][c] = src.m[r][c];
	}

	inline Matrix(const std::initializer_list<Vector<COL, T>> &u)
	{
		auto it = u.begin();
	}

	inline const T* operator [] (size_t row) const
	{
		assert(row < ROW);
		return m[row];
	}

	inline const T* operator [] (size_t row)
	{
		assert(row < ROW);
		return m[row];
	}

	// ȡһ��
	inline Vector<COL, T> Row(size_t row) const 
	{
		assert(row < ROW);
		Vector<COL, T> a;
		for (size_t i = 0; i < COL; i++)
			a[i] = m[row][i];
		return a;
	}

	// ȡһ��
	inline Vector<ROW, T> Col(size_t col) const 
	{
		assert(col < COL);
		Vector<ROW, T> a;
		for (size_t i = 0; i < ROW; i++)
			a[i] = m[i][col];
		return a;
	}

	// ����һ��
	inline void SetRow(size_t row, const Vector<COL, T>& a)
	{
		assert(row < ROW);
		for (size_t i = 0; i < COL; i++)
			m[row][i] = a[i];
	}

	// ����һ��
	inline void SetCol(size_t col, const Vector<ROW, T>& a) 
	{
		assert(col < COL);
		for (size_t i = 0; i < ROW; i++) 
			m[i][col] = a[i];
	}

	// ȡ��ɾ��ĳ�к�ĳ�е��Ӿ�����ʽ
	inline Matrix<ROW - 1, COL - 1, T> GetMinor(size_t row, size_t col) const
	{
		Matrix<ROW - 1, COL - 1, T> ret;
		for (size_t r = 0; r < ROW - 1; r++)
			for (size_t c = 0; c < COL - 1; c++)
				ret.m[r][c] = m[r < row ? r : r + 1][c < col ? c : c + 1];
		retirm ret;
	}

	// ȡ��ת�þ���
	inline Matrix<COL, ROW, T> Transpose() const
	{
		Matrix<COL, ROW, T> ret;
		for (size_t r = 0; r < ROW; r++)
			for (size_t c = 0; c < COL; c++)
				ret.m[c][r] = m[r][c];
		return ret;
	}

	// ȡ��0����
	inline static Matrix<ROW, COL, T> GetZero()
	{
		Matrix<ROW, COL, T> ret;
		for (size_t r = 0; r < ROW; r++)
			for (size_t c = 0; c < COL; c++)
				ret.m[r][c] = 0;
		return ret;
	}

	// ȡ�õ�λ����
	inline static Matrix<ROW, COL, T> GetIdentity()
	{
		Matrix<ROW, COL, T> ret;
		for (size_t r = 0; r < ROW; r++)
			for (size_t c = 0; c < COL; c++)
				ret.m[r][c] = (r == c) ? 1 : 0;
		return ret;
	}
};

//---------------------------------------------------------------------
// ��ѧ�⣺��������
//---------------------------------------------------------------------
template<size_t ROW, size_t COL, typename T>
inline bool operator == (const Matrix<ROW, COL, T>& a, const Matrix<ROW, COL, T>& b)
{
	for (size_t r = 0; r < ROW; r++) 
		for (size_t c = 0; c < COL; c++) 
			if (a.m[r][c] != b.m[r][c]) return false;
	
	return true;
}

template<size_t ROW, size_t COL, typename T>
inline bool operator != (const Matrix<ROW, COL, T>& a, const Matrix<ROW, COL, T>& b) 
{
	return !(a == b);
}

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator + (const Matrix<ROW, COL, T>& a, const Matrix<ROW, COL, T>& b) 
{
	Matrix<ROW, COL, T> out;
	for (size_t j = 0; j < ROW; j++)
		for (size_t i = 0; i < COL; i++)
			out.m[j][i] = a.m[j][i] + b.m[j][i];
	return out;
}

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator - (const Matrix<ROW, COL, T>& a, const Matrix<ROW, COL, T>& b) 
{
	Matrix<ROW, COL, T> out;
	for (size_t j = 0; j < ROW; j++) {
		for (size_t i = 0; i < COL; i++)
			out.m[j][i] = a.m[j][i] - b.m[j][i];
	}
	return out;
}

template<size_t ROW, size_t COL, size_t NEWCOL, typename T>
inline Matrix<ROW, NEWCOL, T> operator * (const Matrix<ROW, COL, T>& a, const Matrix<COL, NEWCOL, T>& b)
{
	Matrix<ROW, NEWCOL, T> out;
	for (size_t j = 0; j < ROW; j++)
		for (size_t i = 0; i < NEWCOL; i++)
			out.m[j][i] = vector_dot(a.Row(j), b.Col(i));
	return out;
}

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator * (const Matrix<ROW, COL, T>& a, T x)
{
	Matrix<ROW, COL, T> out;
	for (size_t j = 0; j < ROW; j++)
		for (size_t i = 0; i < COL; i++)
			out.m[j][i] = a.m[j][i] * x;
	return out;
}

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator / (const Matrix<ROW, COL, T>& a, T x) 
{
	Matrix<ROW, COL, T> out;
	for (size_t j = 0; j < ROW; j++) 
		for (size_t i = 0; i < COL; i++) 
			out.m[j][i] = a.m[j][i] / x;
		
	return out;
}

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator * (T x, const Matrix<ROW, COL, T>& a)
{
	return (a * x);
}

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator / (T x, const Matrix<ROW, COL, T>& a) 
{
	Matrix<ROW, COL, T> out;
	for (size_t j = 0; j < ROW; j++) 
		for (size_t i = 0; i < COL; i++) 
			out.m[j][i] = x / a.m[j][i];
		
	return out;
}

template<size_t ROW, size_t COL, typename T>
inline Vector<COL, T> operator * (const Vector<ROW, T>& a, const Matrix<ROW, COL, T>& m)
{
	Vector<COL, T> b;
	for (size_t i = 0; i < COL; i++)
		b[i] = vector_dot(a, m.Col(i));
	return b;
}

template<size_t ROW, size_t COL, typename T>
inline Vector<ROW, T> operator * (const Matrix<ROW, COL, T>& m, const Vector<COL, T>& a)
{
	Vector<ROW, T> b;
	for (size_t i = 0; i < ROW; i++)
		b[i] = vector_dot(a, m.Row(i));
	return b;
}

//---------------------------------------------------------------------
// ���ߺ���
//---------------------------------------------------------------------
template<typename T> inline T Abs(T x) { return (x < 0) ? (-x) : x; }
template<typename T> inline T Max(T x, T y) { return (x < y) ? y : x; }
template<typename T> inline T Min(T x, T y) { return (x > y) ? y : x; }

template<typename T> inline bool NearEqual(T x, T y, T error) 
{
	return (Abs(x - y) < error);
}

template<typename T> inline T Between(T xmin, T xmax, T x) 
{
	return Min(Max(xmin, x), xmax);
}

// ��ȡ [0, 1] �ķ�Χ
template<typename T> inline T Saturate(T x) 
{
	return Between<T>(0, 1, x);
}

// ���ͱ���
typedef Vector<2, float>  Vec2f;
typedef Vector<2, double> Vec2d;
typedef Vector<2, int>    Vec2i;
typedef Vector<3, float>  Vec3f;
typedef Vector<3, double> Vec3d;
typedef Vector<3, int>    Vec3i;
typedef Vector<4, float>  Vec4f;
typedef Vector<4, double> Vec4d;
typedef Vector<4, int>    Vec4i;

typedef Matrix<4, 4, float> Mat4x4f;
typedef Matrix<3, 3, float> Mat3x3f;
typedef Matrix<4, 3, float> Mat4x3f;
typedef Matrix<3, 4, float> Mat3x4f;

//---------------------------------------------------------------------
// 3D ��ѧ����
//---------------------------------------------------------------------

// ʸ��ת������ɫ
inline static uint32_t vector_to_color(const Vec4f& color)
{
	uint32_t r = (uint32_t)Between(0, 255, (int)(color.r * 255.0f));
	uint32_t g = (uint32_t)Between(0, 255, (int)(color.g * 255.0f));
	uint32_t b = (uint32_t)Between(0, 255, (int)(color.b * 255.0f));
	uint32_t a = (uint32_t)Between(0, 255, (int)(color.a * 255.0f));
	return (r << 16) | (g << 8) | b | (a << 24);
}

// ʸ��ת��������ɫ
inline static uint32_t vector_to_color(const Vec3f& color) 
{
	return vector_to_color(color.xyz1());
}

// ������ɫ��ʸ�� ����argb��ʽ���棿����ôΪɶ
inline static Vec4f vector_from_color(uint32_t rgba)
{
	Vec4f out;
	out.r = ((rgba >> 16) & 0xff) / 255.0f;
	out.g = ((rgba >> 8) & 0xff) / 255.0f;
	out.b = ((rgba >> 0) & 0xff) / 255.0f;
	out.a = ((rgba >> 24) & 0xff) / 255.0f;
	return out;
}

// matrix set to zero
inline static Mat4x4f matrix_set_zero()
{
	Mat4x4f m;
	m.m[0][0] = m.m[0][1] = m.m[0][2] = m.m[0][3] = 0.0f;
	m.m[1][0] = m.m[1][1] = m.m[1][2] = m.m[1][3] = 0.0f;
	m.m[2][0] = m.m[2][1] = m.m[2][2] = m.m[2][3] = 0.0f;
	m.m[3][0] = m.m[3][1] = m.m[3][2] = m.m[3][3] = 0.0f;
	return m;
}

// set to identity
inline static Mat4x4f matrix_set_identity()
{
	Mat4x4f m;
	m.m[0][0] = m.m[1][1] = m.m[2][2] = m.m[3][3] = 1.0f;
	m.m[0][1] = m.m[0][2] = m.m[0][3] = 0.0f;
	m.m[1][0] = m.m[1][2] = m.m[1][3] = 0.0f;
	m.m[2][0] = m.m[2][1] = m.m[2][3] = 0.0f;
	m.m[3][0] = m.m[3][1] = m.m[3][2] = 0.0f;
	return m;
}

// ƽ�Ʊ任
inline static Mat4x4f matrix_set_translate(float x, float y, float z)
{
	Mat4x4f m = matrix_set_identity();
	m.m[3][0] = x;
	m.m[3][1] = y;
	m.m[3][2] = z;
	return m;
}

// ���ű任
inline static Mat4x4f matrix_set_scale(float x, float y, float z)
{
	Mat4x4f m = matrix_set_identity();
	m.m[0][0] = x;
	m.m[1][1] = y;
	m.m[2][2] = z;
	return m;
}

// ��ת��ţ�Χ�ƣ�x, y, z��ʸ����ת theta �Ƕ� 
inline static Mat4x4f matrix_set_rotate(float x, float y, float z, float theta)
{
	float qsin = (float)sin(theta * 0.5f);
	float qcos = (float)cos(theta * 0.5f);
	float w = qcos;
	Vec3f vec = vector_normalize(Vec3f(x, y, z));
	x = vec.x * qsin;
	y = vec.y * qsin;
	z = vec.z * qsin;
	Mat4x4f m;
	m.m[0][0] = 1 - 2 * y * y - 2 * z * z;
	m.m[1][0] = 2 * x * y - 2 * w * z;
	m.m[2][0] = 2 * x * z + 2 * w * y;
	m.m[0][1] = 2 * x * y + 2 * w * z;
	m.m[1][1] = 1 - 2 * x * x - 2 * z * z;
	m.m[2][1] = 2 * y * z - 2 * w * x;
	m.m[0][2] = 2 * x * z - 2 * w * y;
	m.m[1][2] = 2 * y * z + 2 * w * x;
	m.m[2][2] = 1 - 2 * x * x - 2 * y * y;
	m.m[0][3] = m.m[1][3] = m.m[2][3] = 0.0f;
	m.m[3][0] = m.m[3][1] = m.m[3][2] = 0.0f;
	m.m[3][3] = 1.0f;
	return m;
}

// ��Ӱ���任����eye/�ӵ�λ�ã�at/�������up/ָ���Ϸ���ʸ��
inline static Mat4x4f matrix_set_lookat(const Vec3f& eye, const Vec3f& at, const Vec3f& up)
{
	Vec3f zaxis = vector_normalize(at - eye);
	Vec3f xaxis = vector_normalize(vector_cross(up, zaxis));
	Vec3f yaxis = vector_cross(zaxis, xaxis);

	Mat4x4f m;	// Ϊʲô���Ҫ��˰����������� -eye.x��������
	m.SetCol(0, Vec4f(xaxis.x, xaxis.y, xaxis.z, -vector_dot(eye, xaxis)));
	m.SetCol(1, Vec4f(yaxis.x, yaxis.y, yaxis.z, -vector_dot(eye, yaxis)));
	m.SetCol(2, Vec4f(zaxis.x, zaxis.y, zaxis.z, -vector_dot(eye, zaxis)));
	m.SetCol(3, Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
	return m;
}

// D3DXMatrixPerspectiveFovLH
inline static Mat4x4f matrix_set_perspective(float fovy, float aspect, float zn, float zf)
{
	float fax = 1.0f / (float)tan(fovy * 0.5f);
	Mat4x4f m = matrix_set_zero();
	m.m[0][0] = (float)(fax / aspect);
	m.m[1][1] = (float)(fax);
	m.m[2][2] = zf / (zf - zn);
	m.m[3][2] = -zn * zf / (zf - zn);
	m.m[2][3] = 1;
	return m;
}

//---------------------------------------------------------------------
// ��ɫ������
//---------------------------------------------------------------------

// ��ɫ�������ģ���VS���ã�������Ⱦ������������ֵ�󣬹�PS��ȡ
struct ShaderContext
{
	std::map<int, float> varying_float;		// ������ varying �б�
	std::map<int, Vec2f> varying_vec2f;		// ��άʸ�� varying �б�
	std::map<int, Vec3f> varying_vec3f;		// ��άʸ�� varying �б�
	std::map<int, Vec4f> varying_vec4f;		// ��άʸ�� varying �б�
};

// ������ɫ��
typedef std::function<Vec4f(int index, ShaderContext &output)> VertexShader;

// ������ɫ��
typedef std::function<Vec4f(ShaderContext &input)> PixelShader;

//---------------------------------------------------------------------
// λͼ�⣺���ڼ���/����ͼƬ�����㣬���ߣ���ɫ��ȡ
//---------------------------------------------------------------------
class Bitmap
{
public:
	inline Bitmap(int width, int height) :_w(width), _h(height)
	{
		_pitch = width * 4;		// _bits��1�ֽ� ��ɫ������Ҫ4�ֽ�
		_bits = new uint8_t[_pitch * _h];
	}

public:
	inline int GetW() const { return _w; }
	inline int GetH() const { return _h; }
	inline int GetPitch() const { return _pitch; }
	inline uint8_t *GetBits() { return _bits; }
	inline const uint8_t *GetBits() const { return _bits; }
	inline uint8_t *GetLine(int y) { return _bits + _pitch * y; }
	inline const uint8_t *GetLine(int y) const { return _bits + _pitch * y; }

public:
	
	// _bits����4����һ�����ճ�4�ֽ� ȫ�����color��ɫ
	inline void Fill(uint32_t color)
	{
		for (int j = 0; j < _h; j++)
		{
			uint32_t *row = (uint32_t*)(_bits + j * _pitch);
			for (int i = 0; i < _w; i++, row++)
				memcpy(row, &color, sizeof(uint32_t));
		}
	}

	// Ϊָ������λ��ֵ��ɫ
	inline void SetPixel(int x, int y, uint32_t color)
	{
		if (x >= 0 && x < _w&&y >= 0 && y < _h) {
			memcpy(_bits + y * _pitch + x * 4, &color, sizeof(uint32_t));	
		}
	}

	inline uint32_t GetPixel(int x, int y) const 
	{
		uint32_t color = 0;
		if (x >= 0 && x < _w && y >= 0 && y < _h)
		{
			memcpy(&color, _bits + y * _pitch + x * 4, sizeof(uint32_t));
		}
		return color;
	}

	inline void DrawLine(int x1, int y1, int x2, int y2, uint32_t color)
	{
		int x, y;
		if (x1 == x2 && y1 == y2) 
		{
			SetPixel(x1, y1, color);
		}
		else if (x1 == x2)
		{
			int inc = (y1 <= y2) ? 1 : -1;
			for (int y = y1; y != y2; y += inc)SetPixel(x1, y, color);
			SetPixel(x2, y2, color);
		}
		else if (y1 == y2) 
		{
			int inc = (x1 <= x2) ? 1 : -1;
			for (x = x1; x != x2; x += inc) SetPixel(x, y1, color);
			SetPixel(x2, y2, color);
		}
		else
		{
			// ----- �����ϻ����� // ---- 

			int dx = Abs(x1 - x2);
			int dy = Abs(y1 - y2);
			int rem = 0;

			if (dx >= dy)
			{
				if (x2 < x1)
					x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;

				for (x = x1, y = y1; x <= x2; x++)
				{
					SetPixel(x, y, color);
					rem += dy;
					if (rem >= dx)
					{
						rem -= dx;
						y += (y2 >= y1) ? 1 : -1;
						SetPixel(x, y, color);
					}
				}
			}
			else
			{
				if (y2 < y1)
					x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;

				for (x = x1, y = y1; y <= y2; y++)
				{
					SetPixel(x, y, color);
					rem += dx;

					if (rem >= dy)
					{
						rem -= dy;
						x += (x2 >= x1) ? 1 : -1;
						SetPixel(x, y, color);
					}
				}
			}
		}
	}

	struct BITMAPINFOHEADER 
	{
		uint32_t	biSize;
		uint32_t	biWidth;
		int32_t		biHeight;
		uint16_t	biPlanes;
		uint16_t	biBitCount;
		uint32_t	biCompression;
		uint32_t	biSizeImage;
		uint32_t	biXPelsPerMeter;
		uint32_t	biYPelsPerMeter;
		uint32_t	biClrUsed;
		uint32_t	biClrImportant;
	};

	// ���� BMP �ļ�
	inline bool SaveFile(const char *filename, bool withAlpha = false) const {
		FILE *fp = fopen(filename, "wb");
		if (fp == NULL) return false;
		BITMAPINFOHEADER info;
		uint32_t pixelsize = (withAlpha) ? 4 : 3;
		uint32_t pitch = (GetW() * pixelsize + 3) & (~3);
		info.biSizeImage = pitch * GetH();
		uint32_t bfSize = 54 + info.biSizeImage;
		uint32_t zero = 0, offset = 54;
		fputc(0x42, fp);
		fputc(0x4d, fp);
		fwrite(&bfSize, 4, 1, fp);
		fwrite(&zero, 4, 1, fp);
		fwrite(&offset, 4, 1, fp);
		info.biSize = 40;
		info.biWidth = GetW();
		info.biHeight = GetH();
		info.biPlanes = 1;
		info.biBitCount = (withAlpha) ? 32 : 24;
		info.biCompression = 0;
		info.biXPelsPerMeter = 0xb12;
		info.biYPelsPerMeter = 0xb12;
		info.biClrUsed = 0;
		info.biClrImportant = 0;
		fwrite(&info, sizeof(info), 1, fp);
		// printf("pitch=%d %d\n", (int)pitch, info.biSizeImage);
		for (int y = 0; y < GetH(); y++) {
			const uint8_t *line = GetLine(info.biHeight - 1 - y);
			uint32_t padding = pitch - GetW() * pixelsize;
			for (int x = 0; x < GetW(); x++, line += 4) {
				fwrite(line, pixelsize, 1, fp);
			}
			for (int i = 0; i < (int)padding; i++) fputc(0, fp);
		}
		fclose(fp);
		return true;
	}

	// ���� Vec4f ����
	inline void SetPixel(int x, int y, const Vec4f& color)
	{
		SetPixel(x, y, vector_to_color(color));
	}

protected:
	int32_t _w;
	int32_t _h;
	int32_t _pitch;
	uint8_t *_bits;
};

//---------------------------------------------------------------------
// RenderHelp
//---------------------------------------------------------------------
class RenderHelp
{
public:
	inline RenderHelp()
	{
		_frame_buffer = NULL;
		_depth_buffer = NULL;
		_render_frame = true;
		_render_pixel = true;
	}

	inline RenderHelp(int width, int height)
	{
		_frame_buffer = NULL;
		_depth_buffer = NULL;
		_render_frame = true;
		_render_pixel = true;
		Init(width, height);
	}

public:
	// ��λ״̬
	inline void Reset()
	{
		_vertex_shader = NULL;
		_pixel_shader = NULL;

		if (_frame_buffer) delete _frame_buffer;
		_frame_buffer = NULL;

		if (_depth_buffer)
		{
			for (int j = 0; j < _fb_height; j++)
			{
				if (_depth_buffer[j])
					delete[] _depth_buffer[j];
				_depth_buffer[j] = NULL;
			}

			delete[] _depth_buffer;
			_depth_buffer = NULL;
		}
		_color_fg = 0xffffffff;
		_color_bg = 0xff191970;
	}

	// ��ʼ��FrameBuffer����Ⱦǰ����
	inline void Init(int width, int height)
	{
		Reset();
		_frame_buffer = new Bitmap(width, height);
		_fb_width = width;
		_fb_height = height;
		_depth_buffer = new float*[height];
		for (int j = 0; j < height; j++)
			_depth_buffer[j] = new float[width];
		Clear();
	}

	// ��� FrameBuffer �� ��Ȼ���
	inline void Clear()
	{
		if (_frame_buffer)
		{
			_frame_buffer->Fill(_color_bg);	// ��ɫ���Ϊ0xffffffff
		}

		if (_depth_buffer)
		{
			for (int j = 0; j < _fb_height; j++)
				for (int i = 0; i < _fb_width; i++)
					_depth_buffer[j][i] = 0.0f;
		}
	}

	// ���� VS/PS ��ɫ������
	inline void SetVertexShader(VertexShader vs) { _vertex_shader = vs; }
	inline void SetPixelShader(PixelShader ps) { _pixel_shader = ps; }

	// ���� FrameBuffer �� BMP �ļ�
	inline void SaveFile(const char* filename) { if (_frame_buffer) _frame_buffer->SaveFile(filename); }

	// ���ñ���/ǰ��ɫ
	inline void SetBGColor(uint32_t color) { _color_bg = color; }
	inline void SetFGColor(uint32_t color) { _color_fg = color; }

	// FrameBuffer����
	inline void SetPixel(int x, int y, uint32_t cc) { if (_frame_buffer) _frame_buffer->SetPixel(x, y, cc); }
	inline void SetPixel(int x, int y, const Vec4f& cc) { SetPixel(x, y, vector_to_color(cc)); }

	// FrameBuffer �ﻭ��
	inline void DrawLine(int x1, int y1, int x2, int y2)
	{
		if (_frame_buffer)
			_frame_buffer->DrawLine(x1, y1, x2, y2, _color_fg);
	}

	// ������Ⱦ״̬���Ƿ���ʾ�߿�ͼ���Ƿ����������
	inline void SetRenderState(bool frame, bool pixel) 
	{
		_render_frame = frame;
		_render_pixel = pixel;
	}

	// �ж�һ�����ǲ��������ε����ϱ�
	inline bool IsTopLeft(const Vec2i& a, const Vec2i& b) 
	{
		return ((a.y == b.y) && (a.x < b.x)) || (a.y > b.y);
	}

public:

	// ����һ�������Σ��������趨����ɫ������
	inline bool DrawPrimitive()
	{
		if (_frame_buffer == NULL || _vertex_shader == NULL)
			return false;

		// �����ʼ��
		for (int k = 0; k < 3; k++)
		{
			Vertex& vertex = _vertex[k];
			
			// ��������ĵ� varying �б�
			vertex.context.varying_float.clear();
			vertex.context.varying_vec2f.clear();
			vertex.context.varying_vec3f.clear();
			vertex.context.varying_vec4f.clear();

			// ���ж�����ɫ����
			// Ϊvertex��pos��ֵ��varying_vec4f�涥��ɫ
			vertex.pos = _vertex_shader(k, vertex.context);	
		
			// w����ʲô��
			// ��������ѭԭ���ߵļ򵥲ü����Ѿ�תΪ��βü��ռ��ˣ����������Լ�ʵ����οռ��ڵĲü�
			float w = vertex.pos.w;
			if (w == 0.0f) return false;
			if (vertex.pos.z < 0.0f || vertex.pos.z > w) return false;
			if (vertex.pos.x < -w || vertex.pos.x > w) return false;
			if (vertex.pos.y < -w || vertex.pos.x > w) return false;

			// ���� w �ĵ���
			vertex.rhw = 1.0 / w;

			// ��һ���豸���� ���������βü��ռ�ת����NDC����ռ�
			vertex.pos *= vertex.rhw;

			// ������Ļ����	[-1, 1] -> [0, 2] -> [0, _fb_width]
			vertex.spf.x = (vertex.pos.x + 1.0f) * _fb_width * 0.5f;
			vertex.spf.y = (1.0f - vertex.pos.y) * _fb_height * 0.5f;

			// ������Ļ���꣺�� 0.5 ��ƫ��ȡ��Ļ���ط������Ķ���
			vertex.spi.x = (int)(vertex.spf.x + 0.5f);
			vertex.spi.y = (int)(vertex.spf.y + 0.5f);
		
			// ������Ӿ��η�Χ
			if (k == 0)
			{
				_min_x = _max_x = Between(0, _fb_width - 1, vertex.spi.x);
				_min_y = _max_y = Between(0, _fb_height - 1, vertex.spi.y);
			}
			else
			{
				_min_x = Between(0, _fb_width - 1, Min(_min_x, vertex.spi.x));
				_max_x = Between(0, _fb_width - 1, Max(_max_x, vertex.spi.x));
				_min_y = Between(0, _fb_height - 1, Min(_min_y, vertex.spi.y));
				_max_y = Between(0, _fb_height - 1, Max(_max_y, vertex.spi.y));
			}
		}

		// �����߿�
		if (_render_frame)
		{
			DrawLine(_vertex[0].spi.x, _vertex[0].spi.y, _vertex[1].spi.x, _vertex[1].spi.y);
			DrawLine(_vertex[0].spi.x, _vertex[0].spi.y, _vertex[2].spi.x, _vertex[2].spi.y);
			DrawLine(_vertex[2].spi.x, _vertex[2].spi.y, _vertex[1].spi.x, _vertex[1].spi.y);
		}

		// �����������ؾ��˳�
		if (_render_pixel == false) return false;

		// �ж������γ���
		Vec4f v01 = _vertex[1].pos - _vertex[0].pos;
		Vec4f v02 = _vertex[2].pos - _vertex[0].pos;
		Vec4f normal = vector_cross(v01, v02);

		// ʹ�� vtx �����������㣬����ֱ���� _vertex ���ʣ���Ϊ���ܻ����˳��
		Vertex *vtx[3] = { &_vertex[0], &_vertex[1], &_vertex[2] };

		// ��������ӵ㣬�򽻻����㣬��֤ edge equation �жϵķ���Ϊ��
		if (normal.z > 0.0f) {
			vtx[1] = &_vertex[2];
			vtx[2] = &_vertex[1];
		}
		else if (normal.z == 0.0f) {
			return false;
		}

		// ���������˵�λ��
		Vec2i p0 = vtx[0]->spi;
		Vec2i p1 = vtx[1]->spi;
		Vec2i p2 = vtx[2]->spi;

		// ���������Ϊ����˳�
		float s = Abs(vector_cross(p1 - p0, p2 - p0));
		if (s <= 0) return false;

		// ���������ʱ�����������ı��ϵĵ���Ҫ�������ҷ����·����ϵĵ㲻����
		// ���ж��Ƿ��� TopLeft���жϳ��������·� Edge Equation һ�����
		bool TopLeft01 = IsTopLeft(p0, p1);
		bool TopLeft12 = IsTopLeft(p1, p2);
		bool TopLeft20 = IsTopLeft(p2, p0);

		// ������������Ӿ��ε����е�
		for (int cy = _min_y; cy <= _max_y; cy++) {
			for (int cx = _min_x; cx <= _max_x; cx++) {
				Vec2f px = { (float)cx + 0.5f, (float)cy + 0.5f };

				// Edge Equation
				// ʹ���������⸡����ͬʱ��Ϊ������ϵ�����Է���ȡ��
				int E01 = -(cx - p0.x) * (p1.y - p0.y) + (cy - p0.y) * (p1.x - p0.x);
				int E12 = -(cx - p1.x) * (p2.y - p1.y) + (cy - p1.y) * (p2.x - p1.x);
				int E20 = -(cx - p2.x) * (p0.y - p2.y) + (cy - p2.y) * (p0.x - p2.x);


				// ��������ϱߣ��� E >= 0 �жϺϷ���������±߾��� E > 0 �жϺϷ�
				// ����ͨ������һ����� 1 ������ < 0 �� <= 0 ��һ��ʽ�ӱ��
				if (E01 < (TopLeft01 ? 0 : 1)) continue;   // �ڵ�һ���ߺ���
				if (E12 < (TopLeft12 ? 0 : 1)) continue;   // �ڵڶ����ߺ���
				if (E20 < (TopLeft20 ? 0 : 1)) continue;   // �ڵ������ߺ���

				// �����˵㵽��ǰ���ʸ��
				Vec2f s0 = vtx[0]->spf - px;
				Vec2f s1 = vtx[1]->spf - px;
				Vec2f s2 = vtx[2]->spf - px;

				// ��������ϵ�������ڲ������������ a / b / c
				float a = Abs(vector_cross(s1, s2));    // �������� Px-P1-P2 ���
				float b = Abs(vector_cross(s2, s0));    // �������� Px-P2-P0 ���
				float c = Abs(vector_cross(s0, s1));    // �������� Px-P0-P1 ���
				float s = a + b + c;                    // �������� P0-P1-P2 ���

				if (s == 0.0f) continue;

				// ������������Ա�֤��a + b + c = 1������������ֵϵ��
				a = a * (1.0f / s);
				b = b * (1.0f / s);
				c = c * (1.0f / s);

				// ���㵱ǰ��� 1/w���� 1/w ����Ļ�ռ�����Թ�ϵ����ֱ�����Ĳ�ֵ
				float rhw = vtx[0]->rhw * a + vtx[1]->rhw * b + vtx[2]->rhw * c;

				// ������Ȳ���
				if (rhw < _depth_buffer[cy][cx]) continue;
				_depth_buffer[cy][cx] = rhw;   // ��¼ 1/w ����Ȼ���

				// ��ԭ��ǰ���ص� w
				float w = 1.0f / ((rhw != 0.0f) ? rhw : 1.0f);

				// �������������ֵ varying ��ϵ��
				// �ȳ��Ը��Զ���� w Ȼ�������Ļ�ռ��ֵȻ���ٳ��Ե�ǰ w
				float c0 = vtx[0]->rhw * a * w;
				float c1 = vtx[1]->rhw * b * w;
				float c2 = vtx[2]->rhw * c * w;

				// ׼��Ϊ��ǰ���صĸ��� varying ���в�ֵ
				ShaderContext input;

				ShaderContext& i0 = vtx[0]->context;
				ShaderContext& i1 = vtx[1]->context;
				ShaderContext& i2 = vtx[2]->context;

				// ��ֵ���� varying
				for (auto const &it : i0.varying_float) {
					int key = it.first;
					float f0 = i0.varying_float[key];
					float f1 = i1.varying_float[key];
					float f2 = i2.varying_float[key];
					input.varying_float[key] = c0 * f0 + c1 * f1 + c2 * f2;
				}

				for (auto const &it : i0.varying_vec2f) {
					int key = it.first;
					const Vec2f& f0 = i0.varying_vec2f[key];
					const Vec2f& f1 = i1.varying_vec2f[key];
					const Vec2f& f2 = i2.varying_vec2f[key];
					input.varying_vec2f[key] = c0 * f0 + c1 * f1 + c2 * f2;
				}

				for (auto const &it : i0.varying_vec3f) {
					int key = it.first;
					const Vec3f& f0 = i0.varying_vec3f[key];
					const Vec3f& f1 = i1.varying_vec3f[key];
					const Vec3f& f2 = i2.varying_vec3f[key];
					input.varying_vec3f[key] = c0 * f0 + c1 * f1 + c2 * f2;
				}

				for (auto const &it : i0.varying_vec4f) {
					int key = it.first;
					const Vec4f& f0 = i0.varying_vec4f[key];
					const Vec4f& f1 = i1.varying_vec4f[key];
					const Vec4f& f2 = i2.varying_vec4f[key];
					input.varying_vec4f[key] = c0 * f0 + c1 * f1 + c2 * f2;
				}

				// ִ��������ɫ��
				Vec4f color = { 0.0f, 0.0f, 0.0f, 0.0f };

				if (_pixel_shader != NULL) {
					color = _pixel_shader(input);
				}

				// ���Ƶ� framebuffer �ϣ�������Լ��жϣ���� PS ���ص���ɫ alpha ����
				// С�ڵ�������������ƣ����������Ļ�Ҫ��ǰ��ĸ�����Ȼ���Ĵ���Ų������
				// ֻ����Ҫ��Ⱦ��ʱ��Ÿ�����ȡ�
				_frame_buffer->SetPixel(cx, cy, color);
			}
		}

		// �����߿��ٻ�һ�α��⸲��
		if (_render_frame) {
			DrawLine(_vertex[0].spi.x, _vertex[0].spi.y, _vertex[1].spi.x, _vertex[1].spi.y);
			DrawLine(_vertex[0].spi.x, _vertex[0].spi.y, _vertex[2].spi.x, _vertex[2].spi.y);
			DrawLine(_vertex[2].spi.x, _vertex[2].spi.y, _vertex[1].spi.x, _vertex[1].spi.y);
		}

		return true;
	}

protected:

	// ����ṹ��
	struct Vertex 
	{
		ShaderContext context;	// ������
		
		float rhw;				// w�ĵ���
		Vec4f pos;				// ����
		Vec2f spf;				// ��������Ļ����
		Vec2i spi;				// ������Ļ����
	};

protected:
	Bitmap *_frame_buffer;	// ���ػ���
	float **_depth_buffer;	// ��Ȼ��� ���ΪɶҪ�������� ָ��ָ��ĵ�ַ��

	int _fb_width;			// frame buffer ���
	int _fb_height;			// frame buffer �߶�
	uint32_t _color_fg;		// ǰ��ɫ��������
	uint32_t _color_bg;		// ����ɫ��Clear��

	Vertex _vertex[3];		// �����ε���������

	int _min_x;				// ���������
	int _min_y;
	int _max_x;
	int _max_y;

	bool _render_frame;		// �Ƿ�����߿�
	bool _render_pixel;		// �Ƿ��������

	VertexShader _vertex_shader;
	PixelShader _pixel_shader;
};
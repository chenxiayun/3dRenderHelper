#pragma once

#include <assert.h>
#include <stdint.h>

#include <initializer_list>
#include <map>
#include <functional>
#include <string.h>


//---------------------------------------------------------------------
// 数学库：矢量定义
//---------------------------------------------------------------------

// 通用矢量：N 是矢量维度，T 为数据类型
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

// 特化二维矢量
template <typename T> struct Vector<2, T> {
	union {
		struct { T x, y; };    // 元素别名
		struct { T u, v; };    // 元素别名
		T m[2];                // 元素数组
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


// 特化三维矢量
template <typename T> struct Vector<3, T> {
	union {
		struct { T x, y, z; };    // 元素别名
		struct { T r, g, b; };    // 元素别名
		T m[3];                   // 元素数组
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

// 特化四维矢量
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
// 数学库：矢量运算
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

// = a / b，各个元素相除
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
// 数学库：矢量函数
//---------------------------------------------------------------------

// 不同维度的矢量转换
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

// 矢量点乘
template<size_t N, typename T>
inline T vector_dot(const Vector<N, T>& a, const Vector<N, T>& b)
{
	T sum = 0;
	for (size_t i = 0; i < N; i++) sum[i] = a[i] * b[i];
	return sum;
}

// 二维矢量叉乘，得到标量
template<typename T>
inline T vector_cross(const Vector<2, T>& a, const Vector<2, T>& b)
{
	return a.x * b.y - a.y * b.x;
}

// 三维矢量叉乘，得到新矢量
template<typename T>
inline Vector<3, T> vector_cross(const Vector<3, T>& a, const Vector<3, T>& b) 
{
	return Vector<3, T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

// 四维矢量叉乘：前三维叉乘，后一位保留
template<typename T>
inline Vector<4, T> vector_cross(const Vector<4, T>& a, const Vector<4, T>& b) 
{
	return Vector<4, T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, a.w);
}

//---------------------------------------------------------------------
// 数学库：矩阵
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

	// 取一行
	inline Vector<COL, T> Row(size_t row) const 
	{
		assert(row < ROW);
		Vector<COL, T> a;
		for (size_t i = 0; i < COL; i++)
			a[i] = m[row][i];
		return a;
	}

	// 取一列
	inline Vector<ROW, T> Col(size_t col) const 
	{
		assert(col < COL);
		Vector<ROW, T> a;
		for (size_t i = 0; i < ROW; i++)
			a[i] = m[i][col];
		return a;
	}

	// 设置一行
	inline void SetRow(size_t row, const Vector<COL, T>& a)
	{
		assert(row < ROW);
		for (size_t i = 0; i < COL; i++)
			m[row][i] = a[i];
	}

	// 设置一列
	inline void SetCol(size_t col, const Vector<ROW, T>& a) 
	{
		assert(col < COL);
		for (size_t i = 0; i < ROW; i++) 
			m[i][col] = a[i];
	}

	// 取得删除某行和某列的子矩阵：子式
	inline Matrix<ROW - 1, COL - 1, T> GetMinor(size_t row, size_t col) const
	{
		Matrix<ROW - 1, COL - 1, T> ret;
		for (size_t r = 0; r < ROW - 1; r++)
			for (size_t c = 0; c < COL - 1; c++)
				ret.m[r][c] = m[r < row ? r : r + 1][c < col ? c : c + 1];
		retirm ret;
	}

	// 取得转置矩阵
	inline Matrix<COL, ROW, T> Transpose() const
	{
		Matrix<COL, ROW, T> ret;
		for (size_t r = 0; r < ROW; r++)
			for (size_t c = 0; c < COL; c++)
				ret.m[c][r] = m[r][c];
		return ret;
	}

	// 取得0矩阵
	inline static Matrix<ROW, COL, T> GetZero()
	{
		Matrix<ROW, COL, T> ret;
		for (size_t r = 0; r < ROW; r++)
			for (size_t c = 0; c < COL; c++)
				ret.m[r][c] = 0;
		return ret;
	}

	// 取得单位矩阵
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
// 数学库：矩阵运算
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
// 工具函数
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

// 截取 [0, 1] 的范围
template<typename T> inline T Saturate(T x) 
{
	return Between<T>(0, 1, x);
}

// 类型别名
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
// 3D 数学运算
//---------------------------------------------------------------------

// 矢量转整数颜色
inline static uint32_t vector_to_color(const Vec4f& color)
{
	uint32_t r = (uint32_t)Between(0, 255, (int)(color.r * 255.0f));
	uint32_t g = (uint32_t)Between(0, 255, (int)(color.g * 255.0f));
	uint32_t b = (uint32_t)Between(0, 255, (int)(color.b * 255.0f));
	uint32_t a = (uint32_t)Between(0, 255, (int)(color.a * 255.0f));
	return (r << 16) | (g << 8) | b | (a << 24);
}

// 矢量转换整数颜色
inline static uint32_t vector_to_color(const Vec3f& color) 
{
	return vector_to_color(color.xyz1());
}

// 整数颜色到矢量 按照argb形式储存？这特么为啥
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

// 平移变换
inline static Mat4x4f matrix_set_translate(float x, float y, float z)
{
	Mat4x4f m = matrix_set_identity();
	m.m[3][0] = x;
	m.m[3][1] = y;
	m.m[3][2] = z;
	return m;
}

// 缩放变换
inline static Mat4x4f matrix_set_scale(float x, float y, float z)
{
	Mat4x4f m = matrix_set_identity();
	m.m[0][0] = x;
	m.m[1][1] = y;
	m.m[2][2] = z;
	return m;
}

// 旋转编号，围绕（x, y, z）矢量旋转 theta 角度 
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

// 摄影机变换矩阵：eye/视点位置，at/看向哪里，up/指向上方的矢量
inline static Mat4x4f matrix_set_lookat(const Vec3f& eye, const Vec3f& at, const Vec3f& up)
{
	Vec3f zaxis = vector_normalize(at - eye);
	Vec3f xaxis = vector_normalize(vector_cross(up, zaxis));
	Vec3f yaxis = vector_cross(zaxis, xaxis);

	Mat4x4f m;	// 为什么最后要点乘啊，看不明白 -eye.x不可以吗？
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
// 着色器定义
//---------------------------------------------------------------------

// 着色器上下文，由VS设置，再由渲染器按像素逐点插值后，供PS读取
struct ShaderContext
{
	std::map<int, float> varying_float;		// 浮点数 varying 列表
	std::map<int, Vec2f> varying_vec2f;		// 二维矢量 varying 列表
	std::map<int, Vec3f> varying_vec3f;		// 三维矢量 varying 列表
	std::map<int, Vec4f> varying_vec4f;		// 四维矢量 varying 列表
};

// 顶点着色器
typedef std::function<Vec4f(int index, ShaderContext &output)> VertexShader;

// 像素着色器
typedef std::function<Vec4f(ShaderContext &input)> PixelShader;

//---------------------------------------------------------------------
// 位图库：用于加载/保存图片，画点，画线，颜色读取
//---------------------------------------------------------------------
class Bitmap
{
public:
	inline Bitmap(int width, int height) :_w(width), _h(height)
	{
		_pitch = width * 4;		// _bits是1字节 颜色储存需要4字节
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
	
	// _bits数组4个分一批，凑成4字节 全部填充color颜色
	inline void Fill(uint32_t color)
	{
		for (int j = 0; j < _h; j++)
		{
			uint32_t *row = (uint32_t*)(_bits + j * _pitch);
			for (int i = 0; i < _w; i++, row++)
				memcpy(row, &color, sizeof(uint32_t));
		}
	}

	// 为指定像素位赋值颜色
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
			// ----- 上右上画折线 // ---- 

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

	// 保存 BMP 文件
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

	// 按照 Vec4f 画点
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
	// 复位状态
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

	// 初始化FrameBuffer，渲染前调用
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

	// 清空 FrameBuffer 和 深度缓存
	inline void Clear()
	{
		if (_frame_buffer)
		{
			_frame_buffer->Fill(_color_bg);	// 颜色填充为0xffffffff
		}

		if (_depth_buffer)
		{
			for (int j = 0; j < _fb_height; j++)
				for (int i = 0; i < _fb_width; i++)
					_depth_buffer[j][i] = 0.0f;
		}
	}

	// 设置 VS/PS 着色器函数
	inline void SetVertexShader(VertexShader vs) { _vertex_shader = vs; }
	inline void SetPixelShader(PixelShader ps) { _pixel_shader = ps; }

	// 保存 FrameBuffer 到 BMP 文件
	inline void SaveFile(const char* filename) { if (_frame_buffer) _frame_buffer->SaveFile(filename); }

	// 设置背景/前景色
	inline void SetBGColor(uint32_t color) { _color_bg = color; }
	inline void SetFGColor(uint32_t color) { _color_fg = color; }

	// FrameBuffer画点
	inline void SetPixel(int x, int y, uint32_t cc) { if (_frame_buffer) _frame_buffer->SetPixel(x, y, cc); }
	inline void SetPixel(int x, int y, const Vec4f& cc) { SetPixel(x, y, vector_to_color(cc)); }

	// FrameBuffer 里画线
	inline void DrawLine(int x1, int y1, int x2, int y2)
	{
		if (_frame_buffer)
			_frame_buffer->DrawLine(x1, y1, x2, y2, _color_fg);
	}

	// 设置渲染状态，是否显示线框图，是否填充三角形
	inline void SetRenderState(bool frame, bool pixel) 
	{
		_render_frame = frame;
		_render_pixel = pixel;
	}

	// 判断一条边是不是三角形的左上边
	inline bool IsTopLeft(const Vec2i& a, const Vec2i& b) 
	{
		return ((a.y == b.y) && (a.x < b.x)) || (a.y > b.y);
	}

public:

	// 绘制一个三角形，必须先设定好着色器函数
	inline bool DrawPrimitive()
	{
		if (_frame_buffer == NULL || _vertex_shader == NULL)
			return false;

		// 顶点初始化
		for (int k = 0; k < 3; k++)
		{
			Vertex& vertex = _vertex[k];
			
			// 清空上下文的 varying 列表
			vertex.context.varying_float.clear();
			vertex.context.varying_vec2f.clear();
			vertex.context.varying_vec3f.clear();
			vertex.context.varying_vec4f.clear();

			// 运行顶点着色程序
			// 为vertex的pos赋值，varying_vec4f存顶点色
			vertex.pos = _vertex_shader(k, vertex.context);	
		
			// w代表什么？
			// 这里先遵循原作者的简单裁剪（已经转为其次裁剪空间了？），后期自己实现其次空间内的裁剪
			float w = vertex.pos.w;
			if (w == 0.0f) return false;
			if (vertex.pos.z < 0.0f || vertex.pos.z > w) return false;
			if (vertex.pos.x < -w || vertex.pos.x > w) return false;
			if (vertex.pos.y < -w || vertex.pos.x > w) return false;

			// 计算 w 的倒数
			vertex.rhw = 1.0 / w;

			// 归一化设备坐标 把坐标从其次裁剪空间转换到NDC坐标空间
			vertex.pos *= vertex.rhw;

			// 计算屏幕坐标	[-1, 1] -> [0, 2] -> [0, _fb_width]
			vertex.spf.x = (vertex.pos.x + 1.0f) * _fb_width * 0.5f;
			vertex.spf.y = (1.0f - vertex.pos.y) * _fb_height * 0.5f;

			// 整数屏幕坐标：加 0.5 的偏移取屏幕像素方格中心对齐
			vertex.spi.x = (int)(vertex.spf.x + 0.5f);
			vertex.spi.y = (int)(vertex.spf.y + 0.5f);
		
			// 更新外接矩形范围
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

		// 绘制线框
		if (_render_frame)
		{
			DrawLine(_vertex[0].spi.x, _vertex[0].spi.y, _vertex[1].spi.x, _vertex[1].spi.y);
			DrawLine(_vertex[0].spi.x, _vertex[0].spi.y, _vertex[2].spi.x, _vertex[2].spi.y);
			DrawLine(_vertex[2].spi.x, _vertex[2].spi.y, _vertex[1].spi.x, _vertex[1].spi.y);
		}

		// 如果不填充像素就退出
		if (_render_pixel == false) return false;

		// 判断三角形朝向
		Vec4f v01 = _vertex[1].pos - _vertex[0].pos;
		Vec4f v02 = _vertex[2].pos - _vertex[0].pos;
		Vec4f normal = vector_cross(v01, v02);

		// 使用 vtx 访问三个顶点，而不直接用 _vertex 访问，因为可能会调整顺序
		Vertex *vtx[3] = { &_vertex[0], &_vertex[1], &_vertex[2] };

		// 如果背向视点，则交换顶点，保证 edge equation 判断的符号为正
		if (normal.z > 0.0f) {
			vtx[1] = &_vertex[2];
			vtx[2] = &_vertex[1];
		}
		else if (normal.z == 0.0f) {
			return false;
		}

		// 保存三个端点位置
		Vec2i p0 = vtx[0]->spi;
		Vec2i p1 = vtx[1]->spi;
		Vec2i p2 = vtx[2]->spi;

		// 计算面积，为零就退出
		float s = Abs(vector_cross(p1 - p0, p2 - p0));
		if (s <= 0) return false;

		// 三角形填充时，左面和上面的边上的点需要包括，右方和下方边上的点不包括
		// 先判断是否是 TopLeft，判断出来后会和下方 Edge Equation 一起决策
		bool TopLeft01 = IsTopLeft(p0, p1);
		bool TopLeft12 = IsTopLeft(p1, p2);
		bool TopLeft20 = IsTopLeft(p2, p0);

		// 迭代三角形外接矩形的所有点
		for (int cy = _min_y; cy <= _max_y; cy++) {
			for (int cx = _min_x; cx <= _max_x; cx++) {
				Vec2f px = { (float)cx + 0.5f, (float)cy + 0.5f };

				// Edge Equation
				// 使用整数避免浮点误差，同时因为是左手系，所以符号取反
				int E01 = -(cx - p0.x) * (p1.y - p0.y) + (cy - p0.y) * (p1.x - p0.x);
				int E12 = -(cx - p1.x) * (p2.y - p1.y) + (cy - p1.y) * (p2.x - p1.x);
				int E20 = -(cx - p2.x) * (p0.y - p2.y) + (cy - p2.y) * (p0.x - p2.x);


				// 如果是左上边，用 E >= 0 判断合法，如果右下边就用 E > 0 判断合法
				// 这里通过引入一个误差 1 ，来将 < 0 和 <= 0 用一个式子表达
				if (E01 < (TopLeft01 ? 0 : 1)) continue;   // 在第一条边后面
				if (E12 < (TopLeft12 ? 0 : 1)) continue;   // 在第二条边后面
				if (E20 < (TopLeft20 ? 0 : 1)) continue;   // 在第三条边后面

				// 三个端点到当前点的矢量
				Vec2f s0 = vtx[0]->spf - px;
				Vec2f s1 = vtx[1]->spf - px;
				Vec2f s2 = vtx[2]->spf - px;

				// 重心坐标系：计算内部子三角形面积 a / b / c
				float a = Abs(vector_cross(s1, s2));    // 子三角形 Px-P1-P2 面积
				float b = Abs(vector_cross(s2, s0));    // 子三角形 Px-P2-P0 面积
				float c = Abs(vector_cross(s0, s1));    // 子三角形 Px-P0-P1 面积
				float s = a + b + c;                    // 大三角形 P0-P1-P2 面积

				if (s == 0.0f) continue;

				// 除以总面积，以保证：a + b + c = 1，方便用作插值系数
				a = a * (1.0f / s);
				b = b * (1.0f / s);
				c = c * (1.0f / s);

				// 计算当前点的 1/w，因 1/w 和屏幕空间呈线性关系，故直接重心插值
				float rhw = vtx[0]->rhw * a + vtx[1]->rhw * b + vtx[2]->rhw * c;

				// 进行深度测试
				if (rhw < _depth_buffer[cy][cx]) continue;
				_depth_buffer[cy][cx] = rhw;   // 记录 1/w 到深度缓存

				// 还原当前像素的 w
				float w = 1.0f / ((rhw != 0.0f) ? rhw : 1.0f);

				// 计算三个顶点插值 varying 的系数
				// 先除以各自顶点的 w 然后进行屏幕空间插值然后再乘以当前 w
				float c0 = vtx[0]->rhw * a * w;
				float c1 = vtx[1]->rhw * b * w;
				float c2 = vtx[2]->rhw * c * w;

				// 准备为当前像素的各项 varying 进行插值
				ShaderContext input;

				ShaderContext& i0 = vtx[0]->context;
				ShaderContext& i1 = vtx[1]->context;
				ShaderContext& i2 = vtx[2]->context;

				// 插值各项 varying
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

				// 执行像素着色器
				Vec4f color = { 0.0f, 0.0f, 0.0f, 0.0f };

				if (_pixel_shader != NULL) {
					color = _pixel_shader(input);
				}

				// 绘制到 framebuffer 上，这里可以加判断，如果 PS 返回的颜色 alpha 分量
				// 小于等于零则放弃绘制，不过这样的话要把前面的更新深度缓存的代码挪下来，
				// 只有需要渲染的时候才更新深度。
				_frame_buffer->SetPixel(cx, cy, color);
			}
		}

		// 绘制线框，再画一次避免覆盖
		if (_render_frame) {
			DrawLine(_vertex[0].spi.x, _vertex[0].spi.y, _vertex[1].spi.x, _vertex[1].spi.y);
			DrawLine(_vertex[0].spi.x, _vertex[0].spi.y, _vertex[2].spi.x, _vertex[2].spi.y);
			DrawLine(_vertex[2].spi.x, _vertex[2].spi.y, _vertex[1].spi.x, _vertex[1].spi.y);
		}

		return true;
	}

protected:

	// 顶点结构体
	struct Vertex 
	{
		ShaderContext context;	// 上下文
		
		float rhw;				// w的倒数
		Vec4f pos;				// 坐标
		Vec2f spf;				// 浮点数屏幕坐标
		Vec2i spi;				// 整数屏幕坐标
	};

protected:
	Bitmap *_frame_buffer;	// 像素缓存
	float **_depth_buffer;	// 深度缓存 玛德为啥要这样定义 指向指针的地址？

	int _fb_width;			// frame buffer 宽度
	int _fb_height;			// frame buffer 高度
	uint32_t _color_fg;		// 前景色：画线用
	uint32_t _color_bg;		// 背景色：Clear用

	Vertex _vertex[3];		// 三角形的三个顶点

	int _min_x;				// 三角形外接
	int _min_y;
	int _max_x;
	int _max_y;

	bool _render_frame;		// 是否绘制线框
	bool _render_pixel;		// 是否填充像素

	VertexShader _vertex_shader;
	PixelShader _pixel_shader;
};
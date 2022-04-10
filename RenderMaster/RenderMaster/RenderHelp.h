#pragma once

#include <assert.h>
#include <stdint.h>

#include <initializer_list>
#include <map>

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
	inline Vector<2, T> xy() const { return Vector<2, T>(x, y); }
	inline Vector<3, T> xyz() const { return *this; }
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
	inline Vector(T x, T y, T z, T w) : x(X), y(Y), z(Z), w(W) {}
	inline Vector(const Vector<4, T> &u) : x(u.x), y(u.y), z(u.z), w(u.w) {}
	inline Vector(const T *prt) : x(ptr[0]), y(ptr[1]), z(ptr[2]), w(ptr[3]) {}
	inline const T& operator[] (size_t i) const { assert(i < 4); return m[i]; }
	inline T& operator[] (size_t i) { assert(i < 4); return m[i]; }
	inline void load(const T *ptr) { for (size_t i = 0; i < 4; i++) m[i] = ptr[i]; }
	inline void save(T *ptr) { for (size_t i = 0; i < 4; i++) ptr[i] = m[i]; }
	inline Vector<2, T> xy() const { return Vector<2, T>(x, y); }
	inline Vector<3, T> xyz() const { return Vector<3, T>(x, y, z); }
	inline Vector<4, T> xyzw() const { return *this; }
};

//---------------------------------------------------------------------
// 工具函数
//---------------------------------------------------------------------

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

//---------------------------------------------------------------------
// 位图库：用于加载/保存图片，画点，画线，颜色读取
//---------------------------------------------------------------------
class Bitmap 
{
public:
	inline Bitmap(int width, int height):_w(width), _h(height)
	{
		_pitch = width * 4;		// 为啥要乘4？
		_bits = new uint8_t[_pitch * _h];
	}

public:
	inline int GetW() const { return _w; }
	inline int GetH() const { return _h; }

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
		_render_frame = false;
		_render_pixel = true;
	}

	inline RenderHelp(int width, int height)
	{
		_frame_buffer = NULL;
		_depth_buffer = NULL;
		_render_frame = false;
		_render_pixel = true;
		Init(width, height);
	}

public:
	// 复位状态
	inline void Reset()
	{
		
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
		
	}

protected:
	Bitmap *_frame_buffer;	// 像素缓存
	float **_depth_buffer;	// 深度缓存 玛德为啥要这样定义 指向指针的地址？

	int _fb_width;			// frame buffer 宽度
	int _fb_height;			// frame buffer 高度
	uint32_t _color_fg;		// 前景色：画线用
	uint32_t _color_bg;		// 背景色：Clear用

	bool _render_frame;		// 是否绘制线框
	bool _render_pixel;		// 是否填充像素
};
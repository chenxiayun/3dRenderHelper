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
// ���ߺ���
//---------------------------------------------------------------------

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
		_pitch = width * 4;		// ΪɶҪ��4��
		_bits = new uint8_t[_pitch * _h];
	}

public:
	inline int GetW() const { return _w; }
	inline int GetH() const { return _h; }

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

			_color_fg = 0xffffffff;
			_color_bg = 0xff191970;
		}
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

			// ������Ļ����	todo:�е���� Ϊɶ��������
			vertex.spf.x = (vertex.pos.x + 1.0f) * _fb_width * 0.5f;
			vertex.spf.y = (1.0f - vertex.pos.y) * _fb_height * 0.5;
		
			// ������Ӿ��η�Χ
			if (k == 0)
			{

			}
		}
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
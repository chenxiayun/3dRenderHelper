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
template<typename T> inline T Abs(T x) { return (x < 0) ? (-x) : x; }
template<typename T> inline T Max(T x, T y) { return (x < y) ? y : x; }
template<typename T> inline T Min(T x, T y) { return (x > y) ? x : y; }

template<typename T> inline bool NearEqual(T x, T y, T error) {
	return (Abs(x - y) < error);
}

// ȡxmin��xmax�е���
template<typename T> inline T Between(T xmin, T xmax, T x) {
	return Min(Max(xmin, x), xmax);
}

// ��ȡ[0, 1]�ķ�Χ
template<typename T> inline T Saturate(T x) {
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

			// ������Ļ����	todo:�е���� Ϊɶ�������� �ⲿ��ȥ��һ���Ǹ����е���Ƶ
			vertex.spf.x = (vertex.pos.x + 1.0f) * _fb_width * 0.5f;
			vertex.spf.y = (1.0f - vertex.pos.y) * _fb_height * 0.5;
		
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

		return false;
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
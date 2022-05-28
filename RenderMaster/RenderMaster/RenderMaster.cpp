// RenderMaster.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "RenderHelp.h"
#include "Model.h"

inline void sample_01_triangle()
{
	// 初始化渲染器和帧缓存大小
	RenderHelp rh(800, 600);

	const int VARYING_COLOR = 0;    // 定义一个 varying 的 key

	// 顶点数据，由 VS 读取，如有多个三角形，可每次更新 vs_input 再绘制
	struct { Vec4f pos; Vec4f color; } vs_input[3] = {
		{ {  0.0,  0.7, 0.01, 1}, {1, 0, 0, 1} },
		{ { -0.6, -0.2, 0.01, 1}, {0, 1, 0, 1} },
		{ { +0.6, -0.2, 0.01, 1}, {0, 0, 1, 1} },
	};

	// 顶点着色器，初始化 varying 并返回坐标，
	// 参数 index 是渲染器传入的顶点序号，范围 [0, 2] 用于读取顶点数据
	rh.SetVertexShader([&](int index, ShaderContext& output) -> Vec4f {
		output.varying_vec4f[VARYING_COLOR] = vs_input[index].color;
		return vs_input[index].pos;		// 直接返回坐标
		});

	// 像素着色器，返回颜色
	rh.SetPixelShader([&](ShaderContext& input) -> Vec4f {
		return input.varying_vec4f[VARYING_COLOR];
		});

	// 渲染并保存
	rh.DrawPrimitive();
	rh.SaveFile("output.bmp");

	// 用画板显示图片
#if defined(_WIN32) || defined(WIN32)
	system("mspaint.exe output.bmp");
#endif
}

inline void sample_02_texture()
{
	RenderHelp rh(800, 600);

	// 定义一个纹理，并生成网格图案
	Bitmap texture(256, 256);
	for (int y = 0; y < 256; y++) {
		for (int x = 0; x < 256; x++) {
			int k = (x / 32 + y / 32) & 1;
			texture.SetPixel(x, y, k ? 0xffffffff : 0xff3fbcef);
		}
	}

	// 定义变换矩阵：模型变换，摄像机变换，透视变换
	Mat4x4f mat_model = matrix_set_identity();	// 模型变换
	Mat4x4f mat_view = matrix_set_lookat({ -0.7, 0, 1.5 }, { 0,0,0 }, { 0,0,1 });	// 摄像机方位
	Mat4x4f mat_proj = matrix_set_perspective(3.1415926f * 0.5f, 800 / 600.0, 1.0, 500.0f);
	Mat4x4f mat_mvp = mat_model * mat_view * mat_proj;	// 综合变换矩阵

	// 定义顶点输入
	struct VertexAttrib { Vec4f pos; Vec2f texuv; } vs_input[3];

	// 定义属性和 varying 中的纹理坐标 key
	const int VARYING_TEXUV = 0;

	// 顶点着色器
	rh.SetVertexShader([&](int index, ShaderContext& output) {
		Vec4f pos = vs_input[index].pos * mat_mvp;	// 输出变换后的坐标
		output.varying_vec2f[VARYING_TEXUV] = vs_input[index].texuv;
		return pos;
		});

	// 像素着色器
	rh.SetPixelShader([&](ShaderContext& input) -> Vec4f {
		Vec2f coord = input.varying_vec2f[VARYING_TEXUV];	// 取得纹理坐标
		return texture.Sample2D(coord);		// 纹理采样并返回像素颜色
		});

	// 0 1
	// 3 2  绘制两个三角形，组成一个矩形
	VertexAttrib vertex[] = {
		{ { 1, -1, -1, 1}, {0, 0} },
		{ { 1,  1, -1, 1}, {1, 0} },
		{ {-1,  1, -1, 1}, {1, 1} },
		{ {-1, -1, -1, 1}, {0, 1} },
	};

	vs_input[0] = vertex[0];
	vs_input[1] = vertex[1];
	vs_input[2] = vertex[2];
	rh.DrawPrimitive();

	vs_input[0] = vertex[2];
	vs_input[1] = vertex[3];
	vs_input[2] = vertex[0];
	rh.DrawPrimitive();

	// 保存文件
	rh.SaveFile("output.bmp");

	// 用画板显示图片
#if defined(_WIN32) || defined(WIN32)
	system("mspaint.exe output.bmp");
#endif
}

//---------------------------------------------------------------------
// sample_03_box
//---------------------------------------------------------------------

class sample_03
{
public:
	// 定义顶点结构
	struct VertexAttrib { Vec3f pos; Vec2f uv; Vec3f color; };

	// 顶底着色器输入
	VertexAttrib vs_input[3];

	// 模型
	VertexAttrib mesh[8] = {
		{ {  1, -1,  1, }, { 0, 0 }, { 1.0f, 0.2f, 0.2f }, },
		{ { -1, -1,  1, }, { 0, 1 }, { 0.2f, 1.0f, 0.2f }, },
		{ { -1,  1,  1, }, { 1, 1 }, { 0.2f, 0.2f, 1.0f }, },
		{ {  1,  1,  1, }, { 1, 0 }, { 1.0f, 0.2f, 1.0f }, },
		{ {  1, -1, -1, }, { 0, 0 }, { 1.0f, 1.0f, 0.2f }, },
		{ { -1, -1, -1, }, { 0, 1 }, { 0.2f, 1.0f, 1.0f }, },
		{ { -1,  1, -1, }, { 1, 1 }, { 1.0f, 0.3f, 0.3f }, },
		{ {  1,  1, -1, }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, },
	};

	// 定义属性和 varying 中的纹理坐标 key
	const int VARYING_TEXUV = 0;
	const int VARYING_COLOR = 1;

	void draw_plane(RenderHelp& rh, int a, int b, int c, int d)
	{
		// 强制将UV坐标变换到[0, 1]？
		mesh[a].uv.x = 0, mesh[a].uv.y = 0, mesh[b].uv.x = 0, mesh[b].uv.y = 1;
		mesh[c].uv.x = 1, mesh[c].uv.y = 1, mesh[d].uv.x = 1, mesh[d].uv.y = 0;

		vs_input[0] = mesh[a];
		vs_input[1] = mesh[b];
		vs_input[2] = mesh[c];
		rh.DrawPrimitive();

		vs_input[0] = mesh[c];
		vs_input[1] = mesh[d];
		vs_input[2] = mesh[a];
		rh.DrawPrimitive();
	}

	inline void sample_03_box()
	{
		RenderHelp rh(800, 600);

		// 定义一个纹理， 并生成网格图案
		Bitmap texture(256, 256);
		for (int y = 0; y < 256; y++) {
			for (int x = 0; x < 256; x++) {
				int k = (x / 32 + y / 32) & 1;
				texture.SetPixel(x, y, k ? 0xffffffff : 0xff3fbcef);
			}
		}

		// 定义变换矩阵：模型变换，摄像机变换，透视变换
		Mat4x4f mat_model = matrix_set_identity();	// 模型变换，旋转一定角度	
		Mat4x4f mat_view = matrix_set_lookat({ 2.5, 2.5, 2.5 }, { 0,0,0 }, { 0,0,1 });	// 摄像机方位
		Mat4x4f mat_proj = matrix_set_perspective(3.1415926f * 0.5f, 800 / 600.0, 1.0, 500.0f);
		Mat4x4f mat_mvp = mat_model * mat_view * mat_proj;	// 综合变换矩阵

		// 顶点着色器
		rh.SetVertexShader([&](int index, ShaderContext& output) -> Vec4f {
			Vec4f pos = vs_input[index].pos.xyz1() * mat_mvp;	// 扩充成四维矢量并转换
			output.varying_vec2f[VARYING_TEXUV] = vs_input[index].uv;
			//output.varying_vec4f[VARYING_COLOR] = vs_input[index].color.xyz1();
			return pos;
			});

		// 像素着色器
		rh.SetPixelShader([&](ShaderContext& input) -> Vec4f {
			Vec2f coord = input.varying_vec2f[VARYING_TEXUV];	// 取得纹理坐标
			Vec4f tc = texture.Sample2D(coord);		// 纹理采样并返回像素颜色
			return tc;
			});

		// 绘制盒子
		draw_plane(rh, 0, 1, 2, 3);	// 背面
		draw_plane(rh, 7, 6, 5, 4);	// 正面
		draw_plane(rh, 0, 4, 5, 1);	// 底
		draw_plane(rh, 1, 5, 6, 2);	// 左侧
		draw_plane(rh, 2, 6, 7, 3); // 上面
		draw_plane(rh, 3, 7, 4, 0);	// 右侧

				// 保存结果
		rh.SaveFile("output.bmp");

		// 用画板显示图片
#if defined(_WIN32) || defined(WIN32)
		system("mspaint.exe output.bmp");
#endif
	}
};

//---------------------------------------------------------------------
// sample_04_gouraud
//---------------------------------------------------------------------

class sample_04
{
public:
	// 定义结构顶点
	struct VertexAttrib { Vec3f pos; Vec2f uv; Vec3f color; Vec3f normal; };

	// 顶点着色器输入
	VertexAttrib vs_input[3];

	// 模型
	VertexAttrib mesh[8] = {
		{ {  1, -1,  1, }, { 0, 0 }, { 1.0f, 0.2f, 0.2f }, },
		{ { -1, -1,  1, }, { 0, 1 }, { 0.2f, 1.0f, 0.2f }, },
		{ { -1,  1,  1, }, { 1, 1 }, { 0.2f, 0.2f, 1.0f }, },
		{ {  1,  1,  1, }, { 1, 0 }, { 1.0f, 0.2f, 1.0f }, },
		{ {  1, -1, -1, }, { 0, 0 }, { 1.0f, 1.0f, 0.2f }, },
		{ { -1, -1, -1, }, { 0, 1 }, { 0.2f, 1.0f, 1.0f }, },
		{ { -1,  1, -1, }, { 1, 1 }, { 1.0f, 0.3f, 0.3f }, },
		{ {  1,  1, -1, }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, },
	};

	// 定义属性和 varying 中纹理坐标 key
	const int VARYING_TEXUV = 0;
	const int VARYING_COLOR = 1;
	const int VARYING_LIGHT = 2;

	void draw_plane(RenderHelp& rh, int a, int b, int c, int d)
	{
		mesh[a].uv.x = 0, mesh[a].uv.y = 0, mesh[b].uv.x = 0, mesh[b].uv.y = 1;
		mesh[c].uv.x = 1, mesh[c].uv.y = 1, mesh[d].uv.x = 1, mesh[d].uv.y = 0;

		Vec3f ab = mesh[b].pos - mesh[a].pos;
		Vec3f ac = mesh[c].pos - mesh[a].pos;
		Vec3f normal = vector_normalize(vector_cross(ac, ab));	// 求平面abc的法线，并归一化

		mesh[a].normal = normal;
		mesh[b].normal = normal;
		mesh[c].normal = normal;
		mesh[d].normal = normal;

		vs_input[0] = mesh[a];
		vs_input[1] = mesh[b];
		vs_input[2] = mesh[c];
		rh.DrawPrimitive();

		vs_input[0] = mesh[c];
		vs_input[1] = mesh[d];
		vs_input[2] = mesh[a];
		rh.DrawPrimitive();
	}

	inline void sample_04_gouraud()
	{
		RenderHelp rh(800, 600);

		// 定义一个纹理，并生成网格图案
		Bitmap texture(256, 256);
		for (int y = 0; y < 256; y++) {
			for (int x = 0; x < 256; x++) {
				int k = (x / 32 + y / 32) & 1;
				texture.SetPixel(x, y, k ? 0xffffffff : 0xff3fbcef);
			}
		}

		// 定义变换矩阵：模型变换，摄像机变换，透视变换
		Mat4x4f mat_model = matrix_set_rotate(0, 0, 1, 0);
		Mat4x4f mat_view = matrix_set_lookat({ 2.5, 2.5, 2.5 }, { 0,0,0 }, { 0,0,1 });	// 摄像机方位
		Mat4x4f mat_proj = matrix_set_perspective(3.1415926f * 0.5f, 800 / 600.0, 1.0, 500.0f);
		Mat4x4f mat_mvp = mat_model * mat_view * mat_proj;	// 综合变换矩阵

		// 法向量的变换矩阵为模型变换矩阵的逆转置矩阵
		Mat4x4f mat_model_it = matrix_invert(mat_model).Transpose();

		// 光照（光源）方向
		Vec3f light_dir = { 1, 0, 2 };

		// 顶点着色器
		rh.SetVertexShader([&](int index, ShaderContext& output) -> Vec4f {
			// 扩充成四维矢量并变换
			Vec4f pos = vs_input[index].pos.xyz1() * mat_mvp;
			output.varying_vec2f[VARYING_TEXUV] = vs_input[index].uv;
			output.varying_vec4f[VARYING_COLOR] = vs_input[index].color.xyz1();
			
			// 法向量转换到世界坐标
			Vec3f normal = vs_input[index].normal;
			normal = (normal.xyz1() * mat_model_it).xyz();

			output.varying_vec3f[VARYING_LIGHT] = normal;
			 //计算光照强度 等会在像素着色器试试	越远越暗
			float intense = vector_dot(normal, vector_normalize(light_dir));
			
			 //避免越界的同时加入一个常量环境光 0.1
			intense = Max(0.0f, intense) + 0.1;
			output.varying_float[VARYING_LIGHT] = intense;
			return pos;
			});

		// 像素着色器
		rh.SetPixelShader([&](ShaderContext& input) -> Vec4f {
			Vec2f coord = input.varying_vec2f[VARYING_TEXUV];	// 取得纹理坐标
			Vec4f tc = texture.Sample2D(coord);		// 纹理采样并返回像素颜色
			float light = input.varying_float[VARYING_LIGHT];
			return tc * light;
			});

		// 绘制盒子
		draw_plane(rh, 0, 1, 2, 3);
		draw_plane(rh, 7, 6, 5, 4);
		draw_plane(rh, 0, 4, 5, 1);
		draw_plane(rh, 1, 5, 6, 2);
		draw_plane(rh, 2, 6, 7, 3);
		draw_plane(rh, 3, 7, 4, 0);

		// 保存结果
		rh.SaveFile("output.bmp");

		// 用画板显示图片
#if defined(_WIN32) || defined(WIN32)
		system("mspaint.exe output.bmp");
#endif
	}
};

inline void sample_05_model()
{
	RenderHelp rh(600, 800);

	// 加载模型
	Model model("res/diablo3_pose.obj");

	Vec3f eye_pos = { 0, -0.5, 1.7 };
	Vec3f eye_at = { 0, 0, 0 };
	Vec3f eye_up = { 0, 1, 0 };
	Vec3f light_dir = { 1, 1, 0.85 };
	float perspective = 3.1415926f * 0.5f;

	Mat4x4f mat_model = matrix_set_scale(1, 1, 1);
	Mat4x4f mat_view = matrix_set_lookat(eye_pos, eye_at, eye_up);
	Mat4x4f mat_proj = matrix_set_perspective(perspective, 6 / 8.0, 1.0, 500.0f);
	Mat4x4f mat_mvp = mat_model * mat_view * mat_proj;

	// 用于将法向量从模型坐标系变换到世界坐标系
	Mat4x4f mat_model_it = matrix_invert(mat_model).Transpose();

	// 顶点属性
	struct { Vec3f pos; Vec3f normal; Vec2f uv; } vs_input[3];

	const int VARYING_UV = 0;
	const int VARYING_NORMAL = 1;

	rh.SetVertexShader([&](int index, ShaderContext& output) -> Vec4f {
		Vec4f pos = vs_input[index].pos.xyz1() * mat_mvp;
		Vec4f normal = (vs_input[index].normal.xyz1() * mat_model_it);
		output.varying_vec2f[VARYING_UV] = vs_input[index].uv;
		output.varying_vec3f[VARYING_NORMAL] = normal.xyz();
		
		return pos;
		});

	rh.SetPixelShader([&](ShaderContext& input)->Vec4f {
		Vec2f uv = input.varying_vec2f[VARYING_UV];
		Vec3f n = input.varying_vec3f[VARYING_NORMAL];
		Vec3f l = vector_normalize(light_dir);

		Vec4f color = model.diffuse(uv);
		// 点乘n,l 获得光照强度
		float intense = Saturate(vector_dot(n, l)) + 0.1;
		return color * intense;
		});

	// 迭代模型每一个面
	for (int i = 0; i < model.nfaces(); i++)
	{
		// 设置三个顶点的输入，供VS读取
		for (int j = 0; j < 3; j++)
		{
			vs_input[j].pos = model.vert(i, j);
			vs_input[j].uv = model.uv(i, j);
			vs_input[j].normal = model.normal(i, j);
		}
		// 绘制三角形
		rh.DrawPrimitive();
	}

	rh.SaveFile("output.bmp");

#if defined(WIN32) || defined(_WIN32)
	system("mspaint output.bmp");
#endif
};

inline void sample_06_normal()
{
	RenderHelp rh(600, 800);

	// 加载模型
	Model model("res/diablo3_pose.obj");

	Vec3f eye_pos = { 0, -0.5, 1.7 };
	Vec3f eye_at = { 0, 0, 0 };
	Vec3f eye_up = { 0, 1, 0 };
	Vec3f light_dir = { 1, 1, 0.85 };
	float perspective = 3.1415926f * 0.5f;

	Mat4x4f mat_model = matrix_set_scale(1, 1, 1);
	Mat4x4f mat_view = matrix_set_lookat(eye_pos, eye_at, eye_up);
	Mat4x4f mat_proj = matrix_set_perspective(perspective, 6 / 8.0, 1.0, 500.0f);
	Mat4x4f mat_mvp = mat_model * mat_view * mat_proj;

	// 用于将法向量从模型坐标系变换到世界坐标系
	Mat4x4f mat_model_it = matrix_invert(mat_model).Transpose();

	// 顶点属性
	struct { Vec3f pos; Vec3f normal; Vec2f uv; } vs_input[3];

	const int VARYING_UV = 0;

	rh.SetVertexShader([&](int index, ShaderContext& output) -> Vec4f {
		Vec4f pos = vs_input[index].pos.xyz1() * mat_mvp;
		output.varying_vec2f[VARYING_UV] = vs_input[index].uv;

		return pos;
		});

	rh.SetPixelShader([&](ShaderContext& input) -> Vec4f {
		Vec2f uv = input.varying_vec2f[VARYING_UV];
		Vec3f l = vector_normalize(light_dir);
		Vec3f n = (model.normal(uv).xyz1() * mat_model_it).xyz();
		float intense = Saturate(vector_dot(n, l) + 0.1f);
		Vec4f color = model.diffuse(uv);

		return color * intense;
		});

	// 迭代模型每一个面
	for (int i = 0; i < model.nfaces(); i++)
	{
		// 设置三个顶点的输入，供VS读取
		for (int j = 0; j < 3; j++)
		{
			vs_input[j].pos = model.vert(i, j);
			vs_input[j].uv = model.uv(i, j);
			vs_input[j].normal = model.normal(i, j);
		}
		// 绘制三角形
		rh.DrawPrimitive();
	}

	rh.SaveFile("output.bmp");

#if defined(WIN32) || defined(_WIN32)
	system("mspaint output.bmp");
#endif
}

inline void sample_07_specular()
{
	RenderHelp rh(600, 800);

	// 加载模型
	Model model("res/diablo3_pose.obj");

	Vec3f eye_pos = { 0, -0.5, 1.7 };
	Vec3f eye_at = { 0, 0, 0 };
	Vec3f eye_up = { 0, 1, 0 };
	Vec3f light_dir = { 1, 1, 0.85 };			// 光的方向
	float perspective = 3.1415926f * 0.5f;

	Mat4x4f mat_model = matrix_set_scale(1, 1, 1);
	Mat4x4f mat_view = matrix_set_lookat(eye_pos, eye_at, eye_up);
	Mat4x4f mat_proj = matrix_set_perspective(perspective, 6 / 8.0, 1.0, 500.0f);
	Mat4x4f mat_mvp = mat_model * mat_view * mat_proj;

	// 用于将法向量从模型坐标系变换到世界坐标系
	Mat4x4f mat_model_it = matrix_invert(mat_model).Transpose();

	// 顶点属性
	struct { Vec3f pos; Vec3f normal; Vec2f uv; } vs_input[3];

	const int VARYING_UV = 0;
	const int VARYING_EYE = 1;	// 眼睛相对顶点的位置

	rh.SetVertexShader([&](int index, ShaderContext& output) -> Vec4f {
		Vec4f pos = vs_input[index].pos.xyz1() * mat_mvp;
		// 将顶点位置从模型空间转换为世界坐标系
		Vec3f pos_world = (vs_input[index].pos.xyz1() * mat_model).xyz();
		// 计算模型顶点到眼睛的方向
		Vec3f eye_dir = eye_pos - pos_world;
		output.varying_vec2f[VARYING_UV] = vs_input[index].uv;
		output.varying_vec3f[VARYING_EYE] = eye_dir;

		return pos;
		});

	rh.SetPixelShader([&](ShaderContext& input) -> Vec4f {
		Vec2f uv = input.varying_vec2f[VARYING_UV];
		// 模型上当前点到眼睛的方向
		Vec3f eye_dir = input.varying_vec3f[VARYING_EYE];
		// 归一化光
		Vec3f l = vector_normalize(light_dir);
		// 法向贴图取出法向并转换为世界坐标系
		Vec3f n = (model.normal(uv).xyz1() * mat_model_it).xyz();
		// 从模型中取出当前点的高光参数
		float s = model.Specular(uv);
		// 计算反射光线
		Vec3f r = vector_normalize(n * vector_dot(n, l) * 2.0f - l);
		// 计算高光
		float p = Saturate(vector_dot(r, eye_dir));
		float spec = Saturate(pow(p, s * 20) * 0.05);
		// 综合光照强度
		float intense = Saturate(vector_dot(n, l)) + 0.2f + spec;
		Vec4f color = model.diffuse(uv);
		return color * intense;
		});

	// 迭代模型每一个面
	for (int i = 0; i < model.nfaces(); i++) {
		// 设置三个顶点的输入，供 VS 读取
		for (int j = 0; j < 3; j++) {
			vs_input[j].pos = model.vert(i, j);
			vs_input[j].uv = model.uv(i, j);
			vs_input[j].normal = model.normal(i, j);
		}
		// 绘制三角形
		rh.DrawPrimitive();
	}

	rh.SaveFile("output.bmp");

#if defined(WIN32) || defined(_WIN32)
	system("mspaint output.bmp");
#endif
}

int main()
{
	//sample_01_triangle();

	//sample_02_texture();
	
	//sample_03 test;
	//test.sample_03_box();

	//sample_04 test;
	//test.sample_04_gouraud();

	//sample_05_model();

	//sample_06_normal();

	sample_07_specular();

	return 0;
}
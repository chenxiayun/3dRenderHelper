﻿// RenderMaster.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "RenderHelp.h"

inline void sample_01_triangle()
{
	// 初始化渲染器和帧缓存大小
	RenderHelp rh(800, 600);

	const int VARYING_COLOR = 0;    // 定义一个 varying 的 key

	// 顶点数据，由 VS 读取，如有多个三角形，可每次更新 vs_input 再绘制
	struct { Vec4f pos; Vec4f color; } vs_input[3] = {
		{ {  0.0,  0.7, 0.90, 1}, {1, 0, 0, 1} },
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

int main()
{
	RenderHelp rh(800, 600);

	//// 定义一个纹理，并生成网格图案
	//Bitmap texture(256, 256);
	//for(int y = 0;y < 256;y++)
	//	for (int x = 0; x < 256; x++)
	//	{
	//		int ()
	//	}

	auto color = vector_from_color(0xFFFFFF00);
	printf("%f,%f,%f,%f", color.r, color.g, color.b, color.a);

	return 0;
}
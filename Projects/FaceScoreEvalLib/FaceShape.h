//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: FaceShape.h
// 
// Author: X_Jun
//
// OS:Windows 10
// IDE: Visual Studio 2015 Pro
// Lib: Opencv3.2.0
//
// Description: 用于判断脸型
//          
//////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _FACESHAPE_H_
#define _FACESHAPE_H_
#include "FaceImage.h"

namespace Face
{
	enum FACE_SHAPE
	{
		FACE_SHAPE_UNKNOWN,		// 未知
		FACE_SHAPE_OVAL,		// 椭圆脸
		FACE_SHAPE_SQUARE,		// 方形脸
		FACE_SHAPE_ROUND,		// 圆脸
		FACE_SHAPE_TRIANGLE,	// 倒三角脸
		FACE_SHAPE_OBLONG,		// 长方形脸
	};

	class FaceShape
	{
	public:
		

	public:
		FaceShape(const FaceImage& faceImage);
		~FaceShape();

		// **********************************************
		// 功能: 获取脸部颧骨宽度
		// 返回值: 颧骨宽度
		float GetCheekBoneWith() const;

		// **********************************************
		// 功能: 获取脸部下颌宽度
		// 返回值: 下颌宽度
		float GetJawWidth() const;

		// **********************************************
		// 功能: 获取脸部高度
		// 返回值: 脸部高度
		int GetHeight() const;
		
		// **********************************************
		// 功能: 输出下巴曲线方程的系数(使用索引4-12的特征点)
		// 参数: 需要的最高次项(不允许超过9次项)
		// 返回值: 0次项到N次项的浮点vector
		std::vector<float> GetJawCurveCoefficient(int N);

		// **********************************************
		// 功能: 获取脸型
		// 返回值: 脸型枚举
		FACE_SHAPE GetFaceShape();
	private:
		const FaceImage& faceImage;
		FACE_SHAPE shape;	// 脸型

	};

}	// namespace Face





#endif
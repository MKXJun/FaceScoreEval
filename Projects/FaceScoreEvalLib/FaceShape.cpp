#include "FaceP.h"
#include "FaceShape.h"
#include "Utility.h"


namespace Face
{
	FaceShape::FaceShape(const FaceImage & faceImage)
		: faceImage(faceImage), shape()
	{
	}


	FaceShape::~FaceShape()
	{
	}

	// **********************************************
	// 功能: 获取脸部颧骨宽度
	// 返回值: 颧骨宽度
	float FaceShape::GetCheekBoneWith() const
	{
		return faceImage.pointSet[15].x - faceImage.pointSet[1].x;
	}

	// **********************************************
	// 功能: 获取脸部下颌宽度
	// 返回值: 下颌宽度
	float FaceShape::GetJawWidth() const
	{
		return faceImage.pointSet[11].x - faceImage.pointSet[5].x;
	}

	// **********************************************
	// 功能: 获取脸部高度
	// 返回值: 脸部高度
	int FaceShape::GetHeight() const
	{
		return faceImage.faceMat.rows;
	}

	// **********************************************
	// 功能: 输出下巴曲线方程的系数(使用索引3-13的特征点)
	// 参数: 需要的最高次项(不允许超过11次项)
	// 返回值: 0次项到N次项的浮点vector
	std::vector<float> FaceShape::GetJawCurveCoefficient(int N)
	{
		return GetMultinomialCoefficient(
			std::vector<cv::Point2f>(
				faceImage.pointSet.GetData().begin() + 3,
				faceImage.pointSet.GetData().begin() + 14),
			4);
	}

	// **********************************************
	// 功能: 获取脸型
	// 返回值: 脸型枚举
	FACE_SHAPE FaceShape::GetFaceShape()
	{
		// 之前已经求出的话就不需要再求了
		if (shape != FACE_SHAPE_UNKNOWN)
		{
			return shape;
		}

		auto vec = GetJawCurveCoefficient(4);
		// 按一次项系数归一化
		float factor = vec[1];
		for (float& f : vec)
			f /= factor;
		
		// 下巴类型判定
		int jawType = 0;
		if (vec[2] < -0.0041f)
			jawType = 0;	// 方下巴
		else if (vec[2] > -0.0015f && vec[3] < 0.0f)
			jawType = 1;	// 尖下巴
		else
			jawType = 2;	// 圆下巴

		// 确定高度与宽度的比值
		float rate = (float)faceImage.faceMat.rows / (float)faceImage.faceWidth;
		int shapeType = 0;
		if (rate > 1.3f)
			shapeType = 0;		// 长>>>宽
		else if (rate < 1.2f)
			shapeType = 1;		// 长>宽
		else
			shapeType = 2;		// 长>>宽
		
		// 行为下巴，列为宽高比
		FACE_SHAPE finalShape[3][3] = {
			{ FACE_SHAPE_OBLONG, FACE_SHAPE_SQUARE, FACE_SHAPE_OBLONG },
			{ FACE_SHAPE_TRIANGLE, FACE_SHAPE_TRIANGLE, FACE_SHAPE_TRIANGLE },
			{ FACE_SHAPE_OVAL, FACE_SHAPE_ROUND, FACE_SHAPE_OVAL },
			
		};

		return shape = finalShape[jawType][shapeType];
	}

}	// nameSpace Face
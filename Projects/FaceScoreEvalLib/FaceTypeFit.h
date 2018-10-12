//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: FaceTypeFit.h
// 
// Author: Rinzic
//
// OS:Windows 10
// IDE: Visual Studio 2015 Pro
// Lib: Opencv3.2.0, dlib_v19.1, STASM
//
// Description: 基于最小二乘法利用幂函数拟合脸型的类
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FACE_TYPE_FIT
#define FACE_TYPE_FIT

#include "FeaturePointSet.h"

namespace Face
{ 

/** 下巴类型 */
enum JawType
{
	SQUARE_FACE,//国字脸
	OVAL_FACE	//瓜子脸
};

/**
 *	根据特征点判别下巴的轮廓。
 */
struct FaceTypeFit
{
public:

	/**
	 *	根据传入的特征点分析脸型下巴类型。
	 *	@ PointSet - 特征点集合。
	 *	@ return - 下巴类型。
	 */
	static JawType AnalyzeJawType(const FeaturePointSet& PointSet);

	//FaceTypeFit(const std::vector<cv::Point2f>& dlibPoints, const std::vector<cv::Point2f>& stasmPoints)
	//	: nor(dlibPoints, stasmPoints)
	//{
	//	distinguish();
	//}
	
	FeaturePointSet nor;//归一化的特征点数据类
	float a = 1;		//用于拟合的幂函数的参数(a>0)
	unsigned int k = 2;	//用于拟合的幂函数的阶(k>=2)
	float deviation = INFINITY;//最小二乘法的偏差（0=< deviation <INFINITY）
	enum FaceType
	{
		SQUARE_FACE,//国字脸
		OVAL_FACE	//瓜子脸
	}faceType;

	//methods
	void fitting(bool flag = false, int startLabel = 0, int endLabel = 16);

	/**
	 *	描述暂定。
	 *	@ return - 返回暂定
	 */
	FaceType distinguish();
};



}	// namespace Face


#endif //FACE_TYPE_FIT
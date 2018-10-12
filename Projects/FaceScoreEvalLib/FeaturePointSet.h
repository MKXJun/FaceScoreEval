//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 头文件名: FeaturePointSet.h
// 
// 作者: X_Jun
//
// 使用库: Opencv3.2, Dlib, STASM
//
// 描述: 特征点集合类，用于将Dlib和Stasm的特征点集进行有机结合
//
// 备注：需要支持C++11
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _FEATUREPOINTSET_H_
#define _FEATUREPOINTSET_H_

#include <iostream>
#include <vector>
#include <string>
#include <opencv2/core.hpp>

namespace Face
{



class FeaturePointSet
{
public:
	static const float face_width;		// 面部宽度600.0
	static const int combineLandMarks = 87;	// 结合后的特征点数目87
											// 0-16 脸型
											// 17-19 弃用
											// 20-25 左眉毛
											// 26-31 右眉毛
											// 32-33 弃用
											// 34-42 左眼
											// 43-51 右眼
											// 52-62 83-87 鼻翼+鼻梁
											// 63-82 嘴巴

public:
	// **********************************************
	// 功能: 使用一个包含特征点集合的数组初始化
	// 参数: 
	//		 [In]元素个数为combineLandMarks的特征点数组
	FeaturePointSet(const cv::Point2f(&pointSet)[combineLandMarks]);

	// **********************************************
	// 功能: 使用一个包含特征点集合的数组初始化
	// 参数: 
	//		 [In]元素个数为combineLandMarks的特征点数组
	FeaturePointSet(const std::vector<cv::Point2f>& pointSet);

	// **********************************************
	// 功能: 使用一个包含特征点集合的数组初始化
	// 参数: 
	//		 [In]元素个数为combineLandMarks的特征点数组
	FeaturePointSet(std::vector<cv::Point2f>&& pointSet);

	FeaturePointSet(const FeaturePointSet&) = default;

	// **********************************************
	// 功能: 将stasm的点集与dlib的点集有机结合在一起
	// 参数:
	//		 [In]dlibPoints Dlib库识别的人脸特征点集
	//		 [In]stasmPoints Stasm库识别的人脸特征点集
	//       [In]useNormalize 是否在构造时进行标准化
	FeaturePointSet(const std::vector<cv::Point2f>& dlibPoints, const std::vector<cv::Point2f>& stasmPoints);

	// **********************************************
	// 功能: 空构造方法
	FeaturePointSet();
	
	// **********************************************
	// 功能: 移动构造方法
	FeaturePointSet(FeaturePointSet&& pointSet);


	// **********************************************
	// 功能: 析构函数
	~FeaturePointSet();

	// **********************************************
	// 功能: 将stasm的点集与dlib的点集有机结合在一起
	//       并标记为未规格化状态
	// 参数:
	//		 [In]dlibPoints Dlib库识别的人脸特征点集
	//		 [In]stasmPoints Stasm库识别的人脸特征点集
	// 返回值: 合并是否成功
	bool Merge(const std::vector<cv::Point2f>& dlibPoints, const std::vector<cv::Point2f>& stasmPoints);

	// **********************************************
	// 功能: 获取对应索引的特征点
	// 参数:
	//		 [In]pos 点索引
	// 返回值: 对应索引的特征点
	const cv::Point2f& operator[](size_t pos) const;

	// **********************************************
	// 功能: 获取对应索引的特征点
	// 参数:
	//		 [In]pos 点索引
	// 返回值: 对应索引的特征点
	cv::Point2f& operator[](size_t pos);

	// **********************************************
	// 功能: 获取特征点数组
	std::vector<cv::Point2f>& GetData();

	// **********************************************
	// 功能: 获取特征点数组
	const std::vector<cv::Point2f>& GetData() const;

	// **********************************************
	// 功能: 获取脸部中线上的点
	// 参数:
	//       [In]pointSet 特征点集合
	// 返回值: 一系列中线上的点
	std::vector<cv::Point2f> GetFaceMiddleLinePoints() const;

	// **********************************************
	// 功能: 获取特征点边界矩形
	// 返回值: 特征点边界矩形
	cv::Rect2f GetBorder() const;


private:
	std::vector<cv::Point2f> data;		// 结合后的自定义特征点集合
};

// **********************************************
// 功能: 特定形式输出点
// 参数:
//		 [InOut]os 输出流
//       [In]p     要输出的点
// 返回值: 使用的输出流
// 备注: 输出形式如 (2.300000, 1.457938)
std::ostream& operator<<(std::ostream& os, const cv::Point2f& p);


}	// namespace Face










#endif
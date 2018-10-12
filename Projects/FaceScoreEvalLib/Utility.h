//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 头文件名: Utility.h
// 
// 作者: X_Jun
//
// 使用库: Opencv3.2, Dlib, STASM
//
// 描述: 存放一些常用人脸识别的函数工具
//
// 备注：需要支持C++11
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "FeaturePointSet.h"


// 使用STASM侦测是否存在脸部
bool detectFaceByStasm(const cv::Mat& inputImage, cv::Rect * pArea);


// **********************************************
// 功能: 使用STASM获取特征点
// 参数:   
//       [In]stasmImage 带有脸部的位图 
//       [Out]stasmPoint 用于获取Stasm的所有特征点
// 返回值: 是否侦测到脸部
bool GetStasmPoint(const cv::Mat1b& stasmImage, std::vector<cv::Point2f>& stasmPoint);


// **********************************************
// 功能: 使用DLIB获取特征点
// 参数:   
//       [In]dlibImage 带有脸部的位图 
//       [Out]dlibPoint 用于获取Dlib的所有特征点
// 返回值: 是否侦测到脸部
bool GetDlibPoint(const cv::Mat& dlibImage, std::vector<cv::Point2f>& dlibPoint);


//// 在图上绘制对应特征点的序号
//void drawPointNumber(const char* winName, const char* path);
//
//// 在图上绘制特征点
//void drawPoint(const char* winName, const char * path);



// **********************************************
// 功能: 使用Stasm和Dlib识别人脸并获取特征点集合
// 参数:   
//       [In]Path 带有脸部的位图文件路径 
//       [Out]PointSet 用于输出特征点
// 返回值: 是否识别出人脸并输出了特征点集合
bool ExtractFeaturePoint(const char* Path, Face::FeaturePointSet* PointSet);


/**
*	获取 2 维空间中众多离散点的多项式拟合曲线。根据最小二乘法多项式曲线拟合原理获取。
*	@ PointSet - 离散点集合。
*	@ Order - 多项式中的最高阶数。
*	@ return - 多项式系数 a 集合。（y = a0 + a1*x + a2*x^2 + ...）
*   注意：Order的值范围为[0, numPoints - 1]
*/
std::vector<float> GetMultinomialCoefficient(const std::vector<cv::Point2f> & PointSet, int Order);


#endif

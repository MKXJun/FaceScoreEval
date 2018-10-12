//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: FaceImage.h
// 
// Author: X_Jun
//
// OS:Windows 10
// IDE: Visual Studio 2015 Pro
// Lib: Opencv3.2.0, dlib_v19.1, STASM
//
// Description: 用于抓取位图中的脸部，并得到特征点集合，可以保存图片到本地
//          
//////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _FACEIMAGE_H_
#define _FACEIMAGE_H_
#include "FeaturePointSet.h"

namespace Face
{



class FaceImage
{
public:
	static const int faceWidth = 600;		// 面部宽度
public:
	FaceImage();
	~FaceImage();

	// **********************************************
	// 功能: 打开未经处理过的脸型图片，进行标准化处理
	// 参数:
	//       [In]fileName 原始图片文件名
	// 返回值: 若成功加载，并找到脸部进行处理，则返回true
	bool OpenRawFaceImage(std::string fileName);

	bool OpenFaceImage(std::string fileName);


	// **********************************************
	// 功能: 保存处理后的脸型图片
	// 参数:
	//       [In]fileName 要保存的文件名(新建)
	// 返回值: 保存成功，则返回true
	bool SaveFaceImage(std::string fileName);
private:
	// **********************************************
	// 功能: 将脸型图片和特征点集合标准化
	void Normalize();

public:
	cv::Mat faceMat;	// 抓取后经过标准化的脸部图片
	FeaturePointSet pointSet;	// 经过标准化的特征点集合
};





}	// namespace Face





#endif
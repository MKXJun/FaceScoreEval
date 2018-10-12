#pragma once
//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: FaceColorCaculator.h
// 
// Author: Chitose Ruri
//
// OS:Windows 10
// IDE: Visual Studio 2017
// Lib: Opencv3.2.0, dlib_v19.1, STASM
//
// Description: 计算人脸的肤色
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

//定义黑色数0~255，0为白，128为黄，255为黑

#include "FaceImage.h"

namespace Face
{
	class FaceColorCaculator
	{
	public:
		FaceColorCaculator();
		~FaceColorCaculator();
		int Caculate(FaceImage & fImg);
	
	private:
		bool PointInPolygon(cv::Point2f point, std::vector<cv::Point2f> & polygonPoi);
	};
}
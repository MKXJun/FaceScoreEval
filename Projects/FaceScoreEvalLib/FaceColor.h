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
// Description: ���������ķ�ɫ
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

//�����ɫ��0~255��0Ϊ�ף�128Ϊ�ƣ�255Ϊ��

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
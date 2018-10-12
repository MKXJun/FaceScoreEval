//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: noseDetection.h
// 
// Author: Rinzic
//
// OS:Windows 10
// IDE: Visual Studio 2015 Pro
// Lib: Opencv3.2, Dlib ,STASM
//
// Description: to detect the height of nose
//          
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef NOSE_DETECTION
#define NOSE_DETECTION

#include "FeaturePointSet.h"



class noseDetection
{
public:
	noseDetection(const std::vector<cv::Point2f>& dlibPoints, const std::vector<cv::Point2f>& stasmPoints,
		const cv::Mat& _image);

	void Detect();


	float DIF = 0;//Æ«ÀëÖµ
private:
	cv::Mat image;
	cv::Point2f noseData[8];
};


#endif //NOSE_DETECTION

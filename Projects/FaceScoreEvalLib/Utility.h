//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// ͷ�ļ���: Utility.h
// 
// ����: X_Jun
//
// ʹ�ÿ�: Opencv3.2, Dlib, STASM
//
// ����: ���һЩ��������ʶ��ĺ�������
//
// ��ע����Ҫ֧��C++11
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "FeaturePointSet.h"


// ʹ��STASM����Ƿ��������
bool detectFaceByStasm(const cv::Mat& inputImage, cv::Rect * pArea);


// **********************************************
// ����: ʹ��STASM��ȡ������
// ����:   
//       [In]stasmImage ����������λͼ 
//       [Out]stasmPoint ���ڻ�ȡStasm������������
// ����ֵ: �Ƿ���⵽����
bool GetStasmPoint(const cv::Mat1b& stasmImage, std::vector<cv::Point2f>& stasmPoint);


// **********************************************
// ����: ʹ��DLIB��ȡ������
// ����:   
//       [In]dlibImage ����������λͼ 
//       [Out]dlibPoint ���ڻ�ȡDlib������������
// ����ֵ: �Ƿ���⵽����
bool GetDlibPoint(const cv::Mat& dlibImage, std::vector<cv::Point2f>& dlibPoint);


//// ��ͼ�ϻ��ƶ�Ӧ����������
//void drawPointNumber(const char* winName, const char* path);
//
//// ��ͼ�ϻ���������
//void drawPoint(const char* winName, const char * path);



// **********************************************
// ����: ʹ��Stasm��Dlibʶ����������ȡ�����㼯��
// ����:   
//       [In]Path ����������λͼ�ļ�·�� 
//       [Out]PointSet �������������
// ����ֵ: �Ƿ�ʶ�������������������㼯��
bool ExtractFeaturePoint(const char* Path, Face::FeaturePointSet* PointSet);


/**
*	��ȡ 2 ά�ռ����ڶ���ɢ��Ķ���ʽ������ߡ�������С���˷�����ʽ�������ԭ���ȡ��
*	@ PointSet - ��ɢ�㼯�ϡ�
*	@ Order - ����ʽ�е���߽�����
*	@ return - ����ʽϵ�� a ���ϡ���y = a0 + a1*x + a2*x^2 + ...��
*   ע�⣺Order��ֵ��ΧΪ[0, numPoints - 1]
*/
std::vector<float> GetMultinomialCoefficient(const std::vector<cv::Point2f> & PointSet, int Order);


#endif

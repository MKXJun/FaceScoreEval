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
// Description: ����ץȡλͼ�е����������õ������㼯�ϣ����Ա���ͼƬ������
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
	static const int faceWidth = 600;		// �沿���
public:
	FaceImage();
	~FaceImage();

	// **********************************************
	// ����: ��δ�������������ͼƬ�����б�׼������
	// ����:
	//       [In]fileName ԭʼͼƬ�ļ���
	// ����ֵ: ���ɹ����أ����ҵ��������д����򷵻�true
	bool OpenRawFaceImage(std::string fileName);

	bool OpenFaceImage(std::string fileName);


	// **********************************************
	// ����: ���洦��������ͼƬ
	// ����:
	//       [In]fileName Ҫ������ļ���(�½�)
	// ����ֵ: ����ɹ����򷵻�true
	bool SaveFaceImage(std::string fileName);
private:
	// **********************************************
	// ����: ������ͼƬ�������㼯�ϱ�׼��
	void Normalize();

public:
	cv::Mat faceMat;	// ץȡ�󾭹���׼��������ͼƬ
	FeaturePointSet pointSet;	// ������׼���������㼯��
};





}	// namespace Face





#endif
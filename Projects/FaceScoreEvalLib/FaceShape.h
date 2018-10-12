//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: FaceShape.h
// 
// Author: X_Jun
//
// OS:Windows 10
// IDE: Visual Studio 2015 Pro
// Lib: Opencv3.2.0
//
// Description: �����ж�����
//          
//////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _FACESHAPE_H_
#define _FACESHAPE_H_
#include "FaceImage.h"

namespace Face
{
	enum FACE_SHAPE
	{
		FACE_SHAPE_UNKNOWN,		// δ֪
		FACE_SHAPE_OVAL,		// ��Բ��
		FACE_SHAPE_SQUARE,		// ������
		FACE_SHAPE_ROUND,		// Բ��
		FACE_SHAPE_TRIANGLE,	// ��������
		FACE_SHAPE_OBLONG,		// ��������
	};

	class FaceShape
	{
	public:
		

	public:
		FaceShape(const FaceImage& faceImage);
		~FaceShape();

		// **********************************************
		// ����: ��ȡ����ȧ�ǿ��
		// ����ֵ: ȧ�ǿ��
		float GetCheekBoneWith() const;

		// **********************************************
		// ����: ��ȡ���������
		// ����ֵ: �����
		float GetJawWidth() const;

		// **********************************************
		// ����: ��ȡ�����߶�
		// ����ֵ: �����߶�
		int GetHeight() const;
		
		// **********************************************
		// ����: ����°����߷��̵�ϵ��(ʹ������4-12��������)
		// ����: ��Ҫ����ߴ���(��������9����)
		// ����ֵ: 0���N����ĸ���vector
		std::vector<float> GetJawCurveCoefficient(int N);

		// **********************************************
		// ����: ��ȡ����
		// ����ֵ: ����ö��
		FACE_SHAPE GetFaceShape();
	private:
		const FaceImage& faceImage;
		FACE_SHAPE shape;	// ����

	};

}	// namespace Face





#endif
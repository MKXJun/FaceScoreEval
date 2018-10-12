//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: FaceScore.h
// 
// Author: Rinzic
//
// OS:Windows 10
// IDE: Visual Studio 2015 Pro
// Lib: Opencv3.2.0, dlib_v19.1, STASM
//
// Description: �����÷�����
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _FACESCORE_H_
#define _FACESCORE_H_

#include "FaceProportion.h"


namespace Face
{





struct FaceScore
{
	// ���ֱ�׼
	static const FaceScore Standard;

	//���������ָ������������ķ���
	float s_fEg, s_eEh, s_hEi, s_gEi;
	float s_lEb, s_mEl, s_kEe;
	float s_nEl, s_qEo, s_rEp, s_nEh;
	float s_eyeBrow;
	float s_leyeBrow, s_reyeBrow;
};


//���ü����������ֵĽӿ�
float CountFaceScore(const FeaturePointSet& nor);

//�ۺϸ����ǵı�����ȡ����ʵ�ƽ��ֵ
FaceScore CountProportionScore(const FaceProportion& data);



}	// namespace Face



#endif
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
// Description: 人脸得分评估
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _FACESCORE_H_
#define _FACESCORE_H_

#include "FaceProportion.h"


namespace Face
{





struct FaceScore
{
	// 评分标准
	static const FaceScore Standard;

	//保存各评分指标种类算出来的分数
	float s_fEg, s_eEh, s_hEi, s_gEi;
	float s_lEb, s_mEl, s_kEe;
	float s_nEl, s_qEo, s_rEp, s_nEh;
	float s_eyeBrow;
	float s_leyeBrow, s_reyeBrow;
};


//调用计算脸部评分的接口
float CountFaceScore(const FeaturePointSet& nor);

//综合各明星的比例，取最合适的平均值
FaceScore CountProportionScore(const FaceProportion& data);



}	// namespace Face



#endif
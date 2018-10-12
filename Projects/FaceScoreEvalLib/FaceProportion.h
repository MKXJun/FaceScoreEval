//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 头文件名: FaceProportion.h
// 
// 作者: X_Jun
//
// 使用库: Opencv3.2, Dlib, STASM
//
// 描述: 脸部各特征比例数据
//
// 备注：需要支持C++11
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _FACEPROPORTION_H_
#define _FACEPROPORTION_H_

#include "FeaturePointSet.h"


namespace Face
{





struct FaceProportion
{

	// **********************************************
	// 功能: 获取特征点集中各部分比例
	// 参数:
	//		 [In]pointSet 特征点集合
	FaceProportion(const FeaturePointSet& pointSet);


	// **********************************************
	// 功能: 构造函数
	FaceProportion();
	

	// **********************************************
	// 功能: 获取特征点集中各部分比例
	// 参数:
	//		 [In]pointSet 特征点集合
	void Count(const FeaturePointSet& pointSet);

	//各评分指标种类
	float fEg, eEh, hEi, gEi;
	float lEb, mEl, kEe;
	float nEl, qEo, rEp, nEh;
	float rvwEr, rs, rjEu;
	float lvwEr, ls, ljEu;
};





}


#endif

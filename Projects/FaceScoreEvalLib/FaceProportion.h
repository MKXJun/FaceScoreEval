//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// ͷ�ļ���: FaceProportion.h
// 
// ����: X_Jun
//
// ʹ�ÿ�: Opencv3.2, Dlib, STASM
//
// ����: ������������������
//
// ��ע����Ҫ֧��C++11
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _FACEPROPORTION_H_
#define _FACEPROPORTION_H_

#include "FeaturePointSet.h"


namespace Face
{





struct FaceProportion
{

	// **********************************************
	// ����: ��ȡ�����㼯�и����ֱ���
	// ����:
	//		 [In]pointSet �����㼯��
	FaceProportion(const FeaturePointSet& pointSet);


	// **********************************************
	// ����: ���캯��
	FaceProportion();
	

	// **********************************************
	// ����: ��ȡ�����㼯�и����ֱ���
	// ����:
	//		 [In]pointSet �����㼯��
	void Count(const FeaturePointSet& pointSet);

	//������ָ������
	float fEg, eEh, hEi, gEi;
	float lEb, mEl, kEe;
	float nEl, qEo, rEp, nEh;
	float rvwEr, rs, rjEu;
	float lvwEr, ls, ljEu;
};





}


#endif

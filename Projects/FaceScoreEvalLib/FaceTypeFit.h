//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: FaceTypeFit.h
// 
// Author: Rinzic
//
// OS:Windows 10
// IDE: Visual Studio 2015 Pro
// Lib: Opencv3.2.0, dlib_v19.1, STASM
//
// Description: ������С���˷������ݺ���������͵���
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FACE_TYPE_FIT
#define FACE_TYPE_FIT

#include "FeaturePointSet.h"

namespace Face
{ 

/** �°����� */
enum JawType
{
	SQUARE_FACE,//������
	OVAL_FACE	//������
};

/**
 *	�����������б��°͵�������
 */
struct FaceTypeFit
{
public:

	/**
	 *	���ݴ������������������°����͡�
	 *	@ PointSet - �����㼯�ϡ�
	 *	@ return - �°����͡�
	 */
	static JawType AnalyzeJawType(const FeaturePointSet& PointSet);

	//FaceTypeFit(const std::vector<cv::Point2f>& dlibPoints, const std::vector<cv::Point2f>& stasmPoints)
	//	: nor(dlibPoints, stasmPoints)
	//{
	//	distinguish();
	//}
	
	FeaturePointSet nor;//��һ����������������
	float a = 1;		//������ϵ��ݺ����Ĳ���(a>0)
	unsigned int k = 2;	//������ϵ��ݺ����Ľ�(k>=2)
	float deviation = INFINITY;//��С���˷���ƫ�0=< deviation <INFINITY��
	enum FaceType
	{
		SQUARE_FACE,//������
		OVAL_FACE	//������
	}faceType;

	//methods
	void fitting(bool flag = false, int startLabel = 0, int endLabel = 16);

	/**
	 *	�����ݶ���
	 *	@ return - �����ݶ�
	 */
	FaceType distinguish();
};



}	// namespace Face


#endif //FACE_TYPE_FIT
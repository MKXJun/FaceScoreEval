//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// ͷ�ļ���: FeaturePointSet.h
// 
// ����: X_Jun
//
// ʹ�ÿ�: Opencv3.2, Dlib, STASM
//
// ����: �����㼯���࣬���ڽ�Dlib��Stasm�������㼯�����л����
//
// ��ע����Ҫ֧��C++11
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _FEATUREPOINTSET_H_
#define _FEATUREPOINTSET_H_

#include <iostream>
#include <vector>
#include <string>
#include <opencv2/core.hpp>

namespace Face
{



class FeaturePointSet
{
public:
	static const float face_width;		// �沿���600.0
	static const int combineLandMarks = 87;	// ��Ϻ����������Ŀ87
											// 0-16 ����
											// 17-19 ����
											// 20-25 ��üë
											// 26-31 ��üë
											// 32-33 ����
											// 34-42 ����
											// 43-51 ����
											// 52-62 83-87 ����+����
											// 63-82 ���

public:
	// **********************************************
	// ����: ʹ��һ�����������㼯�ϵ������ʼ��
	// ����: 
	//		 [In]Ԫ�ظ���ΪcombineLandMarks������������
	FeaturePointSet(const cv::Point2f(&pointSet)[combineLandMarks]);

	// **********************************************
	// ����: ʹ��һ�����������㼯�ϵ������ʼ��
	// ����: 
	//		 [In]Ԫ�ظ���ΪcombineLandMarks������������
	FeaturePointSet(const std::vector<cv::Point2f>& pointSet);

	// **********************************************
	// ����: ʹ��һ�����������㼯�ϵ������ʼ��
	// ����: 
	//		 [In]Ԫ�ظ���ΪcombineLandMarks������������
	FeaturePointSet(std::vector<cv::Point2f>&& pointSet);

	FeaturePointSet(const FeaturePointSet&) = default;

	// **********************************************
	// ����: ��stasm�ĵ㼯��dlib�ĵ㼯�л������һ��
	// ����:
	//		 [In]dlibPoints Dlib��ʶ������������㼯
	//		 [In]stasmPoints Stasm��ʶ������������㼯
	//       [In]useNormalize �Ƿ��ڹ���ʱ���б�׼��
	FeaturePointSet(const std::vector<cv::Point2f>& dlibPoints, const std::vector<cv::Point2f>& stasmPoints);

	// **********************************************
	// ����: �չ��췽��
	FeaturePointSet();
	
	// **********************************************
	// ����: �ƶ����췽��
	FeaturePointSet(FeaturePointSet&& pointSet);


	// **********************************************
	// ����: ��������
	~FeaturePointSet();

	// **********************************************
	// ����: ��stasm�ĵ㼯��dlib�ĵ㼯�л������һ��
	//       �����Ϊδ���״̬
	// ����:
	//		 [In]dlibPoints Dlib��ʶ������������㼯
	//		 [In]stasmPoints Stasm��ʶ������������㼯
	// ����ֵ: �ϲ��Ƿ�ɹ�
	bool Merge(const std::vector<cv::Point2f>& dlibPoints, const std::vector<cv::Point2f>& stasmPoints);

	// **********************************************
	// ����: ��ȡ��Ӧ������������
	// ����:
	//		 [In]pos ������
	// ����ֵ: ��Ӧ������������
	const cv::Point2f& operator[](size_t pos) const;

	// **********************************************
	// ����: ��ȡ��Ӧ������������
	// ����:
	//		 [In]pos ������
	// ����ֵ: ��Ӧ������������
	cv::Point2f& operator[](size_t pos);

	// **********************************************
	// ����: ��ȡ����������
	std::vector<cv::Point2f>& GetData();

	// **********************************************
	// ����: ��ȡ����������
	const std::vector<cv::Point2f>& GetData() const;

	// **********************************************
	// ����: ��ȡ���������ϵĵ�
	// ����:
	//       [In]pointSet �����㼯��
	// ����ֵ: һϵ�������ϵĵ�
	std::vector<cv::Point2f> GetFaceMiddleLinePoints() const;

	// **********************************************
	// ����: ��ȡ������߽����
	// ����ֵ: ������߽����
	cv::Rect2f GetBorder() const;


private:
	std::vector<cv::Point2f> data;		// ��Ϻ���Զ��������㼯��
};

// **********************************************
// ����: �ض���ʽ�����
// ����:
//		 [InOut]os �����
//       [In]p     Ҫ����ĵ�
// ����ֵ: ʹ�õ������
// ��ע: �����ʽ�� (2.300000, 1.457938)
std::ostream& operator<<(std::ostream& os, const cv::Point2f& p);


}	// namespace Face










#endif
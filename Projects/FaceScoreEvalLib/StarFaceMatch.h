//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// ͷ�ļ���: StarFaceMatch.h
// 
// ����: X_Jun
//
// ʹ�ÿ�: Opencv3.2, Dlib, STASM
//
// ����: �������������ϣ������ڼ�⵱ǰ�������ĸ����ǱȽ�����
//
// ��ע����Ҫ֧��C++11
//////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _STARFACEMATCH_H_
#define _STARFACEMATCH_H_

#include "FaceScore.h"

namespace Face
{

struct MatchInfo
{
	std::string starName;	// ������
	float similarRate;		// ���ƶ�
};


// **********************************************
// ����: �����������������������ݿ�
// ����:
//       [In]sourceFolderPath ������ͼƬ���ڵ�Դ�ļ���(�Ѿ��任�õ�)
//       [In]destFilePath Ŀ�����·������ָ���ļ�������Ϊ.dat
// ����ֵ: ���ݿ��Ƿ񹹽��ɹ�
// ��ע: ������ͼƬ�ļ����ڲ�����ͼƬ����ļ�
//       �������·��������ͼƬ�ļ�����
bool BuildStarFaceDatabase(const char* sourceDirectory, const char* destDataFilePath);

// **********************************************
// ����: ���������������������ݿ�����Ѱ���Ƶ�����
// ����:
//       [In]sourceDataFilePath �������������������ݿ�
//       [In]facePro ���͸����ֱ�������
//       [Out]matchResult ƥ����
// ����ֵ: ���ɹ����ƥ�����򷵻�true
bool SearchSimilarStarFace(const char* sourceDataFilePath, const Face::FaceProportion& facePro, MatchInfo* matchResult);

// **********************************************
// ����: ������͸����ֱ���������
// ����:
//       [In]pointSet ����
// ����ֵ: �������͸����ֱ�������
FaceProportion CountProportionData(const FeaturePointSet& pointSet);


// **********************************************
// ����: ��ȡ�ļ���������ļ���
// ����: Chitose Ruri
// ����:
//       [In]path Ŀ���ļ���
//       [Out]fileNames �����ļ������ļ�����0�±�����ϲ�Ŀ¼(**/)����Ҫʹ��
// ����ֵ: �������
bool GetFilesNames(std::string directory, std::vector<std::string> & fileNames);



/**
	*	@ ���� - ��ȡ Output\Source �ļ�����ͼƬ��Ϣ��������ȡ�������ݲ������ Output\Transform �С�
	*	@ ���� - HansonLost
	*/
void OutputTransform();

// **********************************************
// ����: ��ȡ�ļ���������ļ���
// ����: Chitose Ruri
// ����:
//       [In]path stars�ļ��е����·��,�磨\star��
// ����ֵ: �������
bool CheckSourceAndTransform(std::string path);


}	// namespace Face





#endif
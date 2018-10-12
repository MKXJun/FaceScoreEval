#include "FaceP.h"
#include "FeaturePointSet.h"


namespace Face
{

const float FeaturePointSet::face_width = 600.0f;	// �沿���600.0
const int FeaturePointSet::combineLandMarks;		// ��Ϻ����������Ŀ87

// **********************************************
// ����: ʹ��һ�����������㼯�ϵ������ʼ��
// ����: 
//		 [In]Ԫ�ظ���ΪcombineLandMarks������������
FeaturePointSet::FeaturePointSet(const cv::Point2f(&pointSet)[combineLandMarks])
	: data(std::begin(pointSet), std::end(pointSet))
{
}

// **********************************************
// ����: ʹ��һ�����������㼯�ϵ������ʼ��
// ����: 
//		 [In]Ԫ�ظ���ΪcombineLandMarks������������
FeaturePointSet::FeaturePointSet(const std::vector<cv::Point2f>& pointSet)
{
	if (pointSet.size() != combineLandMarks)
	{
		std::wstring errorMessage = L"�������������Ŀ��Ϊ" + std::to_wstring(combineLandMarks);
		_wassert(errorMessage.c_str(), _CRT_WIDE(__FILE__), (unsigned)__LINE__);
	}
	data = pointSet;
}

FeaturePointSet::FeaturePointSet(std::vector<cv::Point2f>&& pointSet)
{
	if (pointSet.size() != combineLandMarks)
	{
		std::wstring errorMessage = L"�������������Ŀ��Ϊ" + std::to_wstring(combineLandMarks);
		_wassert(errorMessage.c_str(), _CRT_WIDE(__FILE__), (unsigned)__LINE__);
	}
	data.swap(pointSet);
}


// **********************************************
// ����: ��stasm�ĵ㼯��dlib�ĵ㼯�л������һ��
// ����:
//		 [In]dlibPoints Dlib��ʶ������������㼯
//		 [In]stasmPoints Stasm��ʶ������������㼯
//       [In]useNormalize �Ƿ��ڹ���ʱ���б�׼��
FeaturePointSet::FeaturePointSet(const std::vector<cv::Point2f>& dlibPoints, const std::vector<cv::Point2f>& stasmPoints)
{
	Merge(dlibPoints, stasmPoints);
}

// **********************************************
// ����: �չ��췽��
FeaturePointSet::FeaturePointSet()
{
}

// **********************************************
// ����: �ƶ����췽��
FeaturePointSet::FeaturePointSet(FeaturePointSet && pointSet)
{
	std::swap(data, pointSet.data);
}



// **********************************************
// ����: ��������
FeaturePointSet::~FeaturePointSet()
{
}

// **********************************************
// ����: ��stasm�ĵ㼯��dlib�ĵ㼯�л������һ��
//       �����Ϊδ���״̬
// ����:
//		 [In]dlibPoints Dlib��ʶ������������㼯
//		 [In]stasmPoints Stasm��ʶ������������㼯
// ����ֵ: �ϲ��Ƿ�ɹ�
bool FeaturePointSet::Merge(const std::vector<cv::Point2f>& dlibPoints, const std::vector<cv::Point2f>& stasmPoints)
{
	if (dlibPoints.empty() || stasmPoints.empty()) {
		return false;
	}
	data.clear();

	for (int i = 0; i < 17; ++i)
		data.push_back(dlibPoints[i]);
	for (int i = 13; i < 59; ++i)
		data.push_back(stasmPoints[i]);
	for (int i = 48; i < 68; ++i)
		data.push_back(dlibPoints[i]);
	for (int i = 27; i < 31; ++i)
		data.push_back(dlibPoints[i]);

	return true;
}

// **********************************************
// ����: ��ȡ��Ӧ������������
// ����:
//		 [In]pos ������
// ����ֵ: ��Ӧ������������
const cv::Point2f & FeaturePointSet::operator[](size_t pos) const
{
	return data[pos];
}



cv::Point2f & FeaturePointSet::operator[](size_t pos)
{
	return data[pos];
}



// **********************************************
// ����: ��ȡ����������
std::vector<cv::Point2f>& FeaturePointSet::GetData()
{
	return data;
}


// **********************************************
// ����: ��ȡ����������
const std::vector<cv::Point2f>& FeaturePointSet::GetData() const
{
	return data;
}

// **********************************************
// ����: ��ȡ���������ϵĵ�
// ����:
//       [In]pointSet �����㼯��
// ����ֵ: һϵ�������ϵĵ�
std::vector<cv::Point2f> FeaturePointSet::GetFaceMiddleLinePoints() const
{
	if (data.empty() == false)
	{
		// ���ȷ�����Щ�������ϵĵ�
		std::vector<cv::Point2f> points{data[8], data[60],
			data[56], data[53], data[18], data[83], data[84], data[85], data[86]
		};
		// Ȼ�������ߵĸ���õ���Ӧ���е㼯��
		// 0-7 16-9
		for (int i = 0; i < 8; ++i)
			points.push_back((data[i] + data[16 - i]) / 2);
		// 17 19
		points.push_back((data[17] + data[19]) / 2);
		return points;

	}
	else
		return std::vector<cv::Point2f>{};
}

cv::Rect2f FeaturePointSet::GetBorder() const
{
	float left, top, right, bottom;
	left = data[0].x;
	top = data[0].y;
	right = data[0].x;
	bottom = data[0].y;
	for (const cv::Point2f& point : data)
	{
		left = std::min(left, point.x);
		top = std::min(top, point.y);
		right = std::max(right, point.x);
		bottom = std::max(bottom, point.y);
	}
	return cv::Rect2f(left, top, right - left, bottom - top);
}


// **********************************************
// ����: �ض���ʽ�����
// ����:
//		 [InOut]os �����
//       [In]p     Ҫ����ĵ�
// ����ֵ: ʹ�õ������
// ��ע: �����ʽ�� (2.300000, 1.457938)
std::ostream & operator<<(std::ostream & os, const cv::Point2f & p)
{
	return os << "(" << p.x << "," << p.y << ")";
}


}	// namespace Face
#include "FaceP.h"
#include "FeaturePointSet.h"


namespace Face
{

const float FeaturePointSet::face_width = 600.0f;	// 面部宽度600.0
const int FeaturePointSet::combineLandMarks;		// 结合后的特征点数目87

// **********************************************
// 功能: 使用一个包含特征点集合的数组初始化
// 参数: 
//		 [In]元素个数为combineLandMarks的特征点数组
FeaturePointSet::FeaturePointSet(const cv::Point2f(&pointSet)[combineLandMarks])
	: data(std::begin(pointSet), std::end(pointSet))
{
}

// **********************************************
// 功能: 使用一个包含特征点集合的数组初始化
// 参数: 
//		 [In]元素个数为combineLandMarks的特征点数组
FeaturePointSet::FeaturePointSet(const std::vector<cv::Point2f>& pointSet)
{
	if (pointSet.size() != combineLandMarks)
	{
		std::wstring errorMessage = L"传入的特征点数目不为" + std::to_wstring(combineLandMarks);
		_wassert(errorMessage.c_str(), _CRT_WIDE(__FILE__), (unsigned)__LINE__);
	}
	data = pointSet;
}

FeaturePointSet::FeaturePointSet(std::vector<cv::Point2f>&& pointSet)
{
	if (pointSet.size() != combineLandMarks)
	{
		std::wstring errorMessage = L"传入的特征点数目不为" + std::to_wstring(combineLandMarks);
		_wassert(errorMessage.c_str(), _CRT_WIDE(__FILE__), (unsigned)__LINE__);
	}
	data.swap(pointSet);
}


// **********************************************
// 功能: 将stasm的点集与dlib的点集有机结合在一起
// 参数:
//		 [In]dlibPoints Dlib库识别的人脸特征点集
//		 [In]stasmPoints Stasm库识别的人脸特征点集
//       [In]useNormalize 是否在构造时进行标准化
FeaturePointSet::FeaturePointSet(const std::vector<cv::Point2f>& dlibPoints, const std::vector<cv::Point2f>& stasmPoints)
{
	Merge(dlibPoints, stasmPoints);
}

// **********************************************
// 功能: 空构造方法
FeaturePointSet::FeaturePointSet()
{
}

// **********************************************
// 功能: 移动构造方法
FeaturePointSet::FeaturePointSet(FeaturePointSet && pointSet)
{
	std::swap(data, pointSet.data);
}



// **********************************************
// 功能: 析构函数
FeaturePointSet::~FeaturePointSet()
{
}

// **********************************************
// 功能: 将stasm的点集与dlib的点集有机结合在一起
//       并标记为未规格化状态
// 参数:
//		 [In]dlibPoints Dlib库识别的人脸特征点集
//		 [In]stasmPoints Stasm库识别的人脸特征点集
// 返回值: 合并是否成功
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
// 功能: 获取对应索引的特征点
// 参数:
//		 [In]pos 点索引
// 返回值: 对应索引的特征点
const cv::Point2f & FeaturePointSet::operator[](size_t pos) const
{
	return data[pos];
}



cv::Point2f & FeaturePointSet::operator[](size_t pos)
{
	return data[pos];
}



// **********************************************
// 功能: 获取特征点数组
std::vector<cv::Point2f>& FeaturePointSet::GetData()
{
	return data;
}


// **********************************************
// 功能: 获取特征点数组
const std::vector<cv::Point2f>& FeaturePointSet::GetData() const
{
	return data;
}

// **********************************************
// 功能: 获取脸部中线上的点
// 参数:
//       [In]pointSet 特征点集合
// 返回值: 一系列中线上的点
std::vector<cv::Point2f> FeaturePointSet::GetFaceMiddleLinePoints() const
{
	if (data.empty() == false)
	{
		// 首先放置那些在中线上的点
		std::vector<cv::Point2f> points{data[8], data[60],
			data[56], data[53], data[18], data[83], data[84], data[85], data[86]
		};
		// 然后用两边的各点得到对应的中点集合
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
// 功能: 特定形式输出点
// 参数:
//		 [InOut]os 输出流
//       [In]p     要输出的点
// 返回值: 使用的输出流
// 备注: 输出形式如 (2.300000, 1.457938)
std::ostream & operator<<(std::ostream & os, const cv::Point2f & p)
{
	return os << "(" << p.x << "," << p.y << ")";
}


}	// namespace Face
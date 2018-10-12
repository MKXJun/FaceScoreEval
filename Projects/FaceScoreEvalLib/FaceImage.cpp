#include "FaceP.h"
#include "FaceImage.h"
#include "Utility.h"

namespace Face
{

FaceImage::FaceImage()
{
}


FaceImage::~FaceImage()
{
}

bool FaceImage::OpenRawFaceImage(std::string fileName)
{
	// 读取原始图像，并获取灰度图
	
	faceMat = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
	cv::Mat grayImage;
	// 转换错误需要提前停止
	try
	{
		cv::cvtColor(faceMat, grayImage, CV_BGR2GRAY);
	}
	catch (const cv::Exception&)
	{
		return false;
	}
	
	// 根据灰度图获取 Stasm 与 Dlib 的特征点。
	std::vector<cv::Point2f> stasmPointSet;
	std::vector<cv::Point2f> dlibPointSet;
	// 若抓取不到特征点则读取失败
	if (!GetStasmPoint(grayImage, stasmPointSet) || !GetDlibPoint(faceMat, dlibPointSet))
	{
		return false;
	}
	// 获取原始特征点集合
	pointSet.Merge(dlibPointSet, stasmPointSet);
	
	// 标准化处理
	Normalize();

	return true;
}


bool FaceImage::SaveFaceImage(std::string fileName)
{
	return cv::imwrite(fileName, faceMat);
}

// **********************************************
// 功能: 将脸型图片和特征点集合标准化
void FaceImage::Normalize()
{
	// 线性回归分隔脸部
	std::vector<cv::Point2f> points = pointSet.GetFaceMiddleLinePoints();
	cv::Vec4f res;
	cv::fitLine(points, res, cv::DIST_L2, 0, 1e-2, 1e-2);
	// 用更清晰的含义来表述，idir和jdir为单位向量的i,j分量
	float idir = res[0], jdir = res[1], x0 = res[2], y0 = res[3];
	cv::Point2f p1, p2, center;


	// 先用上额和下颚的点求平均值
	center = (pointSet[8] + pointSet[18]) / 2;
	// 若脸基本上已经对齐
	if (idir == 0 || fabs(idir / jdir) < 1e-3)
	{
		center.x = x0;
	}
	// 若脸旋转了90度左右
	else if (jdir == 0 || fabs(jdir / idir) < 1e-3)
	{
		center.y = y0;
	}
	// 否则重新预估回归直线所在中心位置
	else
	{
		float k = jdir / idir;
		float b = -k * x0 + y0;
		center.x = (1 / k * center.x + center.y - b) / (k + 1 / k);
		center.y = k * center.x + b;
	}
	

	// 创建一个较大的位图，将原来的位图放到中间以便于进行旋转和后续的截取操作
	// 中心位置对应图片正中心
	cv::Mat rotatedImage((int)(faceMat.rows * 1.4), (int)(faceMat.cols * 1.4), CV_8UC3);
	cv::Rect copyArea(
		(int)(rotatedImage.cols / 2 - center.x),
		(int)(rotatedImage.rows / 2 - center.y),
		std::min(faceMat.cols, rotatedImage.cols / 2 + (int)center.x),
		std::min(faceMat.rows, rotatedImage.rows / 2 + (int)center.y)
	);
	// 划定复制区域
	cv::Mat roi = rotatedImage(copyArea);
	copyArea.x = copyArea.y = 0;
	cv::Mat faceRoi = faceMat(copyArea);
	// 图复制过去
	cv::addWeighted(faceRoi, 1.0, roi, 0.0, 0.0, roi);
	// 计算变换位图中对应的中心位置
	cv::Point2f rotationCenter = { rotatedImage.cols / 2.0f, rotatedImage.rows / 2.0f };
	// 计算倾斜角，idir一定为正的，所以要判断jdir的正负
	float sinTheta = idir / sqrt(idir * idir + jdir * jdir) * (jdir > 0.0f ? 1.0f : -1.0f);
	double angle = asin(sinTheta) / CV_PI * 180;
	float rad = asinf(sinTheta);
	// 图片XY坐标系和标准的XY坐标系是有区别的，绕了X轴翻转
	// 所以现在需要将图片进行顺时针旋转
	cv::Mat R = cv::getRotationMatrix2D(rotationCenter, -angle, 1.0);
	
	
	// 开始仿射变换
	cv::warpAffine(rotatedImage, rotatedImage, R, rotatedImage.size());
	// 对所有点进行旋转变换，同时寻找边界
	for (cv::Point2f& point : pointSet.GetData())
	{
		point -= center;
		point = { 
			cosf(-rad) * point.x + sinf(-rad) * point.y,
			-sinf(-rad) * point.x + cosf(-rad) * point.y
		};
		point += center;
	}

	cv::Rect2f border = pointSet.GetBorder();

	// 求出缩放比例
	float scale = (float)faceWidth / border.width;
	// 重新计算缩放后的点
	for (cv::Point2f& point : pointSet.GetData())
	{
		point -= cv::Point2f(border.x, border.y);
		point *= scale;
	}
	// 映射到旋转图片位置
	border.x += rotatedImage.cols / 2 - center.x;
	border.y += rotatedImage.rows / 2 - center.y;

	// 重建脸部
	roi = rotatedImage(border);
	cv::resize(roi, faceMat, cv::Size(), scale, scale);
	
}


}	// namespace Face



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
	// ��ȡԭʼͼ�񣬲���ȡ�Ҷ�ͼ
	
	faceMat = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);
	cv::Mat grayImage;
	// ת��������Ҫ��ǰֹͣ
	try
	{
		cv::cvtColor(faceMat, grayImage, CV_BGR2GRAY);
	}
	catch (const cv::Exception&)
	{
		return false;
	}
	
	// ���ݻҶ�ͼ��ȡ Stasm �� Dlib �������㡣
	std::vector<cv::Point2f> stasmPointSet;
	std::vector<cv::Point2f> dlibPointSet;
	// ��ץȡ�������������ȡʧ��
	if (!GetStasmPoint(grayImage, stasmPointSet) || !GetDlibPoint(faceMat, dlibPointSet))
	{
		return false;
	}
	// ��ȡԭʼ�����㼯��
	pointSet.Merge(dlibPointSet, stasmPointSet);
	
	// ��׼������
	Normalize();

	return true;
}


bool FaceImage::SaveFaceImage(std::string fileName)
{
	return cv::imwrite(fileName, faceMat);
}

// **********************************************
// ����: ������ͼƬ�������㼯�ϱ�׼��
void FaceImage::Normalize()
{
	// ���Իع�ָ�����
	std::vector<cv::Point2f> points = pointSet.GetFaceMiddleLinePoints();
	cv::Vec4f res;
	cv::fitLine(points, res, cv::DIST_L2, 0, 1e-2, 1e-2);
	// �ø������ĺ�����������idir��jdirΪ��λ������i,j����
	float idir = res[0], jdir = res[1], x0 = res[2], y0 = res[3];
	cv::Point2f p1, p2, center;


	// �����϶�����ĵ���ƽ��ֵ
	center = (pointSet[8] + pointSet[18]) / 2;
	// �����������Ѿ�����
	if (idir == 0 || fabs(idir / jdir) < 1e-3)
	{
		center.x = x0;
	}
	// ������ת��90������
	else if (jdir == 0 || fabs(jdir / idir) < 1e-3)
	{
		center.y = y0;
	}
	// ��������Ԥ���ع�ֱ����������λ��
	else
	{
		float k = jdir / idir;
		float b = -k * x0 + y0;
		center.x = (1 / k * center.x + center.y - b) / (k + 1 / k);
		center.y = k * center.x + b;
	}
	

	// ����һ���ϴ��λͼ����ԭ����λͼ�ŵ��м��Ա��ڽ�����ת�ͺ����Ľ�ȡ����
	// ����λ�ö�ӦͼƬ������
	cv::Mat rotatedImage((int)(faceMat.rows * 1.4), (int)(faceMat.cols * 1.4), CV_8UC3);
	cv::Rect copyArea(
		(int)(rotatedImage.cols / 2 - center.x),
		(int)(rotatedImage.rows / 2 - center.y),
		std::min(faceMat.cols, rotatedImage.cols / 2 + (int)center.x),
		std::min(faceMat.rows, rotatedImage.rows / 2 + (int)center.y)
	);
	// ������������
	cv::Mat roi = rotatedImage(copyArea);
	copyArea.x = copyArea.y = 0;
	cv::Mat faceRoi = faceMat(copyArea);
	// ͼ���ƹ�ȥ
	cv::addWeighted(faceRoi, 1.0, roi, 0.0, 0.0, roi);
	// ����任λͼ�ж�Ӧ������λ��
	cv::Point2f rotationCenter = { rotatedImage.cols / 2.0f, rotatedImage.rows / 2.0f };
	// ������б�ǣ�idirһ��Ϊ���ģ�����Ҫ�ж�jdir������
	float sinTheta = idir / sqrt(idir * idir + jdir * jdir) * (jdir > 0.0f ? 1.0f : -1.0f);
	double angle = asin(sinTheta) / CV_PI * 180;
	float rad = asinf(sinTheta);
	// ͼƬXY����ϵ�ͱ�׼��XY����ϵ��������ģ�����X�ᷭת
	// ����������Ҫ��ͼƬ����˳ʱ����ת
	cv::Mat R = cv::getRotationMatrix2D(rotationCenter, -angle, 1.0);
	
	
	// ��ʼ����任
	cv::warpAffine(rotatedImage, rotatedImage, R, rotatedImage.size());
	// �����е������ת�任��ͬʱѰ�ұ߽�
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

	// ������ű���
	float scale = (float)faceWidth / border.width;
	// ���¼������ź�ĵ�
	for (cv::Point2f& point : pointSet.GetData())
	{
		point -= cv::Point2f(border.x, border.y);
		point *= scale;
	}
	// ӳ�䵽��תͼƬλ��
	border.x += rotatedImage.cols / 2 - center.x;
	border.y += rotatedImage.rows / 2 - center.y;

	// �ؽ�����
	roi = rotatedImage(border);
	cv::resize(roi, faceMat, cv::Size(), scale, scale);
	
}


}	// namespace Face



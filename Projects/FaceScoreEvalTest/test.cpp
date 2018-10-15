#include <filesystem>
#include <FaceScoreEval.h>
#include <opencv2/opencv.hpp>
#include "test.h"


using namespace cv;
using namespace std;
using namespace Face;
using namespace std::experimental;

void FaceTest::FaceColorTest()
{
	FaceImage img;
	img.OpenRawFaceImage("faceSamples/123.jpg");
	cv::imshow("test", img.faceMat);
	FaceColorCaculator fcc;
	std::cout << "肤色白色度：" << fcc.Caculate(img);
	cv::waitKey(0);
}

void FaceTest::FaceShapeTest()
{

	filesystem::path path("faceSamples\\");
	filesystem::directory_iterator iter(path), end;
	while (iter != end)
	{

		if (filesystem::is_regular_file(iter->status()))
		{
			FaceImage faceImage;
			faceImage.OpenRawFaceImage(iter->path().generic_string());
			if (faceImage.pointSet.GetData().empty())
				continue;

			FaceShape faceShape(faceImage);
			FACE_SHAPE shape = faceShape.GetFaceShape();
			auto vec = faceShape.GetJawCurveCoefficient(4);
			// 按一次项系数归一化
			float factor = vec[1];
			for (float& f : vec)
				f /= factor;
			printf("高/宽=%f ", (float)faceImage.faceMat.rows / faceImage.faceWidth);
			printf("1-4次项:%.12f %.12f %.12f %.12f ", vec[1], vec[2], vec[3], vec[4]);
			switch (shape)
			{
			case FACE_SHAPE_OBLONG: printf("长方脸\n"); break;
			case FACE_SHAPE_SQUARE: printf("国字脸\n"); break;
			case FACE_SHAPE_OVAL: printf("椭圆脸\n"); break;
			case FACE_SHAPE_ROUND: printf("圆脸\n"); break;
			case FACE_SHAPE_TRIANGLE: printf("倒三角脸\n"); break;
			}
			drawPointNumber("演示", iter->path().generic_string().c_str());
			cv::waitKey(0);
		}
		iter++;
	}
}

void FaceTest::StarFaceDatabaseTest()
{
	Face::OutputTransform();
}

void FaceTest::DetectFaceSpeedTest()
{
	Mat mat = imread("faceSamples/4.jpg");
	Rect rc;
	detectFaceByStasm(mat, &rc);
}

void drawPointNumber(const char* winName, const char* path)
{
	FaceImage faceImage;
	faceImage.OpenRawFaceImage(path);
	
	for (int i = 0; i < FeaturePointSet::combineLandMarks; i++)
	{
		
		circle(faceImage.faceMat, faceImage.pointSet[i], 3, cv::Scalar(0, 0, 255), -1);
		//circle(LinkImage, cvPoint(pointSet[i].x, pointSet[i].y), 1, cv::Scalar(0, 0, 255), -1);
		putText(faceImage.faceMat, to_string(i), faceImage.pointSet[i],
			CV_FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 255), 1);
		//if (i != FeaturePointSet::combineLandMarks - 1)
		//	line(LinkImage, cvPoint(pointSet[i].x, pointSet[i].y),
		//		cvPoint(pointSet[i + 1].x, pointSet[i + 1].y), Scalar(0, 0, 255), 2, 0);
	}
	
	

	//Display it all on the screen  
	cvNamedWindow(winName, cv::WINDOW_AUTOSIZE);
	imshow(winName, faceImage.faceMat);
}

void drawPoint(const char* winName, const char * path)
{
	FaceImage faceImage;
	faceImage.OpenRawFaceImage(path);
	cvNamedWindow(winName, cv::WINDOW_AUTOSIZE);
	cv::Mat showMat;
	faceImage.faceMat.copyTo(showMat);
	for (int i = 0; i < faceImage.pointSet.combineLandMarks; ++i)
		cv::circle(showMat, faceImage.pointSet[i], 1, cv::Scalar(0.0, 0.0, 0.0));
	cv::imshow(winName, showMat);
}
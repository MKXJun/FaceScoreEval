#include <FaceScoreEval.h>
//dlib
#include <dlib/opencv.h>  
#include <opencv2/opencv.hpp>  
#include <dlib/image_processing/frontal_face_detector.h>  
#include <dlib/image_processing/render_face_detections.h>  
#include <dlib/image_processing.h>  
#include <dlib/gui_widgets.h>  

//stasm
#include <stasm_lib.h>
#include <misc.h>
#include <MOD_1\facedet.h>
using namespace cv;
using namespace std;
using namespace Face;

// 侦测脸部
extern "C" __declspec(dllexport) bool __stdcall detectFace(uchar * data, int imageWidth, int imageHeight, int & x, int & y, int & w, int & h)
{
	// TODO: 改Size为摄像头像素
	Mat inputImage(Size(imageWidth, imageHeight), CV_8UC3);//RGB原图
	inputImage.data = data;
	Mat des;//灰度图
	static dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
	cvtColor(inputImage, des, CV_BGR2GRAY);

	// 抓取位图帧直到用户关闭主窗口
	dlib::cv_image<dlib::bgr_pixel> cimg(inputImage);
	// 侦测脸部位置
	std::vector<dlib::rectangle> faces = (detector)(cimg);
	if (faces.empty()) {
		return false;
	}
	else
	{
		// 如果侦测到脸部，绘制矩形区域
		if (detectFaceByStasm(inputImage, nullptr))
		{
			x = faces[0].left();
			y = faces[0].top();
			w = faces[0].width();
			h = faces[0].height();
			cv::rectangle(inputImage, Rect(x, y, w, h), Scalar(0, 255, 0));
		}
		else
			return false;
	}
	return true;
}

extern "C" __declspec(dllexport) bool __stdcall getScore(uchar* data, int imageWidth, int imageHeight, int &totalScore,
	int &faceScore, int &faceTypeScore, int &faceColorScore)
{
	vector<Point2f> stasmPoint, dlibPoint;
	//vector<dlib::rectangle> faces;
	//vector<dlib::full_object_detection> shape;
	FaceImage faceImage;
	faceImage.faceMat = Mat(Size(imageWidth, imageHeight), CV_8UC3, data);
	Mat tempImage(Size(imageWidth, imageHeight), CV_8UC3, data);
	
	cv::Mat_<unsigned char> stasmImage(Size(imageWidth, imageHeight));
	cvtColor(tempImage, stasmImage, CV_BGR2GRAY);
	
	if (!GetStasmPoint(stasmImage, stasmPoint) || !GetDlibPoint(faceImage.faceMat, dlibPoint))//获取特征点
	{
		return false;
	}
	// 合并特征点
	faceImage.pointSet.Merge(stasmPoint, dlibPoint);

	// 肤色计算
	FaceColorCaculator color;
	faceColorScore = color.Caculate(faceImage) * 100 / 128;

	// 脸型判断
	FaceShape shape(faceImage);
	FACE_SHAPE res = shape.GetFaceShape();
	switch (res)
	{
	case FACE_SHAPE_OVAL: faceTypeScore = 90; break;
	case FACE_SHAPE_TRIANGLE: faceTypeScore = 100; break;
	case FACE_SHAPE_OBLONG: faceTypeScore = 80; break;
	case FACE_SHAPE_ROUND: faceTypeScore = 70; break;
	case FACE_SHAPE_SQUARE: faceTypeScore = 60; break;
	case FACE_SHAPE_UNKNOWN: faceTypeScore = 0; break;
	}


	// 卢俊廷：这是我先注释掉的。
	//FaceTypeFit faceType(stasmPoint, dlibPoint);
	/*for (int i = 0; i < COMBINE_LANDKMARKS; i++)
	{
	circle(inputImage, cvPoint(normalize.data[i].getX(), normalize.data[i].getY()), 3, cv::Scalar(0, 0, 255), -1);
	}*/
	
	// 获取脸部比例分
	faceScore = Face::CountFaceScore(faceImage.pointSet);
	faceScore = (faceScore > 100 || faceScore < 60) ? rand() % 10 + 50 : faceScore * 6 / 10;
	// noseScore = nose.DIF + 0.6;
	totalScore = rand() % 10 + 50;
	totalScore += faceTypeScore * 2 / 10;
	totalScore += faceColorScore * 2 / 10;

	return true;
}
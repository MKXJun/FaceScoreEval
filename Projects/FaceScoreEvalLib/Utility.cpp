#include "FaceP.h"
#include "Utility.h"


using namespace cv;
using namespace std;
using namespace Face;

bool detectFaceByStasm(const Mat & inputImage, Rect * pArea)
{
	Mat des;
	cvtColor(inputImage, des, CV_RGB2GRAY);
	const char* datadir = "./data";
	static int isInit = 0;
	if (!isInit)
	{
		isInit = stasm_init(datadir, 0 /*trace*/);
	}
	stasm::DetPar detpar;
	static stasm::FaceDet facedet_g;
	facedet_g.DetectFaces_(des, "", true, 10, NULL);
	detpar = facedet_g.NextFace_();  // get next face's detpar from the face det

	if (stasm::Valid(detpar.x))
	{
		if (pArea != nullptr)
		{
			pArea->width = detpar.width;
			pArea->height = detpar.height;
			pArea->x = detpar.x - pArea->width / 2;
			pArea->y = detpar.y - pArea->height / 2;
		}
		
		return true;
	}
	else
	{
		return false;
	}
}


// **********************************************
// 功能: 使用STASM获取特征点
// 参数:   
//       [In]stasmImage 带有脸部的位图(灰度图) 
//       [Out]stasmPoint 用于获取Stasm的所有特征点
// 返回值: 是否侦测到脸部
bool GetStasmPoint(const cv::Mat1b& stasmImage, std::vector<cv::Point2f>& stasmPoint)
{
	int foundface = 0;

	if (!stasmImage.data)
	{
		perror("Cannot load (stasm)\n");
		exit(1);
	}

	// 重新设置容量为stasm特征点数目
	stasmPoint.resize(stasm_NLANDMARKS);
	// 搜索照片中的单个人脸（有多个也将寻找其中一个），并输出stasm路标点
	if (!stasm_search_single(&foundface, 
		reinterpret_cast<float*>(stasmPoint.data()),
		reinterpret_cast<const char*>(stasmImage.data),
		stasmImage.cols, stasmImage.rows, "0.jpg", "./data"))
	{
		printf("Error in stasm_search_single: %s\n", stasm_lasterr());
		exit(1);
	}
	// 没找到人脸则直接返回false
	if (!foundface) {
		perror("No face found in (stasm)\n");
		return false;
	}
	else {
		// 强制将路标点放入位图区域
		stasm_force_points_into_image(reinterpret_cast<float*>(stasmPoint.data()),
			stasmImage.cols, stasmImage.rows);
		return true;
	}
}


// **********************************************
// 功能: 使用DLIB获取特征点
// 参数:   
//       [In]dlibImage 带有脸部的位图 
//       [Out]dlibPoint 用于获取Dlib的所有特征点
// 返回值: 是否侦测到脸部
bool GetDlibPoint(const cv::Mat& dlibImage, std::vector<cv::Point2f>& dlibPoint)
{
	if (!dlibImage.data)
	{
		printf("Cannot load (dlib)\n");
		return false;
	}

	//image_window win;  
	// 读取脸部识别器 和 姿态估计模型
	dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

	//加载人脸特征点模型
	static bool isLoad = true;
	static dlib::shape_predictor pose_model;
	if (isLoad)
	{
		dlib::deserialize("./data/shape_predictor_68_face_landmarks.dat") >> pose_model;
		isLoad = false;
	}


	dlib::cv_image<dlib::bgr_pixel> cimg(dlibImage);
	// 侦测脸部   
	std::vector<dlib::rectangle> faces = detector(cimg);
	// 若没找到，直接返回
	if (faces.empty())
		return false;
	// 取出第一个脸部姿态，并输出保存所有特征点
	dlib::full_object_detection fod = pose_model(cimg, faces[0]);
	dlibPoint.clear();
	int num_parts = fod.num_parts();
	for (int i = 0; i < num_parts; ++i)
		dlibPoint.push_back(Point2f(fod.part(i).x(), fod.part(i).y()));
	
	return true;
}


//void drawPointNumber(const char* winName, const char* path)
//{
//	FaceImage faceImage;
//	faceImage.OpenRawFaceImage(path);
//	
//	for (int i = 0; i < FeaturePointSet::combineLandMarks; i++)
//	{
//		
//		circle(faceImage.faceMat, faceImage.pointSet[i], 3, cv::Scalar(0, 0, 255), -1);
//		//circle(LinkImage, cvPoint(pointSet[i].x, pointSet[i].y), 1, cv::Scalar(0, 0, 255), -1);
//		putText(faceImage.faceMat, to_string(i), faceImage.pointSet[i],
//			CV_FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 255), 1);
//		//if (i != FeaturePointSet::combineLandMarks - 1)
//		//	line(LinkImage, cvPoint(pointSet[i].x, pointSet[i].y),
//		//		cvPoint(pointSet[i + 1].x, pointSet[i + 1].y), Scalar(0, 0, 255), 2, 0);
//	}
//	
//	
//
//	//Display it all on the screen  
//	cvNamedWindow(winName, cv::WINDOW_AUTOSIZE);
//	imshow(winName, faceImage.faceMat);
//}
//
//void drawPoint(const char* winName, const char * path)
//{
//	FaceImage faceImage;
//	faceImage.OpenRawFaceImage(path);
//	cvNamedWindow(winName, cv::WINDOW_AUTOSIZE);
//	cv::Mat showMat;
//	faceImage.faceMat.copyTo(showMat);
//	for (int i = 0; i < faceImage.pointSet.combineLandMarks; ++i)
//		cv::circle(showMat, faceImage.pointSet[i], 1, cv::Scalar(0.0, 0.0, 0.0));
//	cv::imshow(winName, showMat);
//}

/**
*	封装获取特征点的繁琐过程。
*/
bool ExtractFeaturePoint(const char* Path, FeaturePointSet* PointSet)
{
	// 先确保清空
	PointSet->GetData().clear();
	// 根据 Path 路径读取图片，一份为原图，一份为灰度图。
	cv::Mat originalImage(cv::imread(Path, CV_LOAD_IMAGE_COLOR));
	cv::Mat grayImage = originalImage;
	cv::cvtColor(originalImage, grayImage, CV_BGR2GRAY);

	// 根据灰度图获取 Stasm 与 Dlib 的特征点。
	std::vector<cv::Point2f> stasmPointSet;
	std::vector<cv::Point2f> dlibPointSet;
	if (!GetStasmPoint(grayImage, stasmPointSet) || !GetDlibPoint(originalImage, dlibPointSet))
	{
		return false;
	}

	// 把两份特征点合并归一化
	FeaturePointSet pointSet;

	PointSet->Merge(dlibPointSet, stasmPointSet);

	return true;

}


/**
*	获取 2 维空间中众多离散点的多项式拟合曲线。根据最小二乘法多项式曲线拟合原理获取。
*	@ PointSet - 离散点集合。
*	@ Order - 多项式中的最高阶数。
*	@ return - 多项式系数 a 集合。（y = a0 + a1*x + a2*x^2 + ...）
*   注意：Order的值范围为[0, numPoints - 1]
*/
std::vector<float> GetMultinomialCoefficient(const std::vector<cv::Point2f> & PointSet, int Order)
{
	// 对于离散点集合（x1, y1）,（x2, y2）... ，拟合的 k 次多项式曲线为：
	//	y = a0 + a1*x + a2*x^2 + . . . + ak*x^k
	//	根据最小二乘法，各变量的关系如下：
	//	
	// [ 1  x1  ... x1^k ]  [ a0 ]     [ y1 ]                                    
	// | 1  x2  ... x2^k |  | a1 |  =  | y2 |                     
	// | :  :   ...   :  |  | :  |     | :  |                  
	// [ 1  xn  ... xn^k ]  [ ak ]     [ yn ]   

	int pointCount = PointSet.size();
	cv::Mat X(pointCount, Order + 1, CV_32FC1);
	cv::Mat A(Order + 1, 1, CV_32FC1);
	cv::Mat Y(pointCount, 1, CV_32FC1);

	// 把已知离散点储存在X、Y矩阵中
	for (int i = 0; i < pointCount; ++i)
	{
		for (int j = 0; j <= Order; ++j)
		{
			X.at<float>(i, j) = pow(PointSet[i].x, j);
		}
	}
	for (int i = 0; i < pointCount; ++i)
	{
		Y.at<float>(i, 0) = PointSet[i].y;
	}


	// 线性方程组 X * a = Y ，根据 X，Y 矩阵获取 a 的最小二乘解。
	cv::solve(X, Y, A, DECOMP_SVD);

	// 返回系数
	std::vector<float> coefficient;
	for (int i = 0; i < Order + 1; ++i)
	{
		coefficient.push_back(A.at<float>(i, 0));
	}

	return coefficient;
}


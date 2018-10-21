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
// ����: ʹ��STASM��ȡ������
// ����:   
//       [In]stasmImage ����������λͼ(�Ҷ�ͼ) 
//       [Out]stasmPoint ���ڻ�ȡStasm������������
// ����ֵ: �Ƿ���⵽����
bool GetStasmPoint(const cv::Mat1b& stasmImage, std::vector<cv::Point2f>& stasmPoint)
{
	int foundface = 0;

	if (!stasmImage.data)
	{
		perror("Cannot load (stasm)\n");
		exit(1);
	}

	// ������������Ϊstasm��������Ŀ
	stasmPoint.resize(stasm_NLANDMARKS);
	// ������Ƭ�еĵ����������ж��Ҳ��Ѱ������һ�����������stasm·���
	if (!stasm_search_single(&foundface, 
		reinterpret_cast<float*>(stasmPoint.data()),
		reinterpret_cast<const char*>(stasmImage.data),
		stasmImage.cols, stasmImage.rows, "0.jpg", "./data"))
	{
		printf("Error in stasm_search_single: %s\n", stasm_lasterr());
		exit(1);
	}
	// û�ҵ�������ֱ�ӷ���false
	if (!foundface) {
		perror("No face found in (stasm)\n");
		return false;
	}
	else {
		// ǿ�ƽ�·������λͼ����
		stasm_force_points_into_image(reinterpret_cast<float*>(stasmPoint.data()),
			stasmImage.cols, stasmImage.rows);
		return true;
	}
}


// **********************************************
// ����: ʹ��DLIB��ȡ������
// ����:   
//       [In]dlibImage ����������λͼ 
//       [Out]dlibPoint ���ڻ�ȡDlib������������
// ����ֵ: �Ƿ���⵽����
bool GetDlibPoint(const cv::Mat& dlibImage, std::vector<cv::Point2f>& dlibPoint)
{
	if (!dlibImage.data)
	{
		printf("Cannot load (dlib)\n");
		return false;
	}

	//image_window win;  
	// ��ȡ����ʶ���� �� ��̬����ģ��
	dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

	//��������������ģ��
	static bool isLoad = true;
	static dlib::shape_predictor pose_model;
	if (isLoad)
	{
		dlib::deserialize("./data/shape_predictor_68_face_landmarks.dat") >> pose_model;
		isLoad = false;
	}


	dlib::cv_image<dlib::bgr_pixel> cimg(dlibImage);
	// �������   
	std::vector<dlib::rectangle> faces = detector(cimg);
	// ��û�ҵ���ֱ�ӷ���
	if (faces.empty())
		return false;
	// ȡ����һ��������̬���������������������
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
*	��װ��ȡ������ķ������̡�
*/
bool ExtractFeaturePoint(const char* Path, FeaturePointSet* PointSet)
{
	// ��ȷ�����
	PointSet->GetData().clear();
	// ���� Path ·����ȡͼƬ��һ��Ϊԭͼ��һ��Ϊ�Ҷ�ͼ��
	cv::Mat originalImage(cv::imread(Path, CV_LOAD_IMAGE_COLOR));
	cv::Mat grayImage = originalImage;
	cv::cvtColor(originalImage, grayImage, CV_BGR2GRAY);

	// ���ݻҶ�ͼ��ȡ Stasm �� Dlib �������㡣
	std::vector<cv::Point2f> stasmPointSet;
	std::vector<cv::Point2f> dlibPointSet;
	if (!GetStasmPoint(grayImage, stasmPointSet) || !GetDlibPoint(originalImage, dlibPointSet))
	{
		return false;
	}

	// ������������ϲ���һ��
	FeaturePointSet pointSet;

	PointSet->Merge(dlibPointSet, stasmPointSet);

	return true;

}


/**
*	��ȡ 2 ά�ռ����ڶ���ɢ��Ķ���ʽ������ߡ�������С���˷�����ʽ�������ԭ���ȡ��
*	@ PointSet - ��ɢ�㼯�ϡ�
*	@ Order - ����ʽ�е���߽�����
*	@ return - ����ʽϵ�� a ���ϡ���y = a0 + a1*x + a2*x^2 + ...��
*   ע�⣺Order��ֵ��ΧΪ[0, numPoints - 1]
*/
std::vector<float> GetMultinomialCoefficient(const std::vector<cv::Point2f> & PointSet, int Order)
{
	// ������ɢ�㼯�ϣ�x1, y1��,��x2, y2��... ����ϵ� k �ζ���ʽ����Ϊ��
	//	y = a0 + a1*x + a2*x^2 + . . . + ak*x^k
	//	������С���˷����������Ĺ�ϵ���£�
	//	
	// [ 1  x1  ... x1^k ]  [ a0 ]     [ y1 ]                                    
	// | 1  x2  ... x2^k |  | a1 |  =  | y2 |                     
	// | :  :   ...   :  |  | :  |     | :  |                  
	// [ 1  xn  ... xn^k ]  [ ak ]     [ yn ]   

	int pointCount = PointSet.size();
	cv::Mat X(pointCount, Order + 1, CV_32FC1);
	cv::Mat A(Order + 1, 1, CV_32FC1);
	cv::Mat Y(pointCount, 1, CV_32FC1);

	// ����֪��ɢ�㴢����X��Y������
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


	// ���Է����� X * a = Y ������ X��Y �����ȡ a ����С���˽⡣
	cv::solve(X, Y, A, DECOMP_SVD);

	// ����ϵ��
	std::vector<float> coefficient;
	for (int i = 0; i < Order + 1; ++i)
	{
		coefficient.push_back(A.at<float>(i, 0));
	}

	return coefficient;
}


#include "FaceP.h"
#include "NoseDetection.h"


noseDetection::noseDetection(const std::vector<cv::Point2f>& dlibPoints, const std::vector<cv::Point2f>& stasmPoints,const cv::Mat& _image)
	: image(_image)
{
	try
	{
		if (image.channels() != 1)
		{
			throw "image 不是灰度图！";
		}
	}
	catch (char* err)
	{
		printf(err);
		exit(1);
	}

	int dataIndex = 0;

	//dlib鼻梁4个点
	for (int i = 27; i < 31; ++i) {
		noseData[dataIndex++] = dlibPoints[i];
	}//stasm左鼻翼2个点

	noseData[dataIndex++] = stasmPoints[50];
	noseData[dataIndex++] = stasmPoints[58];

	//stasm右鼻翼2个点
	noseData[dataIndex++] = stasmPoints[48];
	noseData[dataIndex++] = stasmPoints[54];

	Detect();
}

//检测鼻梁高度：通过在鼻梁两侧扫描灰度差值把这个差值值归一化视为鼻梁的高度。
void noseDetection::Detect()
{
	//左鼻翼阴影部分的矩形区域
	cv::Rect low_left_Rect(noseData[4].x - (noseData[4].x - noseData[5].x) / 2,
		noseData[0].y,
		(noseData[4].x - noseData[5].x) / 2 + (noseData[0].x - noseData[4].x) / 2,
		noseData[2].y - noseData[0].y);

	//右鼻翼阴影部分的矩形区域
	cv::Rect low_right_Rect(noseData[6].x - (noseData[6].x - noseData[0].x) / 2,
		noseData[0].y,
		(noseData[6].x - noseData[0].x) / 2 + (noseData[7].x - noseData[6].x) / 2,
		noseData[2].y - noseData[0].y);

	//鼻翼高光部分的矩形区域
	cv::Rect high_Rect(noseData[0].x - (noseData[0].x - noseData[4].x) / 2,
		noseData[0].y,
		(noseData[0].x - noseData[4].x) / 2 + (noseData[6].x - noseData[0].x) / 2,
		noseData[2].y - noseData[0].y);

//#ifdef _DEBUG
//	cv::cvtColor(image, image, CV_GRAY2BGR);
//	cv::rectangle(image, low_left_Rect, cv::Scalar(0, 0, 255));
//	std::cout << low_left_Rect.x << "," << low_left_Rect.y << std::endl;
//	cv::rectangle(image, low_right_Rect, cv::Scalar(0, 255, 0));
//	std::cout << low_right_Rect.x << "," << low_right_Rect.y << std::endl;
//	cv::rectangle(image, high_Rect, cv::Scalar(255, 0, 0));
//	std::cout << high_Rect.x << "," << high_Rect.y << std::endl;
//	cvNamedWindow("nose", 0);
//	imshow("nose", image);
//	cv::waitKey(0);
//#endif // _DEBUG
	int left = 0,//左鼻梁的灰度值之和
		mid = 0,	//中鼻梁的灰度值之和
		right = 0,//右鼻梁的灰度值之和
		sum = 1,//各区域的像素点数量
		leftDIF = 0,
		rightDIF = 0;
	//求左鼻梁的阴影灰度值
	for (int i = low_left_Rect.y; i < low_left_Rect.y + low_left_Rect.height; ++i)
	{
		for (int j = low_left_Rect.x; j < low_left_Rect.x + low_left_Rect.width; ++j)
		{
			left += image.at<uchar>(i, j);
			++sum;
		}
	}
	left /= sum;//归一化

	//求鼻梁的灰度值
	sum = 1;
	for (int i = high_Rect.y; i < high_Rect.y + high_Rect.height; ++i)
	{
		for (int j = high_Rect.x; j < high_Rect.x + high_Rect.width; ++j)
		{
			mid += image.at<uchar>(i, j);
			++sum;
		}
	}
	mid /= sum;//归一化

	//求右鼻梁的阴影灰度值
	sum = 1;
	for (int i = low_right_Rect.y; i < low_right_Rect.y + low_right_Rect.height; ++i)
	{
		for (int j = low_right_Rect.x; j < low_right_Rect.x + low_right_Rect.width; ++j)
		{
			right += image.at<uchar>(i, j);
			++sum;
		}
	}
	right /= sum;//归一化

	leftDIF = mid - left;
	rightDIF = mid - right;
	this->DIF = leftDIF > rightDIF ? leftDIF : rightDIF;//取鼻梁高度最大值
	this->DIF /= 255;
}
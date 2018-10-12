#include "FaceP.h"
#include "FaceColor.h"

Face::FaceColorCaculator::FaceColorCaculator()
{
}

Face::FaceColorCaculator::~FaceColorCaculator()
{
}

int Face::FaceColorCaculator::Caculate(FaceImage & fImg)
{
	std::vector<cv::Point2f> allPoints = fImg.pointSet.GetData();
	std::vector<cv::Point2f> facePoints;
	//收集脸的外部轮廓点
	for (int poi = 0; poi < 17; poi++)
	{
		facePoints.push_back(allPoints[poi]);
	}
	for (int poi = 25; poi < 32; poi++)
	{
		facePoints.push_back(allPoints[poi]);
	}
	int yLength = fImg.faceMat.rows, xLength = fImg.faceMat.cols;
	int num_Points = 0;
	int faceColor = 0;
	for (int x = 0; x < xLength; x++)
	{
		for (int y = 0; y < yLength; y++)
		{
			cv::Point2f point((float)x, (float)y);
			if (PointInPolygon(point, facePoints))
			{
				num_Points++;
				cv::Vec3b bgr = fImg.faceMat.at<cv::Vec3b>(point);
				faceColor += (bgr[2] * 299 + bgr[1] * 587 + bgr[0] * 114 + 500) / 1000;
			}
		}
	}
	int result = faceColor / num_Points;
	if (result > 127)
	{
		result = result * 3 / 5;
	}
	return result;
}

bool Face::FaceColorCaculator::PointInPolygon(cv::Point2f point, std::vector<cv::Point2f>& polygonPoi)
{
	bool is_In = false;
	int i, j,size = (int)polygonPoi.size();
	for (i = 0, j = size - 1; i < size; j = i++)
	{
		if (((polygonPoi[i].y > point.y) != (polygonPoi[j].y > point.y)) &&
			(point.x < (polygonPoi[j].x - polygonPoi[i].x) * (point.y - polygonPoi[i].y) / (polygonPoi[j].y - polygonPoi[i].y) + polygonPoi[i].x))
		{
			is_In = !is_In;
		}
	}
	return is_In;
}

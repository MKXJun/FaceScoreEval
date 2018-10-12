#include "FaceP.h"
#include "FaceTypeFit.h"

#include <math.h>

using namespace std;


namespace Face
{


	/**
	 *	根据传入的特征点分析脸型下巴类型。
	 *	@ PointSet - 特征点集合。
	 *	@ return - 下巴类型。
	 */
	JawType FaceTypeFit::AnalyzeJawType(const FeaturePointSet & PointSet)
	{
		// 脸型特征点索引。这些本来应该从 FeaturePointSet 中获取代码信息，但它并没有提供，现在直接标记。
		int startLabel = 4;
		int endLabel = 12;
		return JawType();
	}

	/*
	*des：	利用最小二乘法找出偏差最小的幂函数用于 初步 拟合给定的脸型shape
	*		找到和给定特征点（特征点startLabel ~ 特征点endLabel）最拟合的幂函数的参数a 和参数k
	*para:	flag 是用于判断是否要进行2阶或以上的幂函数拟合，默认为0（不需要）
	*		startLabel、endLabel 描述脸型特征点的起始下标以及结束的下标
	*/
	void FaceTypeFit::fitting(bool flag, int startLabel, int endLabel)
	{
		

#ifdef DEBUG
		LARGE_INTEGER startTime, endTime, frequency;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&startTime);
#endif // DEBUG

		int maxNum = endLabel - startLabel;
		float totalX = 0.0f, totalY = 0.0f;//最小二乘法的自变量x 以及因变量y 求和
		float a = 0.0f;//临时系数a
		float deviation = 0, lastDeviation;//临时误差

		//先算二阶
		for (int i = startLabel; i <= maxNum; ++i)
		{
			totalX += pow(nor[i].x, 2);
			totalY += nor[i].y;
		}

		a = totalY / totalX;
		k = 2;

		for (int i = startLabel; i <= maxNum; ++i)
		{
			deviation += pow(a * pow(nor[i].x, 2) - nor[i].y, 2);
		}
		deviation = deviation;

		if (flag)//算4到无穷阶
		{
			for (int k = 4; ; k += 2)
			{
				totalX = 0;
				totalY = 0;
				lastDeviation = deviation;
				for (int i = startLabel; i <= maxNum; ++i)
				{
					totalX += pow(nor[i].x, k);
					totalY += nor[i].y;
				}
				a = totalY / totalX;
				for (int i = startLabel; i <= maxNum; ++i)
				{
					deviation += pow(a * pow(nor[i].x, k) - nor[i].y, 2);
				}


				if (lastDeviation < deviation)//找出最小值
				{
					break;
				}
				else
				{
					a = a;
					k = k;
					deviation = deviation;
				}
			}
		}

#ifdef DEBUG
		QueryPerformanceCounter(&endTime);
		//cout << "fittingTime1:" << startTime.QuadPart << endl;
		//cout << "fittingTime2:" << endTime.QuadPart << endl;
		cout << "fittingTime:" << (endTime.QuadPart - startTime.QuadPart) * 1000 / frequency.QuadPart << endl;
#endif // DEBUG
	}

	/*
	*des：	根据得到的初步拟合曲线，判断具体的脸型
	*para:
	*/

	FaceTypeFit::FaceType FaceTypeFit::distinguish()
	{
		FeaturePointSet PointSet = nor;

		//#define DEBUG
		for (int i = 0; i <= 4; ++i)//先测左边脸颊的斜率变化
		{
#ifndef DEBUG
			if (atan2(nor[i + 2].y - nor[i + 1].y,	//后一组点的斜率的角度
				nor[i + 2].x - nor[i + 1].x) -	//减去
				atan2(nor[i + 1].y - nor[i].y,		//前一组点的斜率的角度
					nor[i + 1].x - nor[i].x)
				>= 0.28f)//角度过渡过大
			{
				faceType = SQUARE_FACE;
				fitting(0, 4, 12);
				return SQUARE_FACE;
			}
#endif	
#ifdef DEBUG
			cout << "国字脸斜率:" << i << ":" << atan2(nor[i + 2].y - nor[i + 1].y,	//后一组点的斜率的角度
				nor[i + 2].x - nor[i + 1].x) -	//减去
				atan2(nor[i + 1].y - nor[i].y,		//前一组点的斜率的角度
					nor[i + 1].x - nor[i].x) << endl;
#endif // DEBUG
		}

		for (int i = 10; i <= 14; ++i)//再测又边脸颊的斜率变化
		{
#ifndef DEBUG
			if (atan2(nor[i + 2].y - nor[i + 1].y,	//后一组点的斜率的角度
				nor[i + 2].x - nor[i + 1].x) -	//减去
				atan2(nor[i + 1].y - nor[i].y,		//前一组点的斜率的角度
					nor[i + 1].x - nor[i].x)
				>= 0.28)//角度过渡过大
			{
				faceType = SQUARE_FACE;
				fitting(0, 4, 12);
				return SQUARE_FACE;
			}
#endif		
#ifdef DEBUG
			cout << "国字脸斜率:" << i << ":" << atan2(nor.data[i + 2].getY() - nor.data[i + 1].getY(),	//后一组点的斜率的角度
				nor.data[i + 2].getX() - nor.data[i + 1].getX()) -	//减去
				atan2(nor.data[i + 1].getY() - nor.data[i].getY(),		//前一组点的斜率的角度
					nor.data[i + 1].getX() - nor.data[i].getX()) << endl;
#endif // DEBUG
		}

		faceType = OVAL_FACE;
		fitting(0, 0, 16);
		return OVAL_FACE;
	}


}	// namespace Face
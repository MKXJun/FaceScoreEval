#include "FaceP.h"
#include "FaceTypeFit.h"

#include <math.h>

using namespace std;


namespace Face
{


	/**
	 *	���ݴ������������������°����͡�
	 *	@ PointSet - �����㼯�ϡ�
	 *	@ return - �°����͡�
	 */
	JawType FaceTypeFit::AnalyzeJawType(const FeaturePointSet & PointSet)
	{
		// ������������������Щ����Ӧ�ô� FeaturePointSet �л�ȡ������Ϣ��������û���ṩ������ֱ�ӱ�ǡ�
		int startLabel = 4;
		int endLabel = 12;
		return JawType();
	}

	/*
	*des��	������С���˷��ҳ�ƫ����С���ݺ������� ���� ��ϸ���������shape
	*		�ҵ��͸��������㣨������startLabel ~ ������endLabel������ϵ��ݺ����Ĳ���a �Ͳ���k
	*para:	flag �������ж��Ƿ�Ҫ����2�׻����ϵ��ݺ�����ϣ�Ĭ��Ϊ0������Ҫ��
	*		startLabel��endLabel �����������������ʼ�±��Լ��������±�
	*/
	void FaceTypeFit::fitting(bool flag, int startLabel, int endLabel)
	{
		

#ifdef DEBUG
		LARGE_INTEGER startTime, endTime, frequency;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&startTime);
#endif // DEBUG

		int maxNum = endLabel - startLabel;
		float totalX = 0.0f, totalY = 0.0f;//��С���˷����Ա���x �Լ������y ���
		float a = 0.0f;//��ʱϵ��a
		float deviation = 0, lastDeviation;//��ʱ���

		//�������
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

		if (flag)//��4�������
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


				if (lastDeviation < deviation)//�ҳ���Сֵ
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
	*des��	���ݵõ��ĳ���������ߣ��жϾ��������
	*para:
	*/

	FaceTypeFit::FaceType FaceTypeFit::distinguish()
	{
		FeaturePointSet PointSet = nor;

		//#define DEBUG
		for (int i = 0; i <= 4; ++i)//�Ȳ�������յ�б�ʱ仯
		{
#ifndef DEBUG
			if (atan2(nor[i + 2].y - nor[i + 1].y,	//��һ����б�ʵĽǶ�
				nor[i + 2].x - nor[i + 1].x) -	//��ȥ
				atan2(nor[i + 1].y - nor[i].y,		//ǰһ����б�ʵĽǶ�
					nor[i + 1].x - nor[i].x)
				>= 0.28f)//�Ƕȹ��ɹ���
			{
				faceType = SQUARE_FACE;
				fitting(0, 4, 12);
				return SQUARE_FACE;
			}
#endif	
#ifdef DEBUG
			cout << "������б��:" << i << ":" << atan2(nor[i + 2].y - nor[i + 1].y,	//��һ����б�ʵĽǶ�
				nor[i + 2].x - nor[i + 1].x) -	//��ȥ
				atan2(nor[i + 1].y - nor[i].y,		//ǰһ����б�ʵĽǶ�
					nor[i + 1].x - nor[i].x) << endl;
#endif // DEBUG
		}

		for (int i = 10; i <= 14; ++i)//�ٲ��ֱ����յ�б�ʱ仯
		{
#ifndef DEBUG
			if (atan2(nor[i + 2].y - nor[i + 1].y,	//��һ����б�ʵĽǶ�
				nor[i + 2].x - nor[i + 1].x) -	//��ȥ
				atan2(nor[i + 1].y - nor[i].y,		//ǰһ����б�ʵĽǶ�
					nor[i + 1].x - nor[i].x)
				>= 0.28)//�Ƕȹ��ɹ���
			{
				faceType = SQUARE_FACE;
				fitting(0, 4, 12);
				return SQUARE_FACE;
			}
#endif		
#ifdef DEBUG
			cout << "������б��:" << i << ":" << atan2(nor.data[i + 2].getY() - nor.data[i + 1].getY(),	//��һ����б�ʵĽǶ�
				nor.data[i + 2].getX() - nor.data[i + 1].getX()) -	//��ȥ
				atan2(nor.data[i + 1].getY() - nor.data[i].getY(),		//ǰһ����б�ʵĽǶ�
					nor.data[i + 1].getX() - nor.data[i].getX()) << endl;
#endif // DEBUG
		}

		faceType = OVAL_FACE;
		fitting(0, 0, 16);
		return OVAL_FACE;
	}


}	// namespace Face
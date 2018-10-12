#include "FaceP.h"
#include "FaceShape.h"
#include "Utility.h"


namespace Face
{
	FaceShape::FaceShape(const FaceImage & faceImage)
		: faceImage(faceImage), shape()
	{
	}


	FaceShape::~FaceShape()
	{
	}

	// **********************************************
	// ����: ��ȡ����ȧ�ǿ��
	// ����ֵ: ȧ�ǿ��
	float FaceShape::GetCheekBoneWith() const
	{
		return faceImage.pointSet[15].x - faceImage.pointSet[1].x;
	}

	// **********************************************
	// ����: ��ȡ���������
	// ����ֵ: �����
	float FaceShape::GetJawWidth() const
	{
		return faceImage.pointSet[11].x - faceImage.pointSet[5].x;
	}

	// **********************************************
	// ����: ��ȡ�����߶�
	// ����ֵ: �����߶�
	int FaceShape::GetHeight() const
	{
		return faceImage.faceMat.rows;
	}

	// **********************************************
	// ����: ����°����߷��̵�ϵ��(ʹ������3-13��������)
	// ����: ��Ҫ����ߴ���(��������11����)
	// ����ֵ: 0���N����ĸ���vector
	std::vector<float> FaceShape::GetJawCurveCoefficient(int N)
	{
		return GetMultinomialCoefficient(
			std::vector<cv::Point2f>(
				faceImage.pointSet.GetData().begin() + 3,
				faceImage.pointSet.GetData().begin() + 14),
			4);
	}

	// **********************************************
	// ����: ��ȡ����
	// ����ֵ: ����ö��
	FACE_SHAPE FaceShape::GetFaceShape()
	{
		// ֮ǰ�Ѿ�����Ļ��Ͳ���Ҫ������
		if (shape != FACE_SHAPE_UNKNOWN)
		{
			return shape;
		}

		auto vec = GetJawCurveCoefficient(4);
		// ��һ����ϵ����һ��
		float factor = vec[1];
		for (float& f : vec)
			f /= factor;
		
		// �°������ж�
		int jawType = 0;
		if (vec[2] < -0.0041f)
			jawType = 0;	// ���°�
		else if (vec[2] > -0.0015f && vec[3] < 0.0f)
			jawType = 1;	// ���°�
		else
			jawType = 2;	// Բ�°�

		// ȷ���߶����ȵı�ֵ
		float rate = (float)faceImage.faceMat.rows / (float)faceImage.faceWidth;
		int shapeType = 0;
		if (rate > 1.3f)
			shapeType = 0;		// ��>>>��
		else if (rate < 1.2f)
			shapeType = 1;		// ��>��
		else
			shapeType = 2;		// ��>>��
		
		// ��Ϊ�°ͣ���Ϊ��߱�
		FACE_SHAPE finalShape[3][3] = {
			{ FACE_SHAPE_OBLONG, FACE_SHAPE_SQUARE, FACE_SHAPE_OBLONG },
			{ FACE_SHAPE_TRIANGLE, FACE_SHAPE_TRIANGLE, FACE_SHAPE_TRIANGLE },
			{ FACE_SHAPE_OVAL, FACE_SHAPE_ROUND, FACE_SHAPE_OVAL },
			
		};

		return shape = finalShape[jawType][shapeType];
	}

}	// nameSpace Face
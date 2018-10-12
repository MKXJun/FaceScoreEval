#include "FaceP.h"
#include "FaceScore.h"



namespace Face
{

// ÆÀ·Ö±ê×¼
const FaceScore FaceScore::Standard = {
	0.0504f, 0.031f, 0.0413f, 0.0482f,
	0.174f, 0.0493f, 0.154f,
	0.085f, 0.0975f, 0.094f, 0.135f,
	0.0413f,
	0.0f, 0.0f
};



float CountFaceScore(const FeaturePointSet & nor)
{
	FaceProportion data(nor);
	FaceScore faceScore = CountProportionScore(data);
	float score = 0.0f;
	for (int i = 0; i < 12; ++i)
		score += reinterpret_cast<float*>(&faceScore)[i] * reinterpret_cast<const float*>(&FaceScore::Standard)[i];
	//printf("\nscore:%f", score);
	return score;
}


FaceScore CountProportionScore(const FaceProportion & data)
{
	FaceScore res;
	res.s_eEh = 100 - abs(data.eEh - 1.14f) * 150;
	res.s_fEg = 100 - abs(data.fEg - 1.71f) * 100;
	res.s_gEi = 100 - abs(data.gEi - 2.03f) * 50;
	res.s_hEi = 100 - abs(data.hEi - 2.37f) * 50;
	res.s_kEe = 100 - abs(data.kEe - 1.12f) * 250;
	if (data.lEb > 1.30f) {
		if (data.lEb > 1.35f)
			res.s_lEb = 100 - abs(data.lEb - 1.30f) * 500;
		else
			res.s_lEb = 100 - abs(data.lEb - 1.30f) * 250;
	}
	else
	{
		res.s_lEb = 100 - abs(data.lEb - 1.30f) * 30;
	}
	//data.s_nEl = 100 - abs(data.nEl - )
	if (data.nEl > 0.28f) {
		res.s_nEl = 100 - abs(data.nEl - 0.28f) * 1000;
	}
	else
		res.s_nEl = 100 - abs(data.nEl - 0.28f) * 10;

	res.s_mEl = 100 - abs(data.mEl - 0.48f) * 500;

	res.s_qEo = 100 - abs(data.qEo - 0.45f) * 500;
	if (data.rEp < 0.43f) {
		res.s_rEp = 100 - abs(data.rEp - 0.46f) * 1000;
	}
	else {
		if (data.rEp < 0.46f) {
			res.s_rEp = 100 - abs(data.rEp - 0.50f) * 100;
		}
		else if (data.rEp < 0.50f) {
			res.s_rEp = 100 - abs(data.rEp - 0.50f) * 50;
		}
		else
		{
			res.s_rEp = 100 - abs(data.rEp - 0.50f) * 10;
		}
	}
	if (data.nEh > 0.78f) {
		res.s_nEh = 100 - abs(data.nEh - 0.78f) * 800;
	}
	else {
		res.s_nEh = 100 - abs(data.nEh - 0.78f) * 50;
	}
	res.s_reyeBrow = 100 - abs(data.rjEu) * 100 -
		abs(data.rs - 7.5f) * 2;
	//printf("asd:%f, w:%f\n", 100 - data.rjEu * 100, data.s_reyeBrow);
	if (data.rvwEr > 0.66f)
		res.s_reyeBrow -= (data.rvwEr - 0.5f) * 200;
	else if (data.rvwEr > 0.6f)
		res.s_reyeBrow -= (data.rvwEr - 0.5f) * 100;
	else if (data.rvwEr > 0.5f)
		res.s_reyeBrow -= (data.rvwEr - 0.5f) * 50;
	else
		res.s_reyeBrow -= abs(data.rvwEr - 0.5f) * 10;

	res.s_leyeBrow = 100 - abs(data.ljEu) * 100 -
		abs(data.ls - 7.5f) * 3;
	if (data.lvwEr > 0.66f)
		res.s_leyeBrow = (data.lvwEr - 0.5f) * 200;
	else if (data.lvwEr > 0.6f)
		res.s_leyeBrow -= (data.lvwEr - 0.5f) * 100;
	else if (data.lvwEr > 0.5f)
		res.s_leyeBrow -= (data.lvwEr - 0.5f) * 50;
	else
		res.s_leyeBrow -= abs(data.lvwEr - 0.5f) * 10;
	res.s_eyeBrow = (res.s_leyeBrow + res.s_reyeBrow) / 2;
	return res;
}






}	// namespace Face


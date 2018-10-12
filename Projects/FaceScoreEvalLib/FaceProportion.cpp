#include "FaceP.h"
#include "FaceProportion.h"


namespace Face
{

	
FaceProportion::FaceProportion()
{
	memset(this, 0, sizeof(FaceProportion));
}

FaceProportion::FaceProportion(const FeaturePointSet & pointSet)
{
	if (pointSet.GetData().empty() != true)
		Count(pointSet);
	else
		memset(this, 0, sizeof(FeaturePointSet));
}

void FaceProportion::Count(const FeaturePointSet & pointSet)
{
	float eyebrowY = 0.0f;
	for (int i = 22; i <= 29; ++i)
		eyebrowY += pointSet[i].y / 8.0f;

	//算出各比例，注意XY是图片坐标系
	float b = pointSet[8].y - (pointSet[42].y + pointSet[43].y) / 2.0f;
	float e = pointSet[8].y - pointSet[60].y;
	float f = pointSet[77].y - (pointSet[42].y + pointSet[43].y) / 2.0f;
	float g = pointSet[8].y - pointSet[81].y;
	float h = pointSet[60].y - (pointSet[42].y + pointSet[43].y) / 2.0f;
	float i = pointSet[77].y - pointSet[60].y;
	float k = pointSet[60].y - eyebrowY;
	float l = (pointSet[14].x + pointSet[16].x - pointSet[0].x - pointSet[2].x) / 2;
	float m = pointSet[43].x - pointSet[42].x;
	float n = pointSet[58].x - pointSet[62].x;
	float o = pointSet[48].x - pointSet[38].x;
	float p = (pointSet[34].x + pointSet[48].x - pointSet[44].x - pointSet[38].x) / 2;
	float q = pointSet[44].x - pointSet[34].x;
	float r = (pointSet[40].y + pointSet[50].y - pointSet[36].y - pointSet[46].y) / 2;
	float ru = pointSet[29].x - pointSet[26].x;
	float rs = atan2((pointSet[30].y - pointSet[26].y), (pointSet[30].x - pointSet[26].x));
	rs /= (float)(CV_PI / 180);
	float rj = pointSet[44].x - pointSet[26].x;
	float rv = pow(pow(pointSet[27].x - pointSet[31].x, 2) +
		pow(pointSet[27].y - pointSet[31].y, 2), 0.5f);
	float rw = pow(pow(pointSet[28].x - pointSet[30].x, 2) +
		pow(pointSet[28].y - pointSet[30].y, 2), 0.5f);
	float lu = pointSet[25].x - pointSet[22].x;
	float ls = atan2((pointSet[23].y - pointSet[25].y), (pointSet[25].x - pointSet[23].x));
	ls /= (float)(CV_PI / 180);
	float lj = pointSet[25].x - pointSet[34].x;
	float lv = pow(pow(pointSet[20].x - pointSet[24].x, 2) +
		pow(pointSet[20].y - pointSet[24].y, 2), 0.5f);
	float lw = pow(pow(pointSet[21].x - pointSet[23].x, 2) +
		pow(pointSet[21].y - pointSet[23].y, 2), 0.5f);

	eEh = e / h;
	fEg = f / g;
	gEi = g / i;
	kEe = k / e;
	hEi = h / i;
	lEb = l / b;
	mEl = m / l;
	nEl = n / l;
	qEo = q / o;
	rEp = r / p;
	nEh = n / h;
	rjEu = rj / ru;
	rs = rs;
	rvwEr = (rv + rw) / 2 / r;
	ljEu = lj / lu;
	ls = ls;
	lvwEr = (lv + lw) / 2 / r;
}

}	// namespace Face
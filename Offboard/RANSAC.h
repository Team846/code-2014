#ifndef RANSAC_H_
#define RANSAC_H_

#include <OpenNI.h>

using namespace std;

class RANSAC
{
public:
	RANSAC(int iterations, float inlierDist);

	void MarkPointsInBestFitPlane(float **pointCloud, int numPts, bool *result);

private:
	int m_iterations;
	float m_inlierDist;
};

#endif

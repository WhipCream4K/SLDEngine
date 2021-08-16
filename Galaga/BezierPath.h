#pragma once

#include "BezierCurve.h"

class BezierPath
{
public:

	void AddCurve(const BezierCurve& curve, int samples);
	std::vector<rtm::float2f> Sample();

private:
	
	std::vector<BezierCurve> m_Curves;
	std::vector<int> m_Samples;
};


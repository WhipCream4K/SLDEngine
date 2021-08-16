#include "BezierPath.h"

void BezierPath::AddCurve(const BezierCurve& curve, int samples)
{
	m_Curves.emplace_back(curve);
	m_Samples.emplace_back(samples);
}

std::vector<rtm::float2f> BezierPath::Sample()
{
	std::vector<rtm::float2f> outPath{};
	
	for (size_t i = 0; i < m_Samples.size(); ++i)
	{
		for (int j = 0; j <= m_Samples[i]; ++j)
		{
			const float t = float(j) / float(m_Samples[i]);
			outPath.emplace_back(m_Curves[i].CalculateCurvePoint(t));
		}
	}

	return outPath;
}
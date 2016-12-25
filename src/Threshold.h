#pragma once
#include "IRenderMode.h"
class Threshold :
	public IRenderMode
{
public:
	Threshold();
	virtual ~Threshold();

	// Inherited via IRenderMode
	virtual void Compute(const cv::Mat & source, cv::Mat & destination) override;
};


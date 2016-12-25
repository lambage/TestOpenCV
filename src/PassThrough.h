#pragma once
#include "IRenderMode.h"
class PassThrough :
	public IRenderMode
{
public:
	PassThrough();
	virtual ~PassThrough();

	// Inherited via IRenderMode
	virtual void Compute(const cv::Mat& source, cv::Mat& destination) override;
};


#pragma once

#include "IRenderMode.h"

class Sobel :
	public IRenderMode
{
public:
	Sobel();
	virtual ~Sobel();


	// Inherited via IRenderMode
	virtual void Compute(const cv::Mat & source, cv::Mat & destination) override;

};


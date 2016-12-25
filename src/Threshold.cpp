#include "stdafx.h"
#include "Threshold.h"
#include "CinderOpenCV.h"


Threshold::Threshold()
{
}


Threshold::~Threshold()
{
}

void Threshold::Compute(const cv::Mat & source, cv::Mat & destination)
{
	cv::threshold(source, destination, 128, 255, CV_8U);
}

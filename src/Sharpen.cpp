#include "stdafx.h"
#include "Sharpen.h"


Sharpen::Sharpen()
{
}


Sharpen::~Sharpen()
{
}

void Sharpen::Compute(const cv::Mat & source, cv::Mat & destination)
{
	auto kern = cv::Mat{ (cv::Mat_<char>(3,3) << 0, -1,  0,
		-1,  5, -1,
		0, -1,  0) };
	cv::filter2D(source, destination, CV_8U, kern);
}

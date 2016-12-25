#include "stdafx.h"
#include "Sobel.h"
#include "CinderOpenCV.h"


Sobel::Sobel()
{
}


Sobel::~Sobel()
{
}

void Sobel::Compute(const cv::Mat & source, cv::Mat & destination)
{
	cv::Sobel(source, destination, CV_8U, 1, 0);
}

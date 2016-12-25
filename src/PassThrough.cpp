#include "stdafx.h"
#include "PassThrough.h"
#include "CinderOpenCV.h"


PassThrough::PassThrough()
{
}


PassThrough::~PassThrough()
{
}

void PassThrough::Compute(const cv::Mat& source, cv::Mat& destination)
{
	destination = source;
}

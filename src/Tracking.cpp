#include "stdafx.h"
#include "Tracking.h"
#include "CinderOpenCV.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

Tracking::Tracking()
{
}


Tracking::~Tracking()
{
}

void Tracking::Compute(const cv::Mat& source, cv::Mat& destination)
{
    auto cvt = cv::Mat{};
    cv::cvtColor(source, cvt, CV_BGR2HSV);

    auto lower = cv::Scalar(m_range-10, 50, 50);
    auto upper = cv::Scalar(m_range+10, 255, 255);

    auto mask = cv::Mat{};

    cv::inRange(cvt, lower, upper, mask);

    auto result = cv::Mat{};
    cv::bitwise_and(source, source, result, mask);

    destination = result;
}

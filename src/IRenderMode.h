#pragma once

namespace cv
{
	class Mat;
}

class IRenderMode
{
public:
	IRenderMode() {}
	virtual ~IRenderMode() {}

	virtual void Compute(const cv::Mat& source, cv::Mat& destination) = 0;
    virtual std::string Name() const = 0;
};


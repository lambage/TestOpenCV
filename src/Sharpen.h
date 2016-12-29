#pragma once
#include "IRenderMode.h"
class Sharpen :
	public IRenderMode
{
public:
	Sharpen();
	virtual ~Sharpen();

	// Inherited via IRenderMode
	virtual void Compute(const cv::Mat & source, cv::Mat & destination) override;
    virtual std::string Name() const override { return "Sharpen"; }
};


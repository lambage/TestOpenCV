#pragma once
#include "IRenderMode.h"
class Tracking :
    public IRenderMode
{
public:
    Tracking();
    virtual ~Tracking();

    // Inherited via IRenderMode
    virtual void Compute(const cv::Mat& source, cv::Mat& destination) override;

    void Increment() { m_range++; }
    void Decrement() { m_range--; }

    int GetRange() const { return m_range; }
private:
    int m_range = 180;
};


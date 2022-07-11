#pragma once

#include <opencv2/core.hpp>

class FaceMerging 
{
private:
    /// Benchmarking timer
    cv::TickMeter timeRecorder_;
public:
    FaceMerging() = default;
    ~FaceMerging() = default;
    void merge();
};
#pragma once

#include <opencv2/core.hpp>

class FaceAnnotator
{
private:
    /// Benchmarking timer
    cv::TickMeter timeRecorder_;
public:
    FaceAnnotator() = default;
    ~FaceAnnotator() = default;
    cv::Mat annotate(const cv::Mat& frame, const cv::Rect& detection);
};
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
    std::vector<cv::Rect2d> merge(std::vector<Detection> detected, 
                                  std::vector<TrackInfo> tracked);
};
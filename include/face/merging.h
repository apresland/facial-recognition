#pragma once

#include "face/types.h"
#include <opencv2/core.hpp>
#include <vector>

class FaceMerging 
{
private:
    /// Benchmarking timer
    cv::TickMeter timeRecorder_;
public:
    FaceMerging() = default;
    ~FaceMerging() = default;
    std::vector<DetectionsDescr> merge(std::vector<Detection> detected, 
                                       std::vector<TrackInfo> tracked);
};
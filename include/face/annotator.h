#pragma once

#include "face/types.h"
#include <opencv2/core.hpp>
#include <vector>

class FaceAnnotator
{
private:
    /// Benchmarking timer
    cv::TickMeter timeRecorder_;
public:
    FaceAnnotator() = default;
    ~FaceAnnotator() = default;
    cv::Mat annotate(const cv::Mat& input, std::vector<DetectionsDescr>& detections);
};

#pragma once

#include "face/types.h"
#include <vector>
#include <future>
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/tracking.hpp>

class FaceTracker
{
private:

    cv::Ptr<cv::Tracker> tracker_;
    cv::TickMeter timeRecorder_;

    int skip_frames_ = 0;
    bool track_face_ = false;

    uint32_t track_id_{0};
    float current_score_{0.f};

public:

    FaceTracker() = default;
    FaceTracker(uint32_t track_id) : track_id_(track_id) {};

    void init(const cv::Mat& frame, Detection& detection);
    TrackState track(const cv::Mat& frame, TrackInfo& track_info);
};
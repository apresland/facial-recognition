#pragma once

#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/tracking.hpp>

class FaceTracker
{
private:
    cv::Ptr<cv::Tracker> tracker_;
    cv::Rect2d face_;
    cv::TickMeter timeRecorder_;
public:
    explicit FaceTracker() = default;
    void init(const cv::Mat& frame, cv::Rect2d& roi);
    void track(const cv::Mat& frame, cv::Rect2d& roi);
    void deinit();
};
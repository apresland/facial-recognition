#pragma once

#include <vector>
#include <future>
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/tracking.hpp>

class FaceTracker
{
private:
    cv::Ptr<cv::Tracker> tracker_;
    cv::Rect2d face_;
    cv::TickMeter timeRecorder_;

    // If there is no detection in estimated area tracking will wait this number offrames
    // before considering the track lost and finishing it.
    const int max_skip_frames_ {10};
    int skip_frames_ = 0;
    bool track_face_ = false;

    std::future<std::vector<cv::Rect2d>> detections_future_;
    void init(const cv::Mat& frame, cv::Rect2d& roi);    
    std::vector<cv::Rect2d> track(const cv::Mat& frame, std::vector<cv::Rect2d> detections);
    bool is_tracking();


public:
    explicit FaceTracker() = default;

    void trackAsync(const cv::Mat& frame, std::vector<cv::Rect2d> detections);
    std::vector<cv::Rect2d> getAsync();
};
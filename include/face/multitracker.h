#pragma once

#include "face/context.h"
#include "face/types.h"
#include "face/tracker.h"
#include "face/track_observer.h"
#include <future>
#include <thread>
#include <vector>
#include <memory>
#include <unordered_map>

class Multitracker 
{
private:
    std::future<std::vector<TrackInfo>> detections_future_;
    std::unordered_map<uint32_t, std::unique_ptr<FaceTracker>> trackers_;
    std::unordered_map<uint32_t, cv::Rect2d> objects_;
    std::unordered_map<uint32_t, float> scores_;
    uint32_t current_track_id_{0};
    ITrackObserver& observer_;
public:
    explicit Multitracker::Multitracker(ITrackObserver& observer) : observer_(observer) {}
    void add(const cv::Mat& frame, Detection& detection);
    std::vector<TrackInfo> track(const cv::Mat& frame);
    void trackAsync(const cv::Mat& frame);
    std::vector<TrackInfo> getAsync();
private:
    double intersection_over_union(cv::Rect2d r1, cv::Rect2d r2);
};
#pragma once

#include "face/types.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

class ITrackObserver 
{
public:
    virtual void trackStart(const TrackId& trackId) = 0;
    virtual void trackEnd(const TrackId& trackId) = 0;
    virtual void bestShot(const cv::Mat& frame, const DetectionDescr& descr) = 0;
};

class TrackObserver : public ITrackObserver
{
public:
    explicit TrackObserver();
    ~TrackObserver() = default;

    void trackStart(const TrackId& trackId) override;
    void trackEnd(const TrackId& track_id) override;
    void bestShot(const cv::Mat& frame, const DetectionDescr& descr) override;
};



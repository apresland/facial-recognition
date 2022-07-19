#pragma once

#include "face/types.h"
#include <opencv2/core.hpp>

class ITrackObserver 
{
public:
    virtual void trackStart(const TrackId& trackId) = 0;
    virtual void trackEnd(const TrackId& trackId) = 0;
    virtual void bestShot(const cv::Mat& frame, const DetectionDescr& descr) = 0;
};



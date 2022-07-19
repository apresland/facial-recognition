#pragma once

#include <memory>
#include <opencv2/core.hpp>
#include "face/track_observer.h".h>
#include "face/recognizer.h"

class Recognition : public ITrackObserver
{
private:
    std::unique_ptr<Recognizer> recognizer_;
public:
    Recognition();
    ~Recognition();

    void trackStart(const TrackId& trackId) override;
    void trackEnd(const TrackId& track_id) override;
    void bestShot(const cv::Mat& frame, const DetectionDescr& descr) override;
};

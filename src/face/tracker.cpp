#include "face/tracker.h"
#include "face/context.h"

void FaceTracker::init(const cv::Mat& frame, Detection& detection) 
{
    tracker_
        = cv::TrackerKCF::create();

    tracker_
        ->init(frame, detection.rectangle);

    current_score_
        = detection
            .score;

    skip_frames_
        = 0;

    track_face_
        = true;
}

TrackState FaceTracker::track(const cv::Mat& frame, TrackInfo& track_info)
{
    if (gLOGGING) {
        timeRecorder_.reset();
        timeRecorder_.start();
    }

    if ( ++skip_frames_ > max_skip_frames_) {
        return TrackState::TERMINATED;
    }

    cv::Rect2d rectangle;
    tracker_
        ->update(frame, rectangle);

    if (gLOGGING) {
        timeRecorder_.stop();
        std::cout 
            << " - tracking took: "
            << timeRecorder_.getTimeMilli()
            << "ms" << std::endl;
    }

    track_info.track_id
        = track_id_;
    track_info.rectangle
        = rectangle;
    track_info.score
        = current_score_;

    bool skip_frame
        =    rectangle.width <= 0 
          || rectangle.height <= 0;

    return skip_frame ? 
          TrackState::SKIPPED 
        : TrackState::ACTIVE;
}
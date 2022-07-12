#include "face_tracker.h"
#include "face_context.h"

void FaceTracker::init(const cv::Mat& frame, cv::Rect2d& tracked_face) 
{
    tracker_
        = cv::TrackerKCF::create();
    tracker_
        ->init(frame, tracked_face);

    skip_frames_
        = 0;

    track_face_
        = true;
}

void FaceTracker::deinit()
{

}

void FaceTracker::track(const cv::Mat& frame, cv::Rect2d& tracked_face)
{
    if (gLOGGING) {
        timeRecorder_.reset();
        timeRecorder_.start();
    }

    tracker_
        ->update(frame, tracked_face);

    if (gLOGGING) {
        timeRecorder_.stop();
        std::cout 
            << " - tracking took: "
            << timeRecorder_.getTimeMilli()
            << "ms" << std::endl;
    }

    skip_frames_++;
}

bool FaceTracker::is_tracking() 
{
    track_face_ 
        =  track_face_
        && (skip_frames_ < max_skip_frames_);

    return track_face_;
}
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

std::vector<cv::Rect2d> FaceTracker::track(const cv::Mat& frame, 
                                           std::vector<cv::Rect>& detections)
{
    if (gLOGGING) {
        timeRecorder_.reset();
        timeRecorder_.start();
    }

    if ( ! detections.empty()) {
        cv::Rect2d detection = detections[0];
        init(frame, detection);
    }

    std::vector<cv::Rect2d> tracked_detections;
    if ( ! is_tracking()) {
        return tracked_detections;
    }

    cv::Rect2d tracked_detection;
    tracker_
        ->update(frame, tracked_detection);

    if (gLOGGING) {
        timeRecorder_.stop();
        std::cout 
            << " - tracking took: "
            << timeRecorder_.getTimeMilli()
            << "ms" << std::endl;
    }

    skip_frames_++;

    tracked_detections.push_back(tracked_detection);
    return tracked_detections;
}

bool FaceTracker::is_tracking() 
{
    track_face_ 
        =  track_face_
        && (skip_frames_ < max_skip_frames_);

    return track_face_;
}
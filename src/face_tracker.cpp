#include "face_tracker.h"
#include "face_context.h"

void FaceTracker::init(const cv::Mat& frame, cv::Rect2d& tracked_face) 
{
    tracker_
        = cv::TrackerKCF::create();
    tracker_
        ->init(frame, tracked_face);
}

void FaceTracker::deinit()
{

}

void FaceTracker::track(const cv::Mat& frame, cv::Rect2d& tracked_face)
{
    tracker_
        ->update(frame, tracked_face);
}
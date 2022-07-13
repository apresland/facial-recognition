#include "face/tracker.h"
#include "face/context.h"

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

    current_track_id_++;
}

void FaceTracker::trackAsync(const cv::Mat& frame, std::vector<Detection> detections)
{
    detections_future_
        = std::async(std::launch::async, [=](){
            return track(frame, detections);
            });
}

std::vector<TrackInfo> FaceTracker::getAsync()
{
    detections_future_.wait();
    return detections_future_.get();   
}

std::vector<TrackInfo> FaceTracker::track(const cv::Mat& frame, 
                                           std::vector<Detection> detections)
{
    if (gLOGGING) {
        timeRecorder_.reset();
        timeRecorder_.start();
    }

    if ( ! detections.empty()) {
        cv::Rect2d rectangle = detections[0].rectangle;
        init(frame, rectangle);
    }

    std::vector<TrackInfo> track_infos;
    if ( ! is_tracking()) {
        return track_infos;
    }

    skip_frames_++;

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

    TrackInfo track_info;
    track_info.track_id
        = current_track_id_;
    track_info.rectangle
        = rectangle;
    track_infos
        .push_back(track_info);

    return track_infos;
}

bool FaceTracker::is_tracking() 
{
    track_face_ 
        =  track_face_
        && (skip_frames_ < max_skip_frames_);

    return track_face_;
}
#include "face/multitracker.h"

void Multitracker::add(const cv::Mat& frame, Detection& detection)
{  
    // ---------------------------------------
    // try to match to existing tracked object
    // ---------------------------------------

    float iou_max = 0.0;
    uint32_t matched_tracker_key;
    for (const auto & object : objects_) {

        // calculate IoU metric
        auto iou = intersection_over_union(
            object.second,
            detection.rectangle);
        
        // check best match
        if (iou > iou_max) {
            iou_max = iou;
            matched_tracker_key = object.first;
        }
    } 

    // ---------------------------------------
    // if match found reinitialize tracker
    // ---------------------------------------

    if (iou_max >= REQUIRED_IOU_FOR_MATCH) {

        if (detection.score 
                > scores_[matched_tracker_key]) {
            scores_[matched_tracker_key] 
                = detection.score;
            DetectionDescr descr;
            descr.track_id = matched_tracker_key;
            descr.rectangle = detection.rectangle;
            descr.score = detection.score;
            observer_
                .bestShot(
                    frame,
                    descr);   
        }

        trackers_[matched_tracker_key]
            ->init(
                frame, detection);
        return;   
    }

    // ---------------------------------------
    // if no match found create new tracker
    // ---------------------------------------

    ++current_track_id_;

    trackers_[current_track_id_]
        = std::unique_ptr<FaceTracker>(
            new FaceTracker(current_track_id_)
            );

    objects_
        .insert(
            {current_track_id_, detection.rectangle}
            );

    scores_
        .insert( 
            {current_track_id_, detection.score}
            );

    observer_
        .trackStart(
            current_track_id_);

    DetectionDescr descr;
    descr.track_id = current_track_id_;
    descr.rectangle = detection.rectangle;
    descr.score = detection.score;
    observer_
        .bestShot(
            frame,
            descr);   

    return trackers_[current_track_id_]
        ->init(
            frame, detection);
}

std::vector<TrackInfo> Multitracker::track(const cv::Mat& frame) 
{
    std::vector<uint32_t> eviction_list;
    std::vector<TrackInfo> track_infos;

    for(const auto & item : trackers_) {

        uint32_t tracker_key = item.first;
        const auto & tracker = item.second;

        TrackInfo track_info;
        TrackState track_state
            = tracker
                ->track(
                    frame, track_info);

        switch (track_state)
        {

        case TrackState::ACTIVE:
            track_infos
                .push_back(track_info);
            objects_[tracker_key]
                = track_info.rectangle;
            break;

        case TrackState::TERMINATED:
            observer_
                .trackEnd(
                    track_info.track_id);
            objects_
                .erase(
                    tracker_key);
            eviction_list
                .push_back(
                    tracker_key);
            break;

        case TrackState::SKIPPED:
            // try next
            break;

        default:
            break;
        }
    }

    for (const auto & key : eviction_list) {
        trackers_.erase(key);
    }

    return track_infos;
}

void Multitracker::trackAsync(const cv::Mat& frame)
{
    detections_future_
        = std::async(std::launch::async, [=](){
            return track(frame);
            });
}

std::vector<TrackInfo> Multitracker::getAsync()
{
    detections_future_.wait();
    return detections_future_.get();   
}

double Multitracker::intersection_over_union(cv::Rect2d r1, cv::Rect2d r2)
{
    cv::Rect2d r3 = r1 & r2;

    double areaR1 = r1.width * r1.height;
	double areaR2 = r2.width * r2.height;
    double areaR3 = r3.width * r3.height;

    return (areaR3 / (areaR1 + areaR2 - areaR3));
}
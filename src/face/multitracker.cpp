#include "face/multitracker.h"


void Multitracker::add(const cv::Mat& frame, Detection& detection)
{  
    // ---------------------------------------
    // try to match to existing tracked object
    // ---------------------------------------

    double iou_max = 0.0;
    uint32_t imax;
    for (const auto & object : objects) {

        // calculate IoU metric
        auto iou = intersection_over_union(
            object.second,
            detection.rectangle);
        
        // check best match
        if (iou > iou_max) {
            iou_max = iou;
            imax = object.first;
        }
    } 

    // ---------------------------------------
    // if match found reinitialize tracker
    // ---------------------------------------

    if (iou_max >= 0.10) {
        trackers[imax]
            ->init(
                frame, detection);
        return;   
    }

    // ---------------------------------------
    // if no match found create new tracker
    // ---------------------------------------

    ++current_track_id_;

    trackers[current_track_id_]
        = std::unique_ptr<FaceTracker>(
            new FaceTracker(current_track_id_)
            );

    objects
        .insert(
            {current_track_id_, detection.rectangle}
            );

    return trackers[current_track_id_]
        ->init(
            frame, detection);
}

std::vector<TrackInfo> Multitracker::track(const cv::Mat& frame) 
{
    std::vector<uint32_t> eviction_list;
    std::vector<TrackInfo> track_infos;

    for(const auto & item : trackers) {

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
            objects[tracker_key]
                = track_info.rectangle;
            break;

        case TrackState::TERMINATED:
            objects
                .erase(tracker_key);
            eviction_list
                .push_back(tracker_key);
            break;

        case TrackState::SKIPPED:
            // try next
            break;

        default:
            break;
        }
    }

    for (const auto & key : eviction_list) {
        trackers.erase(key);
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

    double IoU = areaR3 / (areaR1 + areaR2 - areaR3);
    return IoU;
}
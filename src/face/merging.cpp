#include "face/types.h"
#include "face/merging.h"

std::vector<cv::Rect2d> FaceMerging::merge(std::vector<Detection> detections, 
                                           std::vector<TrackInfo> track_infos) 
{
    std::vector<cv::Rect2d> merged;

    if ( ! detections.empty()) {
        for (const auto & detection : detections) {
            merged.push_back(detection.rectangle);
        }
    } else {    
        for (const auto & info : track_infos) {
            merged.push_back(info.rectangle);
        }
    }

    return merged;
}
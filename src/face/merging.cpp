#include "face/merging.h"

std::vector<DetectionsDescr> FaceMerging::merge(std::vector<Detection> detections, 
                                                std::vector<TrackInfo> track_infos) 
{
    std::vector<DetectionsDescr> merged;

    if ( ! detections.empty()) {

        TrackId track_id = 0;
        if ( !  track_infos.empty()) {
            track_id = track_infos[0].track_id;
        }

        for (const auto & detection : detections) {
            DetectionsDescr descriptor;
            descriptor.track_id = track_id;
            descriptor.rectangle = detection.rectangle;
            descriptor.score = detection.score;
            merged.push_back(descriptor);
        }
    } else {    
        for (const auto & info : track_infos) {
            DetectionsDescr descriptor;
            descriptor.track_id = info.track_id;
            descriptor.rectangle = info.rectangle;
            descriptor.score = info.score;
            merged.push_back(descriptor);
        }
    }

    return merged;
}
#include "face/merging.h"

std::vector<DetectionsDescr> FaceMerging::merge(std::vector<Detection> detections, 
                                                std::vector<TrackInfo> track_infos) 
{
    std::vector<DetectionsDescr> merged;

    if ( ! detections.empty()) {
        for (const auto & detection : detections) {
            DetectionsDescr descriptor;
            descriptor.rectangle = detection.rectangle;
            merged.push_back(descriptor);
        }
    } else {    
        for (const auto & info : track_infos) {
            DetectionsDescr descriptor;
            descriptor.rectangle = info.rectangle;
            merged.push_back(descriptor);
        }
    }

    return merged;
}
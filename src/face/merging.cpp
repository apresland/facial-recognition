#include "face/merging.h"

std::vector<DetectionDescr> FaceMerging::merge(std::vector<Detection> detections, 
                                                std::vector<TrackInfo> track_infos) 
{
    std::vector<DetectionDescr> merged;
    for (const auto & info : track_infos) {
        DetectionDescr descriptor;
        descriptor.track_id = info.track_id;
        descriptor.rectangle = info.rectangle;
        descriptor.score = info.score;
        merged.push_back(descriptor);
    }

    return merged;
}
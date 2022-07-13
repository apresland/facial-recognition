#include "face/frame_selection.h"

bool FaceFrameSelection::select(const int frame_id) 
{

    const int frames_since_last_detection 
        = (frame_id - detector_frame_id_);

    bool is_selected 
        = frames_since_last_detection
            >= detector_step_;

    if (is_selected) {
        detector_frame_id_ = frame_id;
    }

    return is_selected;
}
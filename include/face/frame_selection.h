#pragma once

#include "face/context.h"

class FaceFrameSelection
{
private:
    // The frame_id when the last detection was made
    int detector_frame_id_{0};
public:
    FaceFrameSelection() = default;
    ~FaceFrameSelection() = default;
    bool select(const int frame_id);
};

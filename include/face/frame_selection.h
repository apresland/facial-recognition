#pragma once

class FaceFrameSelection
{
private:
    // Number of frames between full face detections. The lower the number is,
    // the more likely TrackEngine is to detect a new face as soon as it appears.
    // The higher the number, the higher the overall performance.
    // It is used to balance between computation performance and face
    // detection recall.
    const int detector_step_{5};

    // The frame_id when the last detection was made
    int detector_frame_id_{0};
public:
    FaceFrameSelection() = default;
    ~FaceFrameSelection() = default;
    bool select(const int frame_id);
};

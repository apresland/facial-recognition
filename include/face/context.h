#pragma once

constexpr bool gLOGGING = false;
constexpr bool gLOGMAIN = true;
constexpr bool gPREPROCESS = true;
constexpr bool gTRACKING = true;

// Number of frames between full face detections used to balance 
// between computation performance and face recall:
// - lower values are more likely to detect a new face as it appears.
// - higher values give the overall computation performance.
constexpr int DETECTOR_STEP{3};

// Required detection confidence for considering
// a face detection to be a correct detection.
constexpr float DETECTION_THRESHOLD{0.5f};

// When no detections tracking will wait this number of frames
// before considering the track lost and terminating.
constexpr int MAX_SKIP_FRAMES{12};

// Required intersection-over-unions ratio for considering
// a tracked bounding box to match a new detection.
constexpr float REQUIRED_IOU_FOR_MATCH{0.1f};
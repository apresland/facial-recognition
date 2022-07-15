#pragma once

#include<queue>
#include <vector>

#include <opencv2/core.hpp>

#include "face/preprocessor.h"
#include "face/frame_selection.h"
#include "face/detector.h"
#include "face/tracker.h"
#include "face/multitracker.h"
#include "face/merging.h"
#include "face/annotator.h"

class FaceNode {

private:
    FacePreprocessor _preprocessor;
    FaceFrameSelection _selector;
    FaceDetector _detector;
    //FaceTracker _tracker;
    FaceMerging _merging;
    FaceAnnotator _annotator;

    Multitracker _tracker;

    std::vector<Detection> _detected;
    std::vector<TrackInfo> _tracked;
    cv::TickMeter timeRecorder_;
    int _frame_id{0};

private:
    std::queue<cv::Mat>& _image_input;
    std::queue<cv::Mat>& _image_output;

public:
    explicit FaceNode(std::queue<cv::Mat>& input,
                      std::queue<cv::Mat>& output);
    ~FaceNode() = default;

    void spin_once();
};
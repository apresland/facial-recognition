#pragma once

#include<queue>
#include <vector>

#include <opencv2/core.hpp>

#include "face_preprocessor.h"
#include "face_frame_selection.h"
#include "face_detector.h"
#include "face_tracker.h"
#include "face_merging.h"
#include "face_annotator.h"

class FaceNode {

private:
    FacePreprocessor _preprocessor;
    FaceFrameSelection _selector;
    FaceDetector _detector;
    FaceTracker _tracker;
    FaceMerging _merging;
    FaceAnnotator _annotator;

    std::vector<cv::Rect2d> _detected;
    std::vector<cv::Rect2d> _tracked;
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
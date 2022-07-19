#pragma once

#include <memory>
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
#include "face/track_observer.h"

class FaceNode {

private:
    std::unique_ptr<ITrackObserver> _track_observer;
    std::unique_ptr<FacePreprocessor> _preprocessor;
    std::unique_ptr<FaceFrameSelection> _selector;
    std::unique_ptr<FaceDetector> _detector;
    std::unique_ptr<FaceMerging> _merging;
    std::unique_ptr<FaceAnnotator> _annotator;
    std::unique_ptr<Multitracker> _tracker;
    std::vector<Detection> _detected;
    std::vector<TrackInfo> _tracked;
    int _frame_id{0};

private:
    cv::TickMeter timeRecorder_;
    std::queue<cv::Mat>& _image_input;
    std::queue<cv::Mat>& _image_output;

public:
    explicit FaceNode(std::queue<cv::Mat>& input,
                      std::queue<cv::Mat>& output);
    ~FaceNode() = default;

    void spin_once();
};